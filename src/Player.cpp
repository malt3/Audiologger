//
//  Player.cpp
//  Audiologger
//
//  Created by Malte Poll on 15.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

#include "Player.hpp"
#include "logging.hpp"
#include <string.h>

void Player::init(){
    //Allocate memory for the sample buffer
    sampleBlock = (char *)malloc(FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE);
    if( sampleBlock == NULL ) //Test if memory could be allocated
    {
        printError("Could not allocate memory for the sample buffer.\n");
        exit(1);
    }
    
    //Do specific initialization for portaudio and sndfile
    init_portaudio();
    init_sndfile();
}

void Player::init_portaudio(){
    PaError err = Pa_Initialize(); //Initialize portaudio
    if( err != paNoError ){
        printPaError(err);
    }
    
    /* -- setup output -- */
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    printf( "Output device # %d.\n", outputParameters.device );
    printf( "Output LL: %g s\n", Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency );
    printf( "Output HL: %g s\n", Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency );
    outputParameters.channelCount = NUM_CHANNELS;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    err = Pa_OpenStream(
                        &stream,
                        nullptr, /* We don't need an input device, because there is no recording */
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        paNoFlag,
                        NULL, /* no callback, use blocking API */
                        NULL ); /* no callback, so no callback userData */
    if( err != paNoError ){
        printPaError(err);
    }
}

void Player::init_sndfile(){
    fileInfo.format = 0;
    //File info gets set by the library. Only format must be set to 0.
}

Player::Player(){
    //Set standard output file to an empty string
    char empty = '\0';
    setDestPath(&empty);
    init();
}
Player::Player(const char destPath[]){
    //set output file path
    setDestPath(destPath);
    
    init();
}
Player::Player(std::string destPath){
    setDestPath(destPath.c_str());
}
Player::~Player(){
    PaError err;
    err = Pa_CloseStream( stream ); //Close the opened stream
    if( err != paNoError ){
        printPaError(err);
    }
    err = Pa_Terminate( ); //Terminate portaudio correctly
    if( err != paNoError ){
        printPaError(err);
    }
    
    //Free memory
    if(destPath) delete [] destPath;
    if(sampleBlock) delete [] sampleBlock;
    if (worker) delete worker;
}

void playbackThread(PaStream* stream, char* sampleBlock, SNDFILE* filePtr, bool* stopPlaybackThread, std::mutex* stopPlaybackThread_mutex, bool* playbackThreadFinishedPlaying, std::mutex* playbackThreadFinishedPlaying_mutex){
    PaError err;
    //uses mutex to be thread safe. stopPlaybackThread could change at any time
    playbackThreadFinishedPlaying_mutex->lock();
    *playbackThreadFinishedPlaying = false;
    playbackThreadFinishedPlaying_mutex->unlock();
    stopPlaybackThread_mutex->lock();
    *stopPlaybackThread = false;
    bool stop = *stopPlaybackThread;
    stopPlaybackThread_mutex->unlock();
    long frames = FRAMES_PER_BUFFER;
    memset(sampleBlock, 0, FRAMES_PER_BUFFER*NUM_CHANNELS*SAMPLE_SIZE);
    while (!stop && frames == FRAMES_PER_BUFFER) {
        frames = sf_readf_float(filePtr, (float*)sampleBlock, FRAMES_PER_BUFFER);
        
        err = Pa_WriteStream( stream, sampleBlock, FRAMES_PER_BUFFER );
        if( err != paNoError ){
            printError("Error in worker thread:");
            printPaError(err);
        }

        stopPlaybackThread_mutex->lock();
        stop = *stopPlaybackThread;
        stopPlaybackThread_mutex->unlock();
    }
    playbackThreadFinishedPlaying_mutex->lock();
    *playbackThreadFinishedPlaying = true;
    playbackThreadFinishedPlaying_mutex->unlock();
    //printLog("* Worker ending!");
}

bool Player::hasPlayerFinished(){
    bool ret;
    playbackThreadFinishedPlaying_mutex.lock();
    ret = playbackThreadFinishedPlaying;
    playbackThreadFinishedPlaying_mutex.unlock();
    return ret;
}

int Player::startPlaying(){
    if (playing) {
        stopPlaying();
    }
    time(&playbackStartingTime);
    filePtr = sf_open(destPath, SFM_READ, &fileInfo); //Opens file with info from init_sndfile()
    //printLog(*new std::string("* Opened File ") + destPath);
    PaError err;
    err = Pa_StartStream( stream ); //Start the input stream
    //printLog("* Stream started");
    if( err != paNoError ){
        printPaError(err);
        return -1;
    }
    playing = true; //set playing bool. This helps external classes to decide if the player is running or not.
    worker = new std::thread(playbackThread, stream, sampleBlock, filePtr, &stopPlaybackThread, &stopPlaybackThread_mutex, &playbackThreadFinishedPlaying, &playbackThreadFinishedPlaying_mutex);
    if (playing) {
        stopPlaying();
    }
    return 0;
}

/*Returns -1 if you try to stop a recording if none was started.
 Returns 0 if everything went fine*/
int Player::stopPlaying(){
    if (!playing) {
        return -1;
    }
    //printLog("* Stopping playback");
    
    stopPlaybackThread_mutex.lock();
    stopPlaybackThread = true;
    stopPlaybackThread_mutex.unlock();
    worker->join();
    stopPlaybackThread = false;
    playbackThreadFinishedPlaying = false;
    
    PaError err;
    err = Pa_StopStream( stream );
    if( err != paNoError ){
        printPaError(err);
        return -1;
    }
    
    sf_close(filePtr);
    fileInfo.format = 0;
    //printLog("* Closed File!");
    
    playing = false;
    return 0;
}

bool Player::isPlaying(){
    return playing;
}

char* Player::getDestPath(){
    char* ret = new char[strlen(destPath)];
    strcpy(ret, destPath);
    return ret;
}

void Player::setDestPath(const char destPath[]){
    this->destPath = new char[strlen(destPath)];
    strcpy(this->destPath, destPath);
}

double Player::getDuration(){
    return duration;
}

void Player::setDuration(double dur){
    duration = dur;
}

double Player::getTimePlayed(){
    if (!playing) {
        return 0;
    }else{
        time_t now;
        time(&now);
        return (now-playbackStartingTime);
    }
}

double Player::getTimeLeft(){
    if (!playing) {
        return 0;
    }else{
        return duration-getTimePlayed();
    }
}

//
//  Recorder.cpp
//  Audiologger
//
//  Created by Malte Poll on 13.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

#include "Recorder.hpp"
#include "logging.hpp"
#include <iostream>



Recorder::Recorder(){
    //Set standard output file to an empty string
    char empty = '\0';
    setDestPath(&empty);

    init();
}
Recorder::Recorder(const char* destPath){
    //set output file path
    setDestPath(destPath);
    
    init();
}

Recorder::Recorder(std::string destPath){
    //set output file path
    setDestPath(destPath.c_str());
    
    init();
}
Recorder::~Recorder(){
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
    if(sampleBlock) delete sampleBlock;
    if (worker) delete worker;
}

void Recorder::init(){
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

void Recorder::init_portaudio(){
    PaError err = Pa_Initialize(); //Initialize portaudio
    if( err != paNoError ){
        printPaError(err);
    }
    
    /* -- setup input -- */
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    printf( "Input device # %d.\n", inputParameters.device ); /* print input device number */
    printf( "Input LL: %g s\n", Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency );
    printf( "Input HL: %g s\n", Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency );
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency ;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    
    
    err = Pa_OpenStream(
                        &stream,
                        &inputParameters,
                        NULL, /* We don't need an output device, because there is no music playback */
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        paNoFlag,
                        NULL, /* no callback, use blocking API */
                        NULL ); /* no callback, so no callback userData */
    if( err != paNoError ){
        printPaError(err);
    }

}

void Recorder::init_sndfile(){
    //Init format for sndfile
    //In this case: Float in a WAV container
    fileInfo.frames = FRAMES_PER_BUFFER;
    fileInfo.samplerate = SAMPLE_RATE;
    fileInfo.channels = NUM_CHANNELS;
    fileInfo.format = (SF_FORMAT_WAV|SF_FORMAT_FLOAT);
}

/*This is the recording thread. It is only called by startRecording() and only stopped by stopRecording(). It runs in the background and only checks if the recording should stop in an infinite loop */
void recordingThread(PaStream* stream, char* sampleBlock, SNDFILE* filePtr, bool* stopRecordingThread, std::mutex* stopRecordingThread_mutex){
    PaError err;
    //uses mutex to be thread safe. stopRecordingThread could change at any time
    stopRecordingThread_mutex->lock();
    //printLog("* Worker started!");
    fflush(stdout);
    bool stop = *stopRecordingThread;
    stopRecordingThread_mutex->unlock();
    while (!stop)
    {
        err = Pa_ReadStream(stream, sampleBlock, FRAMES_PER_BUFFER);
        if( err != paNoError ){
            printError("Error in worker thread:");
            printPaError(err);
        }
        sf_writef_float(filePtr, (float*)sampleBlock, FRAMES_PER_BUFFER);
        stopRecordingThread_mutex->lock();
        stop = *stopRecordingThread;
        stopRecordingThread_mutex->unlock();
    }
    //printLog("* Worker ending!");
    
}

/*Returns -1 if you try to start a recording twice.
 Returns 0 if everything went fine*/
int Recorder::startRecording(){
    if (recording) {
        return -1;
    }
    filePtr = sf_open(destPath, SFM_WRITE, &fileInfo); //Opens file with info from init_sndfile()
    //printLog(*new std::string("* Opened File ") + destPath);
    PaError err;
    err = Pa_StartStream( stream ); //Start the input stream
    //printLog("* Stream started");
    if( err != paNoError ){
        printPaError(err);
        return -1;
    }
    recording = true; //set recording bool. This helps external classes to decide if the recorder is running or not.
    memset(sampleBlock, 0, FRAMES_PER_BUFFER*NUM_CHANNELS*SAMPLE_SIZE);
    worker = new std::thread(recordingThread, stream, sampleBlock, filePtr, &stopRecordingThread, &stopRecordingThread_mutex);
    return 0;
}

/*Returns -1 if you try to stop a recording if none was started.
 Returns 0 if everything went fine*/
int Recorder::stopRecording(){
    if (!recording) {
        return -1;
    }
    //printLog("* Stopping recording");
    
    stopRecordingThread_mutex.lock();
    stopRecordingThread = true;
    stopRecordingThread_mutex.unlock();
    worker->join();
    stopRecordingThread = false;
    
    PaError err;
    err = Pa_StopStream( stream );
    if( err != paNoError ){
        printPaError(err);
        return -1;
    }
    
    sf_close(filePtr);
    //printLog("* Closed File!");
    
    recording = false;
    return 0;
}

bool Recorder::isRecording(){
    return recording;
}


void Recorder::setDestPath(const char *destPath){
    this->destPath = new char[strlen(destPath)];
    strcpy(this->destPath, destPath);
}

char* Recorder::getDestPath(){
    char* ret = new char[strlen(destPath)];
    strcpy(ret, destPath);
    return ret;
}



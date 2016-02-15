//
//  Player.hpp
//  Audiologger
//
//  Created by Malte Poll on 15.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

#pragma once

#include <mutex>
#include <thread>
#include "portaudio.h"
#include "sndfile.hh"

#define NUM_CHANNELS 2
#define PA_SAMPLE_TYPE paFloat32
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (1024)
#define SAMPLE_SIZE (4)
#define NUM_SECONDS 10

class Player{
    /* specific init methods for portaudio and sndfile and constructor */
    void init_portaudio();
    void init_sndfile();
    void init();
    
    /* Class specific data */
    bool playing = false;
    char* destPath = nullptr;
    char* sampleBlock = nullptr; //Buffer for playback. Worker reads it from file
    
    /* portaudio related */
    PaStream *stream = nullptr; //Portaudio Stream Object used to get mic input
    PaStreamParameters outputParameters; //Parameters used to open the stream
    
    /* sndfile related */
    SF_INFO fileInfo; //fileInfo for the sndfile lib
    SNDFILE* filePtr; //pointer to the opened .wav file
    
    /* worker vars */ //Might go into own class later for clarity
    bool stopPlaybackThread = false; //Tells the worker when to stop
    bool playbackThreadFinishedPlaying = false;
    std::mutex stopPlaybackThread_mutex;
    std::mutex playbackThreadFinishedPlaying_mutex;//mutex for variable above. Needed for threadsafety
    std::thread* worker = nullptr; //pointer to worker thread
    
public:
    /* constructor and destructor */
    Player();
    Player(const char destPath[]);
    Player(std::string destPath);
    ~Player();
    
    /* functions to start and stop a playing. Also to get info if it is currently playing */
    int startPlaying();
    int stopPlaying();
    bool hasPlayerFinished();
    bool isPlaying();
    
    /* getter and setter for destPath */
    char* getDestPath();
    void setDestPath(const char destPath[]);
};
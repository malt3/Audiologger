//
//  Recorder.hpp
//  Audiologger
//
//  Created by Malte Poll on 13.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

/*=============================================================================|
|                                                                              |
|  Recorder Class                                                              |
|                                                                              |
|  This Class uses 'portaudio' to get the default sound input (aka microphone) |
|  and write it to a specified wav file using the 'sndfile' lib                |
|                                                                              |
|  It uses a seperate thread to record the audio.                              |
|                                                                              |
==============================================================================*/

/* 
 
 USAGE:
 Use "Recorder(char destPath[])" to set the output file or set it manually with "void setDestPath(char destPath[])"
 Use "int Recorder::startRecording()" and "int Recorder::stopRecording()" to start and stop recordings
 
*/

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

class Recorder{
    /* specific init methods for portaudio and sndfile */
    void init();
    void init_portaudio();
    void init_sndfile();
    
    /* Class specific data */
    bool recording = false; //helps to determine if the recorder is currently recording or not
    char* destPath = nullptr; //Path to the .wav file
    char* sampleBlock = nullptr; //Buffer for mic input. Worker writes it to file when it is full
    
    /* portaudio related */
    PaStream *stream = nullptr; //Portaudio Stream Object used to get mic input
    PaStreamParameters inputParameters; //Parameters used to open the stream
    
    /* sndfile related */
    SF_INFO fileInfo; //fileInfo for the sndfile lib
    SNDFILE* filePtr; //pointer to the opened .wav file
    
    /* worker vars */ //Might go into own class later for clarity
    bool stopRecordingThread = false; //Tells the worker when to stop
    std::mutex stopRecordingThread_mutex; //mutex for variable above. Needed for threadsafety
    std::thread* worker = nullptr; //pointer to worker thread
    
    
    
    
public:
    /* constructor and destructor */
    Recorder();
    Recorder(const char destPath[]);
    Recorder(std::string destPath);
    ~Recorder();
    
    /* functions to start and stop a recording. Also to get info if a recording is currently running */
    int startRecording();
    int stopRecording();
    bool isRecording();
    
    /* getter and setter for destPath */
    char* getDestPath();
    void setDestPath(const char destPath[]);
};

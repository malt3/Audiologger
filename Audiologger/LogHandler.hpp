//
//  LogHandler.hpp
//  Audiologger
//
//  Created by Malte Poll on 14.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

/*
 Handles Audiologs
*/

#pragma once

#include <stdio.h>
#include <string>
#include "Recorder.hpp"
#include "Audiolog.hpp"
#include "LinkedList.hpp"



class LogHandler{
    void init();
    std::string folderPath;
    Recorder* rec = nullptr;
    LinkedList<Audiolog> logList;
    
    
public:
    LogHandler();
    LogHandler(std::string folderPath);
    ~LogHandler();
    
    void setFolderPath(std::string folderPath);
    std::string getFolderPath();
    
    void listAllLogNames();
    void listAllFileNames();
    
    
    //calculates the number of audio logs that are in the specified folder
    int audioLogsOnDisk();
    void readAudioLogsFromDisk();
    
    int recordLog(Audiolog* log);
    //recordLog() without parameters is interactive!!
    int recordLog();
    int editLog(Audiolog* log);
    int deleteLog();
    int deleteLog(int index);
    Audiolog* findByTitle(std::string title);
    Audiolog* getAudiologByIndex(int index);
};
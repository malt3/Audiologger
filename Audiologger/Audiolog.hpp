//
//  Audiolog.hpp
//  Audiologger
//
//  Created by Malte Poll on 14.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

#pragma once

#include <string>
#include "sndfile.hh"
#include "taglib.h"
#include "fileref.h"


typedef struct{
    int year=0;
    int month=0;
    int day=0;
} date_t;

typedef struct{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
} formatted_duration_t;


class Audiolog{
    //init is used for shared init code of the constructors
    void init();
    double duration = 0;
    date_t date;
    std::string title = "";
    std::string fileName = "";
    std::string filePath = "";
    bool fileExists = false;
    void retrieveFileExists();
    
    
public:
    Audiolog();
    Audiolog(std::string title, std::string filePath);
    ~Audiolog();
    
    bool operator<(const Audiolog& right){
        return getFileName()<right.fileName;
    }
    bool operator>(const Audiolog& right){
        return !(*this<right);
    }
    
    double getDuration();
    formatted_duration_t getFormattedDuration();
    void setDuration(double duration);
    
    std::string getTitle();
    void setTitle(std::string title);
    
    std::string getFileName();
    void setFileName(std::string fileName);
    // formFileName is a special string method that uses the title of the Audiolog and a date (current or by date var) to form a standardized file name. Format: YYYY-mm-dd-title.wav
    std::string formFileName(const time_t timestamp);
    std::string formFileName(); //now
    
    std::string getFilePath();
    void setFilePath(std::string filePath);
    
    bool getFileExists();
    void setFileExists(bool fileExists);

    void retrieveFileInfo();
    void writeFileInfo();
    
    //Moves/renames recorded file to newFileName. returns 0 on succes. Otherwise -1.
    int renameFile(std::string newFileName);
    
    void setToCurrentDate();
    void setDate(const time_t timestamp);
    void setDate(date_t date);
    
    int getYear();
    void setYear(int year);
    
    int getMonth();
    void setMonth(int month);
    
    int getDay();
    void setDay(int day);
    
};
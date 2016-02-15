//
//  Audiolog.cpp
//  Audiologger
//
//  Created by Malte Poll on 14.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

#include "Audiolog.hpp"
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <sys/stat.h>
#include <cstdlib>

void Audiolog::init(){
    
}

Audiolog::Audiolog(){
    init();
}

Audiolog::~Audiolog(){
    
}

Audiolog::Audiolog(std::string title, std::string filePath){
    setTitle(title);
    setFilePath(filePath);
}


double Audiolog::getDuration(){
    if (duration == 0) {
        if (getFileExists()) {
            retrieveFileInfo();
        }
    }
    return duration;
}

formatted_duration_t Audiolog::getFormattedDuration(){
    formatted_duration_t ret;
    int localSeconds = (int)getDuration();
    ret.hours = localSeconds / 3600;
    localSeconds -= ret.hours*3600;
    ret.minutes = localSeconds / 60;
    localSeconds -= ret.minutes*60;
    ret.seconds = localSeconds;
    return ret;
}

void Audiolog::setDuration(double duration){
    this->duration = duration;
}

std::string Audiolog::getFileName(){
    if (fileName.empty()) {
        //If the filename is empty, we will try to automatically set it with formFileName()
        if (!title.empty()) {
            setFileName(formFileName());
        }
    }
    return fileName;
}

void Audiolog::setFileName(std::string fileName){
    this->fileName = fileName;
}

std::string Audiolog::getTitle(){
    return title;
}

void Audiolog::setTitle(std::string title){
    this->title = title;
}

std::string Audiolog::formFileName(const time_t timestamp){
    setDate(timestamp);
    return formFileName();
}
std::string Audiolog::formFileName(){
    if (date.year == 0 || date.month == 0 || date.day == 0) {
        setToCurrentDate();
    }
    std::string month = std::to_string(date.month);
    if (month.length() == 1) month = "0"+month;
    std::string day = std::to_string(date.day);
    if (day.length() == 1) day = "0"+day;
    
    std::string ret = std::to_string(date.year)+"-"+month+"-"+day+"-"+title+".wav";
    replace(ret.begin(), ret.end(), ' ', '-');
    return ret;
}

std::string Audiolog::getFilePath(){
    return filePath;
}

void Audiolog::setFilePath(std::string filePath){
    this->filePath = filePath;
}

void Audiolog::setToCurrentDate(){
    time_t rawtime;
    time(&rawtime);
    setDate(rawtime);
}

void Audiolog::setDate(const time_t timestamp){
    struct tm * timeinfo;
    char year[5];
    char month[3];
    char day[3];
    timeinfo = localtime(&timestamp);
    //strftime(buffer,80,"%Y-%m-%d",timeinfo);
    strftime(year,5,"%Y",timeinfo);
    strftime(month,3,"%m",timeinfo);
    strftime(day,3,"%d",timeinfo);
    date.year = atoi(year);
    date.month = atoi(month);
    date.day = atoi(day);
}

int Audiolog::getYear(){
    return date.year;
}
void Audiolog::setYear(int year){
    date.year = year;
}

int Audiolog::getMonth(){
    return date.month;
}
void Audiolog::setMonth(int month){
    date.month = month;
}

int Audiolog::getDay(){
    return date.day;
}
void Audiolog::setDay(int day){
    date.day = day;
}

bool Audiolog::getFileExists(){
    if (fileName.empty() && !title.empty()) {
        setFileName(formFileName());
    }
    retrieveFileExists();
    return fileExists;
}

void Audiolog::setFileExists(bool fileExists){
    this->fileExists = fileExists;
}

void Audiolog::retrieveFileExists(){
    struct stat buffer;
    fileExists = (stat ((filePath+fileName).c_str(), &buffer) == 0);
    
}

void Audiolog::retrieveFileInfo(){
    if (getFileExists()) {
        //get duration
        SF_INFO infoStruct;
        infoStruct.format = 0;
        sf_open((filePath+fileName).c_str(), SFM_READ, &infoStruct);
        duration = (infoStruct.frames / (double)(infoStruct.samplerate));
        //get title
        TagLib::FileRef f((filePath+fileName).c_str());
        title = f.tag()->title().to8Bit();
#warning add more tags
    }
}

void Audiolog::writeFileInfo(){
    TagLib::FileRef f((filePath+fileName).c_str());
    f.tag()->setTitle(title);
    f.save();
}


int Audiolog::renameFile(std::string newFileName){
    if(!rename((filePath+fileName).c_str(), (filePath+newFileName).c_str())){
        setFileName(newFileName);
        return 0;
    }
    return -1;
    
}
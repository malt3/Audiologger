//
//  LogHandler.cpp
//  Audiologger
//
//  Created by Malte Poll on 14.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

#include "LogHandler.hpp"
#include "dirent.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>

char* create_c_string(std::string in){
    std::string completeFileName = in;
    const char* temp = in.c_str();
    char* out = new char[in.length()];
    strcpy(out, temp);
    return out;
}


std::string getFileExtension(const std::string& FileName)
{
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".")+1);
    return "";
}

void LogHandler::listAllFileNames(){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(getFolderPath().c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if(*new std::string(getFileExtension(ent->d_name)) == "wav"){
                std::cout << ent->d_name << std::endl;
            }
        }
        closedir (dir);
    }
}

void LogHandler::listAllLogNames(){
    for (int i=0; i<logList.getNumObjects(); i++) {
        formatted_duration_t dur = logList.getVarAt(i)->getFormattedDuration();
        std::cout << i+1 << " " << logList.getVarAt(i)->getTitle() << "\t|\t " << dur.hours<<":"<<dur.minutes<<":"<<dur.seconds << std::endl;
    }
}

void LogHandler::init(){
    rec = new Recorder();
    player = new Player();
    readAudioLogsFromDisk();
}

LogHandler::LogHandler(){
    init();
}

LogHandler::LogHandler(std::string folderPath){
    setFolderPath(folderPath);
    init();
}

LogHandler::~LogHandler(){
    delete rec;
    logList.clear();
}

int LogHandler::audioLogsOnDisk(){
    int amount = 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(getFolderPath().c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            if(*new std::string(getFileExtension(ent->d_name)) == "wav"){
                amount++;
            }
        }
        closedir (dir);
    }
    return amount;
}

void LogHandler::readAudioLogsFromDisk(){
    int i = 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(getFolderPath().c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if(*new std::string(getFileExtension(ent->d_name)) == "wav"){
                logList.push(new Audiolog("", getFolderPath()));
                logList.fst()->setFileName(ent->d_name);
                logList.fst()->retrieveFileInfo();
                i++;
            }
        }
        closedir (dir);
    }
}

int LogHandler::deleteLog(){
    std::cout << "Wich log should be deleted?" << std::endl;
    listAllLogNames();
    std::cout << "Please enter the number of the log or type q to quit";
    std::string str_index;
    std::cin >> str_index;
    if (str_index == "q" || str_index == "Q") {
        std::cout << "Will not delete anything!" << std::endl;
        return -1;
    }
    int index = std::stoi(str_index)-1;
    
    if (!logList.getVarAt(index)->getFileExists()) {
        std::cout << "Audio file does not exist. Cannot be deleted." << std::endl;
        return -1;
    }
    
    std::cout << "You are about to delete the log '"<<logList.getVarAt(index)->getTitle()<<"'. Are you sure? (Type 'y' or 'Y' to confirm)" << std::endl;
    std::string confirmation;
    std::cin >> confirmation;
    if (confirmation != "y" && confirmation != "Y") {
        std::cout << "Will not delete the log." << std::endl;
        return -1;
    }
    return deleteLog(index);
}
int LogHandler::deleteLog(int index){
    if (logList.getVarAt(index)->getFileExists()) {
        int res = remove((logList.getVarAt(index)->getFilePath()+logList.getVarAt(index)->getFileName()).c_str());
        if (res == 0) {
            logList.deleteAt(index);
            return 0;
        }
    }
    return -1;
}

int LogHandler::recordLog(){
    std::string titleForNewLog = "";
    getline(std::cin, titleForNewLog);
    while(titleForNewLog.empty()){
        std::cout << "What title should the new audiolog have?: ";
        getline(std::cin, titleForNewLog);
        std::cout << std::endl;
    }
    Audiolog* newLog = new Audiolog(titleForNewLog, folderPath);
    return recordLog(newLog);
}

int LogHandler::recordLog(Audiolog* log){
    std::string completeFileName = log->getFilePath()+log->getFileName();
    rec->setDestPath(completeFileName.c_str());
    std::cout << "The recording of " << log->getTitle() << " starts when you press ENTER" <<std::endl;
    std::string userinput;
    getline(std::cin, userinput);
    
    rec->startRecording();
    std::cout << "Stop by pressing ENTER" << std::endl;
    getline(std::cin, userinput);
    rec->stopRecording();
    log->writeFileInfo();
    logList.push(log);
    return 0;
}

int LogHandler::playLog(){
    int indexOfLog = 0;
    listAllLogNames();
    std::cout << "What audiolog should be played?: ";
    std::cin >>indexOfLog;
    std::cout << std::endl;
    indexOfLog--;
    return playLog(logList.getVarAt(indexOfLog));
}

int LogHandler::playLog(Audiolog *log){
    std::string completeFileName = log->getFilePath()+log->getFileName();
    player->setDestPath(completeFileName.c_str());
    
    char userinput;
    
    player->startPlaying();
    int secondsPlayed = 0;
    while (!player->hasPlayerFinished()) {
        sleep(1);
        secondsPlayed++;
        printf("Played %.2f percent\n", 100*secondsPlayed/log->getDuration());
    }
    player->stopPlaying();
#warning find a way to SAFELY input sth to stop!
    return 0;
}

Audiolog* LogHandler::getAudiologByIndex(int index){
    return logList.getVarAt(index);
}

void LogHandler::setFolderPath(std::string folderPath){
    this->folderPath = folderPath;
}

std::string LogHandler::getFolderPath(){
    return folderPath;
}



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
#include <string.h>
#include <algorithm>

char* create_c_string(std::string in){
    std::string completeFileName = in;
    const char* temp = in.c_str();
    char* out = new char[in.length()];
    strcpy(out, temp);
    return out;
}

inline bool isInteger(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
    
    char * p ;
    strtol(s.c_str(), &p, 10) ;
    
    return (*p == 0) ;
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

void LogHandler::displayPlaybackInfo(){
    if (!player->isPlaying()) {
        std::cout << "Noting is being played right now. You can play a log by typing 'p'" << std::endl;
    }else{
        std::cout << "Now playing \""<<currentlyPlayed->getTitle()<<"\"\nPlayed "<<100*player->getTimePlayed()/player->getDuration()<<"%\n";
    }
}

int LogHandler::deleteLog(){
    std::cout << "Wich log should be deleted?" << std::endl;
    listAllLogNames();
    std::cout << "Please enter the number of the log or type q to quit" << std::endl;
    std::string str_index;
    std::cin >> str_index;
    if (str_index == "q" || str_index == "Q") {
        std::cout << "Will not delete anything!" << std::endl;
        return -1;
    }
    if (!isInteger(str_index)) {
        std::cout << "You did not enter a valid number." << std::endl;
        return -1;
    }
    int index = std::stoi(str_index)-1;
    
    if (index >= logList.getNumObjects() || index < 0) {
        std::cout << "There is no log with this number" << std::endl;
        return -1;
    }
    
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

int LogHandler::recordLog(std::string title){
    Audiolog* newLog = new Audiolog(title, folderPath);
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

void LogHandler::waitForPlaybackEnd(){
    while (player->isPlaying()) {
        sleep(1);
    }
}

int LogHandler::playLog(){
    std::string userinput;
    int indexOfLog = 0;
    listAllLogNames();
    std::cout << "What audiolog should be played?: ";
    //while (getchar()!='\n') {}
    getline(std::cin, userinput);
    if(!isInteger(userinput)){
        std::cout << "Please enter a number" <<std::endl;
        return -1;
    }
    indexOfLog = stoi(userinput);
    std::cout << std::endl;
    indexOfLog--;
    if (indexOfLog >= logList.getNumObjects() || indexOfLog < 0) {
        std::cout << "Not a valid log number." <<std::endl;
        return -1;
    }
    return playLog(logList.getVarAt(indexOfLog));
}

int LogHandler::playLog(int number){
    number--;
    if (number >= logList.getNumObjects() || number < 0) {
        std::cout << "Not a valid log number." <<std::endl;
        return -1;
    }
    return playLog(logList.getVarAt(number));
}

int LogHandler::playLog(Audiolog *log){
    std::string completeFileName = log->getFilePath()+log->getFileName();
    player->setDestPath(completeFileName.c_str());
    currentlyPlayed = log;
    
    //char userinput;
    player->setDuration(log->getDuration());
    player->startPlaying();
    return 0;
}

int LogHandler::stopLog(){
    if(player->isPlaying()){
        player->stopPlaying();
        currentlyPlayed = nullptr;
        return 0;
    }
    return -1;
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



//
//  logging.cpp
//  Audiologger
//
//  Created by Malte Poll on 14.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

#include "logging.hpp"

void printLog(char message[]){
    printLog(*new std::string(message));
}
void printLog(std::string message){
    std::cout << "Log: " << message << std::endl;
}

void printError(char message[]){
    printError(*new std::string(message));
}
void printError(std::string message){
    std::cerr << "Error: " << message << std::endl;
}

/* print portaudio errors*/
void printPaError(PaError err){
    printError(*new std::string("PortAudio error: ")+Pa_GetErrorText(err));
}
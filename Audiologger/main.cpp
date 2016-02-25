//
//  main.cpp
//  Audiologger
//
//  Created by Malte Poll on 13.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

#include <iostream>
#include "Recorder.hpp"
#include "LogHandler.hpp"
#include "Audiolog.hpp"
#include "LinkedList.hpp"
#include <string>
#include <stdio.h>
#include <time.h>
#include <wordexp.h>

#define standardPath "~/audiologs/"

using namespace std;

string shellExtension(string inputPath){
    wordexp_t exp_result;
    
    wordexp(inputPath.c_str(), &exp_result, 0);
    string resultingPath = exp_result.we_wordv[0];
    wordfree(&exp_result);
    return resultingPath;
}

int main(int argc, char* argv[]){
    string filePath = standardPath;
    if (argc>1) {
        filePath = argv[1];
    }
    
    filePath = shellExtension(filePath);
    
    LogHandler* handler = new LogHandler(filePath);
    char userInput = '\0';
    while (userInput != 'q' && userInput != 'Q') {
        cout << "What do you want to do? (type '?' for help)" << endl;
        //Get User Input
        userInput = getchar();
        //read chars until new line so next getchar() can read next user input
        while (getchar()!='\n') {}
        userInput = tolower(userInput);
        
        switch (userInput) {
            case '?':
            case 'h':
                cout << "Help: the commands are \n'?' for help\n'n' for a new log\n'l' for a list of all logs\n'p' to play a log\n'i' to get information on what is currently playing\n's' to stop a running playback\n'e' to edit a log\n'd' to delete a log\n'q' to quit the program" << endl;
                break;
            case 'n':
                handler->recordLog();
                break;
            case 'l':
                handler->listAllLogNames();
                break;
            case 'p':
                handler->playLog();
                break;
            case 's':
                handler->stopLog();
                break;
            case 'i':
                handler->displayPlaybackInfo();
                break;
            case 'e':
                #warning implement edit
                cout << "This feature is not implemented yet."<<endl;
                break;
            case 'd':
                handler->deleteLog();
                break;
            case 'q':
                cout << "Exiting normally" << endl;
                break;
            default:
                cout << "Command not found. Type '?' to get a list of valid commands."<<endl;
                break;
        }
        
    }
    return 0;
}

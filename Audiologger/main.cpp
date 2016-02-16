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

#define standardPath "/Users/malte/audiologs/"

using namespace std;

int main(int argc, char* argv[]){
    string filePath = standardPath;
    if (argc>1) {
        filePath = argv[1];
    }
    LogHandler handler(filePath);
    string userInput = "";
    while (userInput != "q" && userInput != "Q" && userInput != "quit" && userInput != "Quit" && userInput != "exit" && userInput != "Exit") {
        cout << "What do you want to do? (type '?' for help)" << endl;
        cin >> userInput;
        if(userInput == "?" || userInput == "h" || userInput == "H" || userInput == "help") {
            cout << "Help: the commands are \n'?' for help\n'n' for a new log\n'l' for a list of all logs\n'p' to play a log\n'i' to get information on what is currently playing\n's' to stop a running playback\n'e' to edit a log\n'd' to delete a log\n'q' to quit the program" << endl;
        }else if(userInput == "n" || userInput == "N"){
            handler.recordLog();
        }else if(userInput == "l" || userInput == "L"){
            handler.listAllLogNames();
        }else if(userInput == "p" || userInput == "P"){
            handler.playLog();
        }else if(userInput == "s" || userInput == "S"){
            handler.stopLog();
        }else if(userInput == "i" || userInput == "I"){
            handler.displayPlaybackInfo();
        }else if(userInput == "e" || userInput == "E"){
#warning implement edit
        }else if(userInput == "d" || userInput == "D"){
            handler.deleteLog();
        }else if(userInput == "q" || userInput == "Q" || userInput == "quit" || userInput == "Quit" || userInput == "exit" || userInput == "Exit"){
            cout << "Exiting normally" << endl;
        }
    }
    return 0;
}

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
#include "AudiologgerConfig.hpp"
#include "parseoptions.hpp"
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
    bool help = false;
    bool version = false;
    char* title = nullptr;
    char* filename = nullptr;
    char* path = nullptr;
    int position = option_parser(argc, argv, help, version, title, filename, path);
    if (help) {
        cout << "usage: \tAudiologger [--version] [--help] [--title] [--filename] [--path] <command>" << "\n";
        cout << "commands:" << "\n";
        cout << "\tcreate \t\tCreates a new audiolog.\n\t\t\tTitle can be specified with '--title'." << "\n";
        cout << "\tplay \t\tPlays an audiolog.\n\t\t\tA number should be given." << "\n";
        cout << "\tlist \t\tLists all audiologs with their number." << "\n";
        cout << "\tdelete \t\tDeletes the audiolog with the specified number." << "\n";
        cout << "\tedit \t\tEdits the audilog with the specified number." << "\n";
        return 0;
    }
    if (version) {
        cout << "Audiologger version " << Audiologger_VERSION_MAJOR << "." << Audiologger_VERSION_MINOR << "\n";
        return 0;
    }
    
    
    
    command com = nocommand;
    int number = 0;
    command_parser(argc, argv, position, com, number);
    
    /* sanitize filepath */
    string pathStr;
    if(!path){
        pathStr = standardPath;
    }else{
        pathStr = path;
    }
    pathStr = shellExtension(pathStr);
    
    string filenameStr;
    if (filename) {
       filenameStr = filename;

    }else{
        filenameStr = "";
    }
    string titleStr;
    if (title) {
        titleStr = title;
    }else{
        titleStr = "";
    }
    delete [] path;
    delete [] filename;
    delete [] title;
    
    LogHandler* handler = new LogHandler(pathStr);
    
    switch (com) {
        case create:
            if (titleStr.empty()) {
                cout << "You need to specify a title with the '--title' option.\nSee 'Audiologger --help' for more.\n";
                break;
            }
            cout << "Creating new audilog \""<<titleStr<<"\"\n";
            handler->recordLog(titleStr);
            break;
        case play:
            if (number < 1){
                cout << "You need to specify a valid number. Get a list of audiologs with numbers with the 'list' command.\nSee 'Audiologger --help' for more.\n";
                break;
            }
            handler->playLog(number);
            handler->waitForPlaybackEnd();
            break;
        case lst:
            handler->listAllLogNames();
            break;
        case del:
            handler->deleteLog(number);
            break;
        case edit:
            #warning implement edit
            break;
        case nocommand:
#warning put interactive mode here
            break;
    }
    
    
    /*
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
                printVersion();
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
        
    }*/
    delete handler;
    return 0;
}

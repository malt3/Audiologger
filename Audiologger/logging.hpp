//
//  logging.hpp
//  Audiologger
//
//  Created by Malte Poll on 14.02.16.
//  Copyright © 2016 Malte Poll. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "portaudio.h"

void printLog(char message[]);
void printLog(std::string message);

void printError(char message[]);
void printError(std::string message);

void printPaError(PaError err);
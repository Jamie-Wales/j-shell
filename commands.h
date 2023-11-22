//
// Created by Jamie Wales on 08/11/2023.
//

#ifndef JSHELL_COMMANDS_H
#define JSHELL_COMMANDS_H

#include "input.h"

void parseCommand(InputHandler *pHandler); // Function to parse and execute a command
void ls(InputHandler *pHandler);           // Function for 'ls' command
void cd(InputHandler *pHandler);           // Function for 'cd' command
void exitProgram();                        // Function to exit the program
void run(InputHandler *pHandler);          // Function to compile and run a C program

#endif  // JSHELL_COMMANDS_H

//
// Created by Jamie Wales on 08/11/2023.
//

#ifndef JSHELL_COMMANDS_H
#define JSHELL_COMMANDS_H
#include "input.h"

void parseCommand(InputHandler *pHandler);
void ls(InputHandler *pHandler);
void cd(InputHandler *pHandler);
void exitProgram();
void run(InputHandler *pHandler);

#endif  // JSHELL_COMMANDS_H

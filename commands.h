//
// Created by Jamie Wales on 08/11/2023.
//

#ifndef JSHELL_COMMANDS_H
#define JSHELL_COMMANDS_H
#include "input.h"

void parseCommand(InputHandler const *pHandler);
void ls(InputHandler const *pHandler);
void cd(InputHandler const *pHandler);
void exitProgram();
void run(InputHandler const *pHandler);

#endif  // JSHELL_COMMANDS_H

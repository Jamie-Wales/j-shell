//
// Created by Jamie Wales on 08/11/2023.
//

#ifndef JSHELL_OUTPUT_H
#define JSHELL_OUTPUT_H
#include "input.h"

void recoverableError(char *errorMessage);
void promptDelim(InputHandler const *pHandler);
void unrecoverableError(char *errorMessage);


#endif //JSHELL_OUTPUT_H

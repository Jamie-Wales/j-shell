//
// Created by Jamie Wales on 08/11/2023.
//

#ifndef JSHELL_OUTPUT_H
#define JSHELL_OUTPUT_H

#include "input.h"

// Functions for handling output and errors in the shell
void recoverableError(char *errorMessage); // Function to handle recoverable errors
void promptDelim(InputHandler const *pHandler); // Function to output the prompt delimiter
void unrecoverableError(char *errorMessage); // Function to handle unrecoverable errors

#endif //JSHELL_OUTPUT_H

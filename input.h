//
// Created by Jamie Wales on 08/11/2023.
//

#ifndef SHELL_INPUT_H
#define SHELL_INPUT_H

#include <stdio.h>
#include "path.h"

// Structure defining the input handler for the shell
typedef struct InputHandler InputHandler;
void updateCurrentDirectory(InputHandler *pHandler, const char* newPath);

struct InputHandler {
    void (*getInput)(InputHandler* pHandler);
    char* line;
    size_t size;
    void (*getLine)(InputHandler* pHandler);
    char** tokenisedInput;
    void (*tokenise)(InputHandler* pHandler);
    void (*updateCurrentDirecrtory)(InputHandler * pHandler, char* newPath);
    int tokenInputSize;
    Path *path;
};// Function to update the current directory in the InputHandler's path structure
void updateCurrentDirectory(InputHandler *pHandler, const char* newPath);


InputHandler getInputHandler(); // Function to initialize and get an InputHandler

void destructInputHandler(InputHandler* pHandler); // Function to free resources associated with an InputHandler

#endif  // SHELL_INPUT_H

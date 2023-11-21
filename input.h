//
// Created by Jamie Wales on 08/11/2023.
//

#ifndef SHELL_INPUT_H
#define SHELL_INPUT_H
#include <stdio.h>
#include "path.h"

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
};

InputHandler getInputHandler();

void destructInputHandler(InputHandler* pHandler);

#endif  // SHELL_INPUT_H

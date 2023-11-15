//
// Created by Jamie Wales on 08/11/2023.
//

#ifndef SHELL_INPUT_H
#define SHELL_INPUT_H
#include <stdio.h>


typedef struct InputHandler InputHandler;

struct InputHandler {
    void (*getInput)(InputHandler *pHandler);
    char *line;
    size_t size;
    void (*getLine)(InputHandler *pHandler);
    char **tokenisedInput;
    void (*tokenise)(InputHandler *pHandler);
    int tokenInputSize;
};

InputHandler getInputHandler();

void destructInputHandler(InputHandler const *pHandler);

#endif  // SHELL_INPUT_H

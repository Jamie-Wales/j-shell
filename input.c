//
// Created by Jamie Wales on 08/11/2023.
//

#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getInput(InputHandler *pHandler) {
    pHandler->getLine(pHandler);
    pHandler->tokenise(pHandler);
}

void getLine(InputHandler *pHandler) {
    getline(&pHandler->line, &pHandler->size, stdin);
}

void tokenise(InputHandler *pHandler) {
    char *delims = " \t\r\n";
    pHandler->tokenisedInput = (char **)calloc(sizeof(char *), 15);
    char *token = strtok(pHandler->line, delims);
    char **head = pHandler->tokenisedInput;
    int count = 0;

    while (token != NULL) {
        int tokenSize = strlen(token);
        *(head) = (char *)malloc(tokenSize);
        strncpy(*(head), token, tokenSize);
        head++;
        count++;
        token = strtok(NULL, delims);
    }

    pHandler->tokenInputSize = count;
}


InputHandler getInputHandler() {
    InputHandler handler = {
        .getInput = getInput,
        .tokenise = tokenise,
        .getLine = getLine,
        .line = NULL,
        .path = getPath()
    };

    return handler;
}


void destructInputHandler(InputHandler const *pHandler) {
    free(pHandler->line);
    free(pHandler->tokenisedInput);
    destructPath(pHandler->path);
    free(pHandler);
}

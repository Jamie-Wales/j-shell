//
// Created by Jamie Wales on 08/11/2023.
//

#include "input.h"
#define BUFFSIZE 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void getInput(InputHandler* pHandler) {
    pHandler->getLine(pHandler);
    pHandler->tokenise(pHandler);
}

void getLine(InputHandler* pHandler) {
    getline(&pHandler->line, &pHandler->size, stdin);
}

void tokenise(InputHandler* pHandler) {
    const char* delims = " \t\r\n";
    pHandler->tokenisedInput = (char**)calloc(sizeof(char*), 15);
    const char* token = strtok(pHandler->line, delims);
    char** head = pHandler->tokenisedInput;
    int count = 0;

    while (token != NULL) {
        char* buffer = (char*)malloc(BUFFSIZE);
        if (token[0] == '.') {
            char* currentDir = pHandler->path->currentDir;
            strcpy(buffer, currentDir);
           if (strlen(token) > 1) {
                strcat(buffer, token + 1); // Using token+1 to skip '~'
            }
        } else if (token[0] == '~') {
            char* homeDir = pHandler->path->home;
            strcpy(buffer, homeDir);
            if (strlen(token) > 1) {
                strcat(buffer, token + 1); // Using token+1 to skip '~'
            }
        } else {
            strcat(buffer, token);
        }

        token = buffer;
        const int tokenSize = strlen(token);
        *head = (char*)malloc(tokenSize);
        strncpy(*(head), token, tokenSize);
        head++;
        count++;
        token = strtok(NULL, delims);
    }

    pHandler->tokenInputSize = count;

    if (pHandler->tokenInputSize == 1) {
        pHandler->tokenisedInput[1] = pHandler->path->currentDir;
    }

}


InputHandler getInputHandler() {
    const InputHandler handler = {.getInput = getInput,
                                  .tokenise = tokenise,
                                  .getLine = getLine,
                                  .line = NULL,
                                  .path = getPath(),
                                  .updateCurrentDirecrtory =
                                  updateCurrentDirectory,

    };

    return handler;
}

void destructInputHandler(InputHandler* pHandler) {
    free(pHandler->line);
    free(pHandler->tokenisedInput);
}

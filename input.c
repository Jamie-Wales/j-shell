#include "input.h"
#define BUFFSIZE 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "output.h"
#include "path.h"
#include "utils.h"

// Function to read and tokenize user input
void getInput(InputHandler* pHandler) {
    pHandler->getLine(pHandler);
    pHandler->tokenise(pHandler);
}

// Function to read a line of input from stdin
void getLine(InputHandler* pHandler) {
    getline(&pHandler->line, &pHandler->size, stdin);
}

// Function to tokenize the input line into separate commands/arguments
void tokenise(InputHandler* pHandler) {
    const char* delims = " \t\r\n";  // Delimiters for tokenizing
    pHandler->tokenisedInput = (char**)calloc(sizeof(char*), 15);
    char* token = strtok(pHandler->line, delims);
    char** head = pHandler->tokenisedInput;
    int count = 0;

    while (token != NULL) {
        char* buffer = (char*)malloc(BUFFSIZE);
        // Handling special cases for '.' and '~'
        if (token[0] == '.') {
            char* currentDir = pHandler->path->currentDir;
            strcpy(buffer, currentDir);
            if (strlen(token) > 1) {
                strcat(buffer, token + 1); // Append the rest of the token
            }
        } else if (token[0] == '~') {
            char* homeDir = pHandler->path->home;
            strcpy(buffer, homeDir);
            if (strlen(token) > 1) {
                strcat(buffer, token + 1); // Append the rest of the token
            }
        } else {
            strcpy(buffer, token);
        }

        int tokenSize = strlen(buffer);
        *head = (char*)malloc(tokenSize + 1); // +1 for the null terminator
        strncpy(*head, buffer, tokenSize);
        (*head)[tokenSize] = '\0'; // Null terminate the string
        head++;
        count++;
        free(buffer); // Free the temporary buffer
        token = strtok(NULL, delims);
    }

    pHandler->tokenInputSize = count;
    aliasHandler(pHandler); // Handle any aliases

    // Append current directory if there's only one token
    if (pHandler->tokenInputSize == 1) {
        pHandler->tokenisedInput[1] = pHandler->path->currentDir;
    }
}

// Function to update the current directory in the InputHandler's path structure
void updateCurrentDirectory(InputHandler *pHandler, const char* newPath) {
    // Free the old path
    char *pOldPath = pHandler->path->currentDir;
    free(pOldPath);

    // Allocate memory for the new path
    pHandler->path->currentDir = malloc(strlen(newPath) + 1); // +1 for the null terminator
    if (pHandler->path->currentDir == NULL) {
        unrecoverableError("Memory allocation failure");
        return; // Early return in case of allocation failure
    }

    // Copy the new path to the allocated space
    strcpy(pHandler->path->currentDir, newPath);
}


// Function to initialize and return an InputHandler
InputHandler getInputHandler() {
    InputHandler handler = {
        .getInput = getInput,
        .tokenise = tokenise,
        .getLine = getLine,
        .line = NULL,
        .path = getPath(),
        .updateCurrentDirecrtory = updateCurrentDirectory
    };

    return handler;
}

// Function to free allocated memory in InputHandler
void destructInputHandler(InputHandler* pHandler) {
    free(pHandler->line);
    for (int i = 0; i < pHandler->tokenInputSize; i++) {
        free(pHandler->tokenisedInput[i]); // Free each token
    }
    free(pHandler->tokenisedInput);
}
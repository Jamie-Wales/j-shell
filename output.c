//
// Created by Jamie Wales on 08/11/2023.
//

#include "output.h"
#include <stdio.h>
#include "stdlib.h"
// Functions for handling output and errors in the shell

void promptDelim(InputHandler const* pHandler) {
    fprintf(stdout, "\n");
    fprintf(stdout, "Current j-Shell Directory -> :%s \n",
            pHandler->path->currentDir);
    fflush(stdout);
    printf("> ");
}

void recoverableError(char* message) {
    fprintf(stderr, "> %s\n", message);
    fflush(stderr);
}

void unrecoverableError(char* message) {
    fprintf(stdout, "> %s", message);
    fflush(stderr);
    exit(EXIT_FAILURE);
}

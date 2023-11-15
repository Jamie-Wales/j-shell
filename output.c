//
// Created by Jamie Wales on 08/11/2023.
//

#include "output.h"

#include <stdio.h>

#include "stdlib.h"

void promptDelim(InputHandler const* pHandler) {
    fprintf(stdout, "%s", "> ");
    fflush(stdout);
}

void recoverableError(char* message) {
    fprintf(stdout, "%s", message);
    fflush(stdout);
}

void unrecoverableError(char* message) {
    fprintf(stdout, "%s", message);
    fflush(stderr);
    exit(EXIT_FAILURE);
}

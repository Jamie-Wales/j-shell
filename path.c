//
// Created by Jamie Wales on 12/11/2023.
//

#include "path.h"

#include <stdlib.h>
#include <string.h>

#include "output.h"
#define BUFF_SIZE 1024




void updateCurrentDirectory(InputHandler *pHandler, const char* newPath) {
    char *pOldPath = pHandler->path->currentDir;
    pHandler->path->currentDir = malloc(sizeof(strlen(newPath)));
    free(pOldPath);
    if (pHandler->path->currentDir == NULL) {
        unrecoverableError("Memory allocation failure");
    }
    strcpy(pHandler->path->currentDir, newPath);
}



Path* getPath() {
    char path[BUFF_SIZE];
    strcpy(path, getenv("HOME"));
    Path *output = malloc(sizeof(Path));
    output->currentDir = (char*)calloc(strlen(path) + 1, sizeof(char));
    output->home = (char *)calloc (strlen(path)+ 1, sizeof(char));
    strcpy(output->currentDir, path);
    strcpy(output->home, path);

    return output;
}


//
// Created by Jamie Wales on 12/11/2023.
//

#include "path.h"

#include <stdlib.h>
#include <string.h>

Path *getPath() {
    Path *path = malloc(sizeof(Path));
    if (path != NULL) {
        // Allocate memory for the directory string.
        path->currentDir = malloc(sizeof(char) * (strlen(".") + 1));
        if (path->currentDir != NULL) {
            strcpy(path->currentDir, ".");
        } else {
            // Handle malloc failure; for example, free path and return NULL.
            free(path);
            return NULL;
        }
    }
    return path;
};

void updateCurrentDirectory(Path *path, char *newPath) {
    char *oldPath = path->currentDir;
    path->currentDir = (char *) calloc(strlen(newPath), sizeof(char));
    strcpy(path->currentDir, oldPath);
    free(oldPath);
}



void addToCurrentDir(Path *path, char *additionalPath) {
    strcat(path->currentDir, additionalPath);
}

void destructPath(Path *path) {
    if (path != NULL) {
        free(path->currentDir);  // Free the string memory
        free(path);              // Free the struct memory
    }
}
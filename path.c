//
// Created by Jamie Wales on 12/11/2023.
//

#include "path.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "output.h"
#define BUFF_SIZE 1024


Path getPath() {
    size_t path_size = BUFF_SIZE;
    char path[BUFF_SIZE];
    getcwd(path, path_size);

    Path output = {
    .currentDir  = (char *) malloc(strlen(path)),
    };

    strncpy(output.currentDir, path, BUFF_SIZE);

    return output;

}

void updateCurrentDirectory(Path *path, char *newPath) {
    char *temp = realloc(path->currentDir, strlen(newPath) + 1);
    if (temp != NULL) {
        path->currentDir = temp;
        strcpy(path->currentDir, newPath);
    } else {
        // Handle realloc failure
        unrecoverableError("Memory allocation failure");
    }
}


void destructPath(Path *path) {
    if (path != NULL) {
        free(path->currentDir);  // Free the string memory
        free(path);              // Free the struct memory
    }
}
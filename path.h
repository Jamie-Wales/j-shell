//
// Created by Jamie Wales on 12/11/2023.
//

#ifndef JSHELL_PATH_H
#define JSHELL_PATH_H

typedef struct Path {
    char *currentDir;
} Path;

Path getPath();

void destructPath(Path *path);
void addToCurrentDir(Path *path, char *addedPath);
void updateCurrentDirectory(Path *path, char *newPath);

#endif  // JSHELL_PATH_H

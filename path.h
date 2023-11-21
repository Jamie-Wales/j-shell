//
// Created by Jamie Wales on 12/11/2023.
//

#ifndef JSHELL_PATH_H
#define JSHELL_PATH_H

typedef struct Path {
    char *currentDir;
    char *home;
} Path;


Path *getPath();

void destructPath(Path *path);


#endif  // JSHELL_PATH_H

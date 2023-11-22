//
// Created by Jamie Wales on 12/11/2023.
//

#ifndef JSHELL_PATH_H
#define JSHELL_PATH_H

// Struct to hold information about paths used in the shell
typedef struct Path {
    char *currentDir; // Current working directory
    char *home;       // Home directory
} Path;

Path *getPath(); // Function to initialize and get a Path structure

void destructPath(Path *path); // Function to free resources associated with a Path structure
#endif  // JSHELL_PATH_H
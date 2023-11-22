//
// Created by Jamie Wales on 12/11/2023.
//

#ifndef JSHELL_UTILS_H
#define JSHELL_UTILS_H

#include "input.h"

// Function prototypes for utility functions used in the shell
void sigquitHandler(int sig_num); // Handler for SIGQUIT signal
void sigintHandler(int sig_num);  // Handler for SIGINT signal
char *getCurrentDirectory();      // Function to get the current directory (not implemented in provided code)
void aliasHandler();              // Function to handle command aliases
void addAliasToFile(const InputHandler* pHandler); // Function to add an alias to a file

#endif  // JSHELL_UTILS_H

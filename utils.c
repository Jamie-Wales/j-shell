#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/unistd.h>
#define BUFFSIZE 1024
#include "output.h"

// Signal handler for SIGINT (Ctrl+C)
void sigintHandler(int sig_num) {
    // Re-register the handler for subsequent SIGINTs
    signal(SIGINT, sigintHandler);
    printf("\nInterrupted. Type 'exit' to exit the shell.\n");
    fflush(stdout); // Flush the output buffer to ensure message is printed immediately
}

// Signal handler for SIGQUIT (Ctrl+\)
void sigquitHandler(int sig_num) {
    printf("\nQuit signal received. Exiting the shell.\n");
    fflush(stdout); // Ensure the message is printed before exiting

    exit(EXIT_SUCCESS); // Exit the shell
}

// Function to handle aliases from .jShell.txt file
void aliasHandler(InputHandler* pHandler) {
    char command[1024];
    char alias[1024];
    // Check if .jShell.txt exists
    if (access("./.jShell.txt", F_OK) == 0) {
        FILE* fd = fopen("./.jShell.txt", "r");
        // Read and process each alias line
        while (fscanf(fd, "%[^=]=%[^,],", command, alias) != EOF) {
            for (int i = 0; i < pHandler->tokenInputSize; i++) {
                if (strcmp(pHandler->tokenisedInput[i], command) == 0) {
                    char* tokens[BUFFSIZE];
                    int tokenCount = 0;
                    char* token = strtok(alias, " ");
                    // Tokenize the alias
                    while (token != NULL && tokenCount < BUFFSIZE) {
                        tokens[tokenCount++] = token;
                        token = strtok(NULL, " ");
                    }

                    // Shift existing tokens in the input handler
                    for (int j = pHandler->tokenInputSize - 1; j >= i; j--) {
                        pHandler->tokenisedInput[j + tokenCount] = pHandler->tokenisedInput[j];
                    }

                    // Insert new tokens from alias
                    for (int j = 0; j < tokenCount; j++) {
                        pHandler->tokenisedInput[i + j] = strdup(tokens[j]);
                    }
                    pHandler->tokenInputSize += tokenCount - 1;
                    break; // Exit loop after handling alias
                }
            }
        }
        fclose(fd); // Close the file
    }
}

// Function to append a new alias to .jShell.txt file
void addAliasToFile(const InputHandler* pHandler) {
    // Check for sufficient tokens
    if (pHandler->tokenInputSize < 2) {
        recoverableError("Please enter valid alias");
        exit(EXIT_FAILURE);
    }

    // Open .jShell.txt in append mode
    FILE* fd = fopen("./.jShell.txt", "a+");
    if (fd == NULL) {
        recoverableError("Problem loading .jShell.txt");
        exit(EXIT_FAILURE);
    }

    // Write each token followed by a space, and end with a comma
    for (int i = 1; i < pHandler->tokenInputSize; i++) {
        if (i != 1) { fprintf(fd, " "); }
        fprintf(fd, "%s", pHandler->tokenisedInput[i]);
    }
    fprintf(fd, ",");
    fclose(fd); // Close the file

    exit(EXIT_SUCCESS); // Exit indicating success
}

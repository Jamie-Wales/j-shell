//
// Created by Jamie Wales on 12/11/2023.
//

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/unistd.h>

#include "output.h"

void aliasHandler(char *str) {
    char *alias = malloc(1024);
    char *command = malloc(1024);
    if (access("./.jShell.txt", F_OK) == 0) {

        FILE *fd = fopen("./.jShell.txt", "r");
        while (fscanf(fd, "%[^=]=%[^,]", command, alias) != EOF) {
            if (strcmp(command, str) == 0) {
                strcpy(str, alias);
            }
        }


    }

}


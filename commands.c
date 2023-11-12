//
// Created by Jamie Wales on 08/11/2023.
//

#define LINUX "gcc"
#define OSX "clang"

enum { BUFF_SIZE = 256, DIR_PATH_SIZE = 2064 };

#include "commands.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "output.h"

void exitProgram(int exit_type, InputHandler const *pHandler) {
    destructInputHandler(pHandler);
    exit(exit_type);
}

void ls(InputHandler const *pHandler) {
    char *flag = pHandler->tokenisedInput[0];
    if (strcmp(flag, "-j") == 0) {
        // longway
    } else {
        char *program = "/bin/ls";
        char *arguments[] = {program, "-l", pHandler->path->currentDir, NULL};
        if (execvp(program, arguments) == -1) {
            recoverableError("execvp failed");
            printf("\n");
            _exit(EXIT_FAILURE);
        }
    }
}

void cd(InputHandler const *pHandler) {
    if (pHandler->tokenisedInput[1][0] == '.') {
        if (access(pHandler->tokenisedInput[1], F_OK) == 0) {
            updateCurrentDirectory(pHandler->path, pHandler->tokenisedInput[1]);
            fprintf(stdout, pHandler->path->currentDir);
            fflush(stdout);
        } else {
            fprintf(stderr, "cant find . directory");
        }
    } else {
        char newpath[DIR_PATH_SIZE];
        sprintf(newpath, "%s%s", pHandler->path->currentDir,
                pHandler->tokenisedInput[1]);
        if (access(newpath, F_OK) == 0) {
            addToCurrentDir(pHandler->path, pHandler->tokenisedInput[1]);
            fprintf(stdout, "%s", newpath);
            fflush(stdout);
        } else {
            fprintf(stderr, "cant find relative directory");
        }
    }


}

void commandHandler(InputHandler const *pHandler,
                    void (*func_ptr)(InputHandler const *pHandler)) {
    pid_t pid = fork();
    if (pid == 0) {
        func_ptr(pHandler);

        _exit(EXIT_SUCCESS);
    } else if (pid < 0) {
        unrecoverableError("Failed to create process");
    } else {
        wait(NULL);
    }
}

void parseCommand(InputHandler const *pHandler) {
    bool exit = strcmp(pHandler->tokenisedInput[0], "exit") == 0;
    if (exit) {
        exitProgram(EXIT_SUCCESS, pHandler);
    }

    char *commands[] = {"ls", "cd", "run"};
    void (*p[4])(InputHandler const *) = {ls, cd, run};
    char *usrCommand = pHandler->tokenisedInput[0];
    bool isMatch = false;

    for (int i = 0; i < 3; i++) {
        if (strcmp(commands[i], usrCommand) == 0) {
            isMatch = true;
            commandHandler(pHandler, p[i]);
        }
    }

    if (!isMatch) {
        recoverableError(
            "Incorrect command if you are stuck please type 'help'\n");
    }
}

void run(InputHandler const *pHandler) {
    if (access(pHandler->tokenisedInput[1], F_OK) != 0) {
        recoverableError("Cannot find file \n");
        _exit(EXIT_FAILURE);
    }
    char *execName = "output";
    pid_t pid = fork();

    if (pid == 0) {
        execlp(OSX, OSX, pHandler->tokenisedInput[1], "-o", execName, NULL);
        // if this returns then an error has occurred
        recoverableError("Cannot compile file \n");

        _exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // parent process waiting for child process to complete compiling
        int status;
        waitpid(pid, &status, 0);

        if (status == 0) {
            fprintf(stdout, "Compilation successful. Running -> %s \n",
                    execName);
            fflush(stdout);
            int pipe_fds[2];
            pipe(pipe_fds);
            pid_t run_pid = fork();

            if (run_pid == 0) {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[0]);  // close the read in child process
                execl(execName, execName, NULL);
                // again if this returns we've had an error
                recoverableError("excl error \n");
                _exit(EXIT_FAILURE);
            } else if (run_pid > 0) {
                close(pipe_fds[1]);
                char buffer[BUFF_SIZE];
                ssize_t bytes_read = BUFF_SIZE;

                while ((bytes_read = read(pipe_fds[0], buffer,
                                          sizeof(buffer) - 1)) > 0) {
                    buffer[bytes_read] = '\0';
                    fprintf(stdout, "%s \n", buffer);
                    fflush(stdout);
                }

                close(pipe_fds[0]);
                waitpid(run_pid, NULL, 0);

            } else {
                recoverableError("Compilation error \n");
                _exit(EXIT_FAILURE);
            }

        } else {
            recoverableError("Fork error \n");
            _exit(EXIT_FAILURE);
        }

        _exit(EXIT_SUCCESS);
    }
}

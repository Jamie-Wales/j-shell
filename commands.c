//
// Created by Jamie Wales on 08/11/2023.
//

#include "commands.h"
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "utils.h"
#include "output.h"
#include "input.h"

#define LINUX "gcc"
#define OSX "clang"
enum { BUFF_SIZE = 1024 };

typedef void (*CommandFunc)(const InputHandler*);

// Function to cleanly exit the program and free resources
void exitProgram(const int exit_type, const InputHandler* pHandler) {
    destructInputHandler(pHandler);
    exit(exit_type);
}

// Executes a command in a child process
void executeCommand(const CommandFunc command, const InputHandler* pHandler) {
    const pid_t pid = fork();
    if (pid == 0) { // Child process
        signal(SIGINT, sigintHandler);
        command(pHandler);

        _exit(EXIT_SUCCESS);
    } else if (pid > 0) { // Parent process
        waitpid(pid, NULL, 0);
    } else { // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

// Executes a command in parent and child processes
void executeParentCommand(const CommandFunc parent,
                          const InputHandler* pHandler,
                          const CommandFunc child) {
    int status = -1;
    const pid_t pid = fork();
    if (pid == 0) { // Child process
        signal(SIGINT, sigintHandler);
        child(pHandler);
        exit(EXIT_SUCCESS);
    } else if (pid > 0) { // Parent process
        waitpid(pid, &status, 0);
        if (status == 0) {
            parent(pHandler);
        }
    } else { // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

// Displays help information

void help() {
    const char* helpText =
        "Welcome to J-shell, a custom shell built by Jamie Wales\n"
        "Custom Shell Commands Help:\n"
        "---------------------------------\n"
        "cd [directory] - Changes the current directory to [directory].\n"
        "                 Usage: cd path/to/directory\n\n"
        "run [file] - Compiles and runs a C program from [file].\n"
        "             Usage: run filename.c\n\n"
        "exit - Exits the shell.\n\n"
        "help - Displays this help information.\n\n"
        "[command] -flag [directory] checks the path\n"
        "Have tested commands such as, ping, gcc, which, all work correctly\n"
        "Redirection and Piping:\n"
        "-----------------------\n"
        "You can redirect the output of a command to a file using '>'.\n"
        "Example: command > file.txt\n\n"
        "Appending the output to a file is done using '>>'.\n"
        "Example: command >> file.txt\n\n"
        "Pipes allow the output of one command to be used as input for another.\n"
        "Example: command1 | command2\n\n";

    printf("%s", helpText);
}


// Redirects output to a file
void redirect(char* file) {
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND | 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
}

// Changes the current directory
void cd(InputHandler* pHandler) {
    pHandler->updateCurrentDirecrtory(pHandler, pHandler->tokenisedInput[1]);
}

void checkDir(InputHandler* pHandler) {
    if (pHandler->tokenInputSize < 2) {
        recoverableError("Please enter valid directory\n");
        exit(EXIT_FAILURE);
    }
    if (chdir(pHandler->tokenisedInput[1]) != 0) {
        recoverableError("Please enter valid directory\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}


// Function to handle execution of binary commands, including piping and redirection
void checkBin(InputHandler* pHandler) {
    int pipePos = -1;
    // Search for a pipe symbol in the command
    for (int i = 0; i < pHandler->tokenInputSize; i++) {
        if (strcmp(pHandler->tokenisedInput[i], "|") == 0) {
            pipePos = i;
            break;
        }
    }

    // Handle commands with pipes
    if (pipePos != -1) {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        // Fork for the first part of the piped command
        pid_t pid1 = fork();
        if (pid1 == 0) {
            // Redirect STDOUT to the pipe
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);
            signal(SIGINT, sigintHandler);
            // Execute the first part of the command
            InputHandler firstPartHandler = *pHandler;
            firstPartHandler.tokenInputSize = pipePos;
            checkBin(&firstPartHandler);
            _exit(EXIT_SUCCESS);
        }

        // Fork for the second part of the piped command
        pid_t pid2 = fork();
        if (pid2 == 0) {
            // Redirect STDIN from the pipe
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[1]);
            close(pipefd[0]);
            signal(SIGINT, sigintHandler);
            // Execute the second part of the command
            InputHandler secondPartHandler = *pHandler;
            secondPartHandler.tokenisedInput += pipePos + 1;
            secondPartHandler.tokenInputSize -= pipePos + 1;
            checkBin(&secondPartHandler);
            _exit(EXIT_SUCCESS);
        }

        // Close pipe in the parent process and wait for child processes
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
    } else {
        // Handle commands without pipes (including redirection handling)
        char* args[BUFF_SIZE] = {};
        int finalIndex = pHandler->tokenInputSize;
        for (int i = 0; i < pHandler->tokenInputSize; i++) {
            if (strcmp(pHandler->tokenisedInput[i], ">>") == 0) {
                if (i + 1 == pHandler->tokenInputSize) {
                    recoverableError("Enter valid file name");
                    exit(EXIT_FAILURE);
                }
                redirect(pHandler->tokenisedInput[i + 1]);
                finalIndex = i;
                break;
            }

            args[i] = pHandler->tokenisedInput[i];
        }

        // Append current directory if necessary and execute the command
        if (pHandler->tokenInputSize == 2 && pHandler->tokenisedInput[1][0] == '-') {
            args[finalIndex] = pHandler->path->currentDir;
            finalIndex++;
        } else if (pHandler->tokenInputSize == 1) {
            args[finalIndex] = pHandler->path->currentDir;
            finalIndex++;
        }

        args[finalIndex] = NULL;
        execvp(pHandler->tokenisedInput[0], args); // If this returns we have hit an error.
        perror("execvp");
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }
}


// Function to parse and execute a given command based on the user input.
void parseCommand(InputHandler* pHandler) {
    // Check if there is any command entered
    if (pHandler->tokenInputSize == 0) {
        recoverableError("Please enter a valid command \n");
        return;
    }

    // Handle 'exit' command
    if (strcmp(pHandler->tokenisedInput[0], "exit") == 0) {
        exitProgram(EXIT_SUCCESS, pHandler);
        return; // Exit the function after handling 'exit'
    }

    // Handle 'help' command
    if (strcmp(pHandler->tokenisedInput[0], "help") == 0) {
        help();
        return; // Exit the function after displaying help
    }

    // List of built-in commands
    char* commands[] = {"cd", "run", "help", "alias"};

    // Corresponding functions for each built-in command
    CommandFunc commandFunctions[] = {cd, run, help, addAliasToFile, checkDir};
    bool isMatch = false;
    // Execute the appropriate function for the command
    for (int i = 0; i < 4; i++) {
        if (strcmp(commands[i], pHandler->tokenisedInput[0]) == 0) {
            // Special handling for 'cd' command with parent-child process execution
            if (i == 0) {
                executeParentCommand(commandFunctions[0], pHandler, commandFunctions[4]);
            } else {
                executeCommand(commandFunctions[i], pHandler);
            }
            isMatch = true;
            break; // Break the loop once the command is found and executed
        }
    }

    // If the command is not a built-in, check binary commands
    if (!isMatch) {
        executeCommand(checkBin, pHandler);
    }
}

// Function to compile and execute a C program.
void run(InputHandler* pHandler) {
    // Check if the file name is provided
    recoverableError(pHandler->tokenisedInput[1]);
    if (access(pHandler->tokenisedInput[1], F_OK) != 0) {
        recoverableError("Cannot find file \n");
        exit(EXIT_FAILURE);
    }

    char* execName = "output";
    // Fork a process to compile the program
    const pid_t pid = fork();

    if (pid == 0) {
        // Child process: compile the program
        execlp(OSX, OSX, pHandler->tokenisedInput[1], "-o", execName, NULL);
        // If execlp returns, an error has occurred
        recoverableError("Cannot compile file \n");
        exit(EXIT_FAILURE);
    }

    // Parent process: wait for the compilation to complete
    if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        // Check if compilation was successful
        if (status == 0) {
            fprintf(stdout, "Compilation successful. Running -> %s \n", execName);
            fflush(stdout);

            // Set up a pipe to capture output of the executed program
            int pipe_fds[2];
            pipe(pipe_fds);
            const pid_t run_pid = fork();
            if (run_pid == 0) {
                // Child process: execute the compiled program
                dup2(pipe_fds[1], STDOUT_FILENO); // Redirect STDOUT to the pipe
                close(pipe_fds[0]); // Close the read end in the child process
                execl(execName, execName, NULL);
                // If execl returns, an error has occurred
                recoverableError("exec error \n");
                exit(EXIT_FAILURE);
            }

            // Parent process: read and print the output from the executed program
            if (run_pid > 0) {
                close(pipe_fds[1]); // Close the write end in the parent process
                char buffer[BUFF_SIZE];
                ssize_t bytes_read = BUFF_SIZE;
                while ((bytes_read = read(pipe_fds[0], buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[bytes_read] = '\0';
                    fprintf(stdout, "%s \n", buffer);
                    fflush(stdout);
                }
                close(pipe_fds[0]);
                waitpid(run_pid, NULL, 0);
            } else {
                recoverableError("Compilation error \n");
                exit(EXIT_FAILURE);
            }
        } else {
            recoverableError("Fork error \n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
}

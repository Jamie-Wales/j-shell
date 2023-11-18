//
// Created by Jamie Wales on 08/11/2023.
//

#define LINUX "gcc"
#define OSX "clang"
#include <sys/errno.h>

enum { BUFF_SIZE = 1024 };


#include "commands.h"

#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "output.h"

void exitProgram(const int exit_type, const InputHandler* pHandler) {
    destructInputHandler(pHandler);
    exit(exit_type);
}

typedef void (*CommandFunc)(const InputHandler*);


void executeCommand(const CommandFunc command, const InputHandler* pHandler) {
    const pid_t pid = fork();
    if (pid == 0) {
        // Child process
        command(pHandler);
        _exit(EXIT_SUCCESS);
        // Ensure the child process exits after executing the command
    }

    if (pid > 0) {
        // Parent process
        waitpid(pid, NULL, 0);
    } else {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

void executeParent(CommandFunc commandChild, CommandFunc commandParent,
                   const InputHandler* pHandler) {
    const pid_t pid = fork();
    if (pid == 0) {
        // Child process
        commandChild(pHandler);
        _exit(EXIT_SUCCESS);
        // Ensure the child process exits after executing the command
    }

    if (pid > 0) {
        // Parent process
        commandParent(pHandler);
        waitpid(pid, NULL, 0);
    } else {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

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
        "Example: command1 | command2\n\n"
        "Advanced Usage:\n"
        "---------------\n"
        "checkBin - Executes a binary file. This is an internal function.\n"
        "           Usage: checkBin binaryName\n\n"
        "Note: Some commands like 'cd' and 'run' have special functionalities\n"
        "      integrated into the shell and can handle additional operations like\n"
        "      output redirection and piping.";


    printf("%s", helpText);

}




void redirect(char* file) {
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND | 0644);
    if (fd == -1) {
        perror("open");
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd); // Close the original file descriptor
}

void cd(const InputHandler* pHandler) {
    printf("cd");
}

void checkBin(const InputHandler* pHandler) {
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
    args[finalIndex] = NULL;

    execvp(pHandler->tokenisedInput[0], args);
    perror("execvp");
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

void parseCommand(const InputHandler* pHandler) {
    if (pHandler->tokenInputSize == 0) {
        recoverableError("Please enter a valid command \n");
        return;
    }

    if (strcmp(pHandler->tokenisedInput[0], "exit") == 0) {
        exitProgram(EXIT_SUCCESS, pHandler);
    }

    char* commands[] = {"cd", "run", "help"};
    CommandFunc commandFunctions[] = {cd, run, "help"};
    bool isMatch = false;

    for (int i = 0; i < 3; i++) {
        if (strcmp(commands[i], pHandler->tokenisedInput[0]) == 0) {
            executeCommand(commandFunctions[i], pHandler);
            isMatch = true;
            break;
        }
    }

    if (!isMatch) {
        executeCommand(checkBin, pHandler);
    }
}

void run(const InputHandler* pHandler) {
    if (access(pHandler->tokenisedInput[1], F_OK) != 0) {
        recoverableError("Cannot find file \n");
        _exit(EXIT_FAILURE);
    }
    char* execName = "output";
    const pid_t pid = fork();

    if (pid == 0) {
        execlp(OSX, OSX, pHandler->tokenisedInput[1], "-o", execName, NULL);
        // if this returns then an error has occurred
        recoverableError("Cannot compile file \n");

        _exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // parent process waiting for child process to complete compiling
        int status;
        waitpid(pid, &status, 0);

        if (status == 0) {
            fprintf(stdout, "Compilation successful. Running -> %s \n",
                    execName);
            fflush(stdout);
            int pipe_fds[2];
            pipe(pipe_fds);
            const pid_t run_pid = fork();

            if (run_pid == 0) {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[0]); // close the read in child process
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
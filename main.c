#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "input.h"
#include "output.h"

int main() {
    InputHandler  inputHandler = getInputHandler();

    while (true) {
        promptDelim(&inputHandler);
        inputHandler.getInput(&inputHandler);
        parseCommand(&inputHandler);
    }

    exitProgram(EXIT_SUCCESS);
}

/* To do: destructor for inputhandler */

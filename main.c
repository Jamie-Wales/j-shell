#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "input.h"
#include "output.h"
#include "path.h"

int main() {
    InputHandler inputHandler = getInputHandler();
    Path path = getPath();


    while (true) {
        promptDelim(&inputHandler);
        inputHandler.getInput(&inputHandler);
        parseCommand(&inputHandler);
    }

    exitProgram(EXIT_SUCCESS);
}

/* To do: destructor for inputhandler */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "commands.h"
#include "input.h"
#include "output.h"
#include "utils.h"


int main() {
    InputHandler inputHandler = getInputHandler();

    signal(SIGINT, sigintHandler);
    signal(SIGQUIT, sigquitHandler);


    while (true) {
        promptDelim(&inputHandler);
        inputHandler.getInput(&inputHandler);
        parseCommand(&inputHandler);
    }


}

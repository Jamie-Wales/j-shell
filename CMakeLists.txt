cmake_minimum_required(VERSION 3.26)
project(jShell C)

set(CMAKE_C_STANDARD 17)

add_executable(jShell main.c
        input.c
        commands.h
        output.h
        commands.c
        output.c
        path.h
        path.c
        utils.h
        utils.c
        input.h
)

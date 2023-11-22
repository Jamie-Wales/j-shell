# J-shell: A Custom Command Line Shell
J-shell is a bespoke command line interface designed as a student project. The goal was to create a wholeistic shell experience with added functionalities and custom commands. 
**Developed by Jamie Wales** 
*this project aims to create a user-friendly and usable interface for managing and executing commands on Unix-based systems.

## Features
  - Custom commands including `cd`, `run`, and `help`.
  - Alias handling from `.jShell.txt` file for user-defined shortcuts.
  - Input handling with tokenization for command parsing.
  - Signal handling for graceful interruption and termination of the shell.
  - Redirection and piping support for advanced command execution, allowing users to direct the output of commands to files (> and >>) or to use the output of one command as input for another (|).
  - Binary command execution with integrated error handling.
  - Dynamic path management to handle current and home directories.

## Installation
To set up J-shell on your system:

Clone the repository or download the source files.
Ensure you have GCC (for Linux) or Clang (for OSX) installed for the run command functionality.
Compile the source code using your system's C compiler. For example:
`sh`
Copy code
`gcc -o jshell main.c commands.c input.c path.c utils.c`
Run the compiled program:
`sh`
Copy code
`./jshell`
Usage
Upon launching J-shell, you can use the following built-in commands:

`cd` **[directory]**: Change the current working directory.
`run` **[file]**: Compile and execute a C program.
`help` - Display help information about J-shell commands.
`exit` - Exit the shell.
J-shell also supports standard Unix commands, with additional features like output redirection and piping:

Redirection: Redirect the output of a command to a file using >> to create.
`echo jShell is awesome >> ./awesome.txt`

Piping: Use the output of one command as input for another with |.
Example: `command1 | command2`.
Custom Alias Configuration
You can create custom aliases in `.jShell.txt` file in the following format:

```bash

command=alias

```

For example, to create an alias for listing files in long format:

``` bash
Copy code
ls=ls -l,
```

### Contributing
Contributions to J-shell are welcome! If you have suggestions for improvements or new features, feel free to create an issue or submit a pull request.

### License
This project is open-source and available under the MIT License.

# Easily Extendible Shell (EESH)

Easily Extendible Shell is a custom shell written in C++, created as a learning project to explore the inner workings of shells and operating systems. It is designed with simplicity, extensibility, and educational value in mind.
## Features

- Basic Command Execution: Run standard system commands with ease.
- Extensible Design: Add and modify functionality to suit your needs.
- Custom Built-Ins: Experiment with adding built-in commands to expand shell capabilities.
- Interactive and Scripted Modes: Use EES interactively or run pre-written scripts.
- Educational Focus: Learn about process management, I/O redirection, and more.

## Getting Started
### Prerequisites

- A C++17 or newer compiler (e.g., GCC, Clang)
- A Unix-like environment (Linux, macOS, WSL)
- Basic familiarity with command-line interfaces

### Installation

- Clone the repository:
    ```bash
    git clone https://github.com/a-trumier/eesh
    cd easily-extendible-shell
    ```

- Compile the project:
    ```bash
    make
    ```
- Run the shell:
    ```bash
    ./eesh
    ```
## Usage

Start the shell by running ./bin/eesh. Enter commands as you would in a standard shell. Some examples:

- Running a command:
    ```eesh
    ls -l
    ```

- Exiting the shell:
    ```eesh
    exit
    ```

## Extending the Shell

To add new features or modify existing ones, simply edit the source files as needed.

For guidance, see the docs/ directory, which includes examples and explanations of the shell's internal structure.

## Learning Goals

This project is intended to help myself explore:

- Process creation and management (e.g., fork(), exec())
- Parsing and interpreting user input
- I/O redirection and piping
- Error handling in system-level programming
- Modular and maintainable design in C++

## Contributions

This is primarily a learning project, but contributions and suggestions are welcome. Feel free to fork the repository and submit a pull request or open an issue.

## License

This project is released under the MIT License. See LICENSE for details.

# Minishell

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)

A custom implementation of a command-line interpreter, or shell, written entirely in C. This project is a modern take on the classic Bourne-Again Shell (bash), focusing on a modular, clean, and well-documented codebase. It serves as a comprehensive demonstration of advanced C programming, process management, and systems programming concepts.

## 🚀 Features

This shell is packed with features that make it a powerful and flexible tool for interacting with the operating system:

*   **Interactive Command-Line:** Utilizes the `readline` library for an enhanced user experience, including history and line editing.
*   **Command Execution:** Can execute both simple and complex commands by searching for executables in the system's `PATH`.
*   **Built-in Commands:** A suite of essential built-in commands are supported:
    *   `cd`: Change the current directory.
    *   `echo`: Print arguments to the standard output.
    *   `env`: Display environment variables.
    *   `exit`: Terminate the shell.
    *   `export`: Set or display environment variables.
    *   `pwd`: Print the current working directory.
    *   `unset`: Remove environment variables.
*   **Pipes (`|`):** Supports chaining commands, allowing the output of one command to be used as the input for another.
*   **I/O Redirection:** Full support for input and output redirection:
    *   `>`: Redirect standard output.
    *   `<`: Redirect standard input.
    *   `>>`: Append standard output.
    *   `<<`: Here document for multi-line input.
*   **Environment Variable Expansion:** Supports shell-like variable expansion using the `$` syntax.
*   **Robust Signal Handling:** Gracefully handles `Ctrl-C` (SIGINT), `Ctrl-D` (EOF), and `Ctrl-\` (SIGQUIT) to ensure shell stability.
*   **Modular Architecture:** The codebase is organized into distinct modules for parsing, execution, and built-in commands, promoting maintainability and scalability.

## 🛠️ Getting Started

### Prerequisites

*   A C compiler (e.g., `gcc` or `clang`).
*   The `readline` library. On Debian-based systems, you can install it with `sudo apt-get install libreadline-dev`. On macOS with Homebrew, use `brew install readline`.

### Building the Project

1.  Clone the repository:
    ```bash
    git clone https://github.com/tpandya42/minishell.git
    cd minishell
    ```
2.  Build the project by running `make`:
    ```bash
    make
    ```

### Running the Shell

To start the interactive shell, run the following command:

```bash
./minishell
```

## 💻 Usage Examples

Here are some examples of what you can do with Minishell:

*   **Simple Commands:**
    ```bash
    ls -l
    pwd
    ```
*   **Pipes:**
    ```bash
    cat /etc/passwd | grep /bin/bash
    ```
*   **Redirects:**
    ```bash
    echo "Hello, World!" > greeting.txt
    cat < greeting.txt
    ```
*   **Environment Variables:**
    ```bash
    export MESSAGE="Hello from Minishell"
    echo $MESSAGE
    ```

## 📂 Project Structure

The project is organized into a modular structure to enhance readability and maintainability:

```
.
├── include/      # Header files for all modules
├── src/          # Source code files
│   ├── builtin/  # Implementation of built-in commands
│   ├── exec/     # Command execution logic
│   ├── expansion/ # Environment variable expansion
│   ├── lexer/    # Lexical analysis
│   ├── parser/   # Parsing and abstract syntax tree (AST) construction
│   ├── signal/   # Signal handling
│   └── utils/    # Utility functions
├── libft/        # Custom C library
├── Makefile      # Build script
└── README.md     # This file
```

## 🔧 Development

This project emphasizes a high standard of code quality. All code is compiled with `-Wall -Wextra -g` to catch potential errors and facilitate debugging. The project relies on a custom `libft` library, a common practice in 42 school projects, which provides a set of utility functions.

## 🧪 Testing

While this project does not yet have a formal testing suite, it has been tested extensively through manual use and by comparing its behavior with `bash`. A dedicated testing framework is planned for a future release to automate this process.

## ✍️ Authors

*   **Tanmay Pandya** - *Initial work* - [tpandya42](https://github.com/tpandya42)


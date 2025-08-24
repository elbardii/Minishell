# Minishell - As Beautiful as a Shell

A simple shell implementation in C that recreates the functionality of Bash. This project provides extensive knowledge about processes, file descriptors, and shell operations.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Implementation Details](#implementation-details)
- [Built-in Commands](#built-in-commands)
- [Testing](#testing)
- [Contributing](#contributing)

## Overview

Minishell is a recreation of a Unix shell that allows users to interact with the system through command-line interface. It handles command parsing, execution, redirection, pipes, and built-in commands while maintaining compatibility with Bash behavior.

## Features

### Core Functionality
- ✅ Interactive command prompt
- ✅ Command history functionality
- ✅ Executable search via PATH variable or absolute/relative paths
- ✅ Signal handling (Ctrl-C, Ctrl-D, Ctrl-\\)
- ✅ Environment variable expansion
- ✅ Exit status handling (`$?`)

### Redirection Support
- ✅ Input redirection (`<`)
- ✅ Output redirection (`>`)
- ✅ Here documents (`<<`)
- ✅ Append mode (`>>`)

### Advanced Features
- ✅ Pipes (`|`) for command chaining
- ✅ Quote handling (single `'` and double `"`)
- ✅ Environment variable expansion in double quotes
- ✅ Built-in commands implementation

### Built-in Commands
- `echo` with `-n` option
- `cd` with relative/absolute paths
- `pwd` (print working directory)
- `export` for environment variables
- `unset` to remove environment variables
- `env` to display environment
- `exit` to terminate shell

## Prerequisites

- GCC compiler
- GNU Readline library
- Make utility
- Standard C library

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential libreadline-dev
```

**macOS:**
```bash
# Using Homebrew
brew install readline
```

## Installation

1. Clone the repository:
```bash
git clone <repository-url>
cd minishell
```

2. Compile the project:
```bash
make
```

3. Run the shell:
```bash
./minishell
```

## Usage

### Basic Commands
```bash
minishell$ echo "Hello, World!"
Hello, World!

minishell$ ls -la
total 48
drwxr-xr-x  8 user user 4096 Nov 17 10:30 .
drwxr-xr-x  3 user user 4096 Nov 17 10:30 ..

minishell$ pwd
/home/user/minishell
```

### Redirection Examples
```bash
# Output redirection
minishell$ echo "Hello" > output.txt

# Input redirection
minishell$ cat < input.txt

# Append mode
minishell$ echo "World" >> output.txt

# Here document
minishell$ cat << EOF
> This is a heredoc
> Multiple lines supported
> EOF
```

### Pipe Examples
```bash
# Simple pipe
minishell$ ls | grep "minishell"

# Multiple pipes
minishell$ ps aux | grep "bash" | wc -l
```

### Environment Variables
```bash
# Display environment
minishell$ env

# Export new variable
minishell$ export MY_VAR="Hello World"

# Use variable
minishell$ echo $MY_VAR
Hello World

# Check exit status
minishell$ echo $?
0
```

## Project Structure

```
minishell/
├── Makefile                    # Build configuration
├── include/                    # Header files
│   ├── minishell.h            # Main header
│   ├── lexer.h                # Tokenization
│   ├── parser.h               # Command parsing
│   ├── executor.h             # Command execution
│   ├── expander.h             # Variable expansion
│   ├── builtins.h             # Built-in commands
│   ├── env.h                  # Environment handling
│   └── signals.h              # Signal management
├── src/                       # Source files
│   ├── main.c                 # Entry point
│   ├── lexer/                 # Tokenization module
│   ├── parser/                # Command parsing
│   ├── exec/                  # Command execution
│   ├── expander/              # Variable expansion
│   ├── builtins/              # Built-in commands
│   ├── init/                  # Initialization
│   ├── signals/               # Signal handling
│   └── free/                  # Memory management
└── Libft/                     # Custom C library
```

## Implementation Details

### Architecture

The shell follows a modular architecture with distinct phases:

1. **Lexical Analysis**: Input string is tokenized into meaningful units
2. **Expansion**: Environment variables and special parameters are expanded
3. **Parsing**: Tokens are organized into command structures (AST)
4. **Execution**: Commands are executed with proper process management

### Key Components

#### Lexer (`src/lexer/`)
- Tokenizes input into words, operators, and special characters
- Handles quote parsing and preservation
- Identifies redirection operators and pipes

#### Parser (`src/parser/`)
- Builds Abstract Syntax Tree (AST) from tokens
- Handles command grouping and precedence
- Manages redirection and pipe structures

#### Expander (`src/expander/`)
- Expands environment variables (`$VAR`, `$?`)
- Respects quote context (no expansion in single quotes)
- Handles parameter expansion edge cases

#### Executor (`src/exec/`)
- Manages process creation and execution
- Handles redirection setup
- Implements pipe communication
- Manages signal handling for child processes

### Memory Management

The project implements careful memory management:
- All heap-allocated memory is properly freed
- Token structures are cleaned up after parsing
- Command structures are freed after execution
- Environment list is maintained throughout shell lifetime

### Signal Handling

- **SIGINT (Ctrl-C)**: Displays new prompt in interactive mode
- **SIGQUIT (Ctrl-\\)**: Ignored in interactive mode
- **EOF (Ctrl-D)**: Exits the shell gracefully

## Built-in Commands

### `echo`
Displays text with optional formatting.
```bash
minishell$ echo -n "No newline"
minishell$ echo -e "Tab:\tNewline:\n"
```

### `cd`
Changes current directory.
```bash
minishell$ cd /home/user
minishell$ cd ..
minishell$ cd ~
minishell$ cd -  # Previous directory
```

### `pwd`
Prints current working directory.
```bash
minishell$ pwd
/home/user/minishell
```

### `export`
Sets environment variables.
```bash
minishell$ export PATH="/usr/local/bin:$PATH"
minishell$ export DEBUG=1
minishell$ export  # List all variables
```

### `unset`
Removes environment variables.
```bash
minishell$ unset DEBUG
minishell$ unset PATH HOME  # Multiple variables
```

### `env`
Displays all environment variables.
```bash
minishell$ env
PATH=/usr/bin:/bin
HOME=/home/user
USER=user
```

### `exit`
Exits the shell with optional exit code.
```bash
minishell$ exit
minishell$ exit 42
```

## Testing

### Manual Testing
```bash
# Basic functionality
./minishell
minishell$ echo "test" | cat
minishell$ ls > files.txt
minishell$ export TEST=value && echo $TEST

# Error handling
minishell$ /nonexistent/command
minishell$ cd /nonexistent/directory
minishell$ echo "unclosed quote
```

### Automated Testing
Create test scripts to verify functionality:

```bash
#!/bin/bash
# test_basic.sh
echo "=== Testing basic commands ==="
echo "echo test" | ./minishell
echo "pwd" | ./minishell
echo "env" | ./minishell
```

## Contributing

1. Follow the 42 Norm coding standard
2. Write clean, readable code with appropriate comments
3. Test thoroughly before submitting
4. Handle edge cases and error conditions
5. Ensure no memory leaks with valgrind

### Development Guidelines

- Use consistent indentation (tabs)
- Keep functions under 25 lines when possible
- Limit 80 characters per line
- Use meaningful variable and function names
- Free all allocated memory
- Handle all possible error cases

## Compilation Flags

The project compiles with strict flags:
- `-Wall`: Enable all warnings
- `-Wextra`: Enable extra warnings
- `-Werror`: Treat warnings as errors

## External Functions Used

The project is limited to specific system calls and library functions:
- `readline`, `rl_clear_history`, `add_history` (GNU Readline)
- `printf`, `malloc`, `free`, `write` (Standard library)
- `access`, `open`, `read`, `close` (File operations)
- `fork`, `wait`, `waitpid`, `execve` (Process management)
- `signal`, `sigaction`, `kill` (Signal handling)
- `getcwd`, `chdir` (Directory operations)
- `pipe`, `dup`, `dup2` (I/O redirection)

## Known Limitations

- No wildcard expansion (`*`) in mandatory part
- No logical operators (`&&`, `||`) in mandatory part
- No job control or background processes
- Limited to POSIX-compatible systems

## License

This project is part of the 42 School curriculum. Educational use only.

---

**Note**: This implementation focuses on understanding shell mechanics and system programming concepts. It serves as an educational tool for learning about process management, I/O redirection, and command-line parsing.
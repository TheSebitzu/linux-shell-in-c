# linux-shell-in-c

A simple command-line shell implemented in C, providing basic shell functionality. Type commands, execute programs, and interact with your system through your own minimalist shell!

## Features

- Run standard Linux commands (e.g., `ls`, `pwd`, `echo`, etc.)
- Built-in commands: `cd`, `exit`, and `help`
- Handles arguments and simple command parsing
- Displays a prompt and current working directory

## Installation

1. **Clone the repository:**
   ```sh
   git clone https://github.com/TheSebitzu/linux-shell-in-c.git
   cd linux-shell-in-c
   ```

2. **Build the project:**
   ```sh
   gcc -o myshell main.c
   ```
   *(Or use your favorite C IDE or build system)*

## Usage

Run your shell with:
```sh
./myshell
```

- Type any command (like `ls`, `pwd`, `echo hello`, etc.) and press Enter.
- Use `cd <directory>` to change directories.
- Use `help` to display a list of built-in commands and their descriptions.
- Use `exit` to quit the shell.
- Any program available in your system’s `$PATH` can be run directly.

## How It Works

- The shell displays a prompt (usually showing the current working directory).
- It reads a line of input, parses it into commands and arguments.
- If the command is a built-in (`cd`, `exit`, `help`), it handles it internally.
- Otherwise, it uses `fork()` and `execvp()` to run external programs.
- The shell waits for the command to finish before prompting again.

## Limitations

- **No piping or redirection:** Advanced shell features like pipes (`|`), I/O redirection (`<`, `>`) are not supported.
- **No background jobs:** Does not support running commands in the background or job control.
- **Simple parsing:** No quoting, variable expansion, or command substitution.
- **No scripting:** Does not run shell scripts; only interactive use.
- **Only works on Linux:** Not supported on Windows or macOS.

## Requirements

- GCC or any C99-compatible compiler
- **Linux** (should work on most Unix-like distributions)

## License

This project is licensed under the MIT License.

---

*Made by [TheSebitzu](https://github.com/TheSebitzu)*

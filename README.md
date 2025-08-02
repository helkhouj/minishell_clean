# Minishell - Clean Implementation

A complete implementation of the 42 School minishell project.

## Features

### Mandatory Requirements
- ✅ Display prompt and working history (readline)
- ✅ Search and launch executables (PATH variable, relative/absolute paths)
- ✅ Single global variable for signal handling only
- ✅ Quote handling: single quotes (`'`) and double quotes (`"`)
- ✅ All redirections: `<`, `>`, `>>`, `<<` (heredoc)
- ✅ Pipes (`|`) for command chaining
- ✅ Environment variable expansion (`$VAR` and `$?`)
- ✅ Signal handling: Ctrl-C, Ctrl-D, Ctrl-\ (bash-like behavior)

### Built-in Commands
- `echo` with `-n` option
- `cd` with relative or absolute path
- `pwd` (no options)
- `export` (no options)
- `unset` (no options)
- `env` (no options or arguments)
- `exit` (no options)

## Compilation

```bash
make
```

Compiles with `-Wall -Wextra -Werror` flags.

## Usage

```bash
./minishell
```

## Testing

Run the test script to verify all features:

```bash
bash test_minishell.sh
```

## Code Standards

- Follows 42 Norm v4.1 completely
- Maximum 25 lines per function
- Maximum 5 functions per file
- Proper memory management (no leaks)
- Clean error handling

## Project Structure

- `main.c` - Main program loop and initialization
- `tokenizer.c` - Input tokenization with quote handling
- `parser.c` - AST generation from tokens
- `environment.c` - Environment variable management
- `expansion.c` - Variable expansion (`$VAR`, `$?`)
- `executor.c` - Command execution and process management
- `builtins.c` / `builtins2.c` - Built-in command implementations
- `redirections.c` - File I/O redirection handling
- `signals.c` - Signal handling (Ctrl-C, Ctrl-D, Ctrl-\)
- `utils.c` / `utils2.c` - Utility functions
- `error.c` - Error handling and cleanup
- `minishell.h` - Header with all structures and prototypes

## Author

42 School Project Implementation
# trash

A shell written in c with **0 memory allocated on the heap!**

# Requirements
- `make`
- `gcc`

# Installation Instructions
- Clone the repo.
- `cd` into it.
- Run `make`. The binary will be built. Run with `./a.out`.

(or)

- Run `make release`.
- The binary will be built. 
- Run with `./release/trash`.

# Directory Structure
- All code is in the `src` directory.
- Each command's implementation is in `src/commands/<command_name>.c`.
- Some utility functions are in `src/utils/<utility>.c`
-   - `src/utils/base.c`: some common structs used in all other files, like the `command` and `commands` struct.
    - `src/utils/command.c`: the point of execution of all commands. Responsible for parsing, handling piping and I/O redirection, and calling the appropriate command.
    - `src/utils/prompt.c`: the main shell prompt.
    - `src/utils/rawmode.c`: commands for enabling and disabling raw mode, and for reading input in it.
    - `src/utils/singals.c`: signal handlers implemented.
- All the header files are in `src/inc/*.h`

# Assumptions

## General Assumptions
1. A maximum of 16 delimiter `(; &)` separated commands can be executed in a single line.
2. Each command can take a maximum of 16 arguments.
3. Each string can have a maximum of 1024 characters.
4. Flags not recognized by a command will be ignored and the command will be executed accordingly.

## Specific Assumptions
### Input
- Empty input is treated as no command.
- Consecutive delimiters like `;;`, `&&`, `;&` and `&;` will behave like there is an empty command in between them.

### Pastevents
- All commands are stored in history in a unified format. For example, `sleep 1`, `sleep 1 ;`, `sleep  1 ;;` are identical commands, and hence won't be saved to history consecutively.
- A failed `pastevents execute <idx>` in any subcommand will result in the entire command not being stored to history.

### System Commands
- If any subcommand in a command list takes greater than or equal to 2 seconds to execute, then the maximum time taking subcommand along with its corresponding time taken is printed along with the prompt upon completion.

### Proclore
- All processes having a different `group id` than the shell are as background processes.
- All processes having the same `group id` as the shell but not having control of it are also background processes.
- Processes having same `group id` as the shell and having control of it are foreground processes.

### Seek
- `-e` flag is only valid when only a singular file/directory is found.
- The command willl look for all files/directories that contain the given argument.

### Piping
- Piping from a `pastevents execute <idx>` is supported, but piping into a `pastevents execute <idx>` is not, due to ambiguous behaviour.
- A command can have at most one input file.
- A command can have at most one output file.
- In case a command is receiving input both from a pipe and an input file, the input file will be prioritized over the pipe (as is done in bash).

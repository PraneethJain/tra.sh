[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# trash

A shell written in c with **0 malloc calls**.

# Requirements
- `make`
- `gcc`

# Installation Instructions
1. Clone the repo.
2. `cd` into it.
3. Run `make`. The binary will be built. Run with `./a.out`.
(or)
3. Run `make release`.
4. The binary will be built. 
5. Run with `./release/trash`.

# Directory Structure
- All code is in the `src` directory.
- Each command's implementation is in `src/commands/<command_name>.c`.
- Some utility functions are in `src/utils/<utility>.c`
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

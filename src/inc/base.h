#ifndef __BASE_H
#define __BASE_H

#include "colors.h"
#include <stdio.h>
#include <sys/types.h>
#define MAX_COMMANDS 16
#define MAX_ARGS 16
#define MAX_STR_LEN 1024
#define MAX_PROCESSES 16

#define SUCCESS 0
#define FAILURE 1

typedef struct string
{
  char *str;
  size_t length;
  size_t size;
  bool allocated;
} string;

typedef struct command
{
  int argc;
  char argv[MAX_ARGS][MAX_STR_LEN];
  bool is_background;
} command;

typedef struct commands
{
  command arr[MAX_COMMANDS];
  size_t size;
} commands;

typedef struct processes
{
  command c[MAX_PROCESSES];
  pid_t pid[MAX_PROCESSES];
  size_t length;
} processes;

string new_string(size_t size);
void replace(string *s, string a, string b);

void print_command(command *c);
void print_commands(commands *cs);

bool command_equal(command *c1, command *c2);
bool commands_equal(commands *cs1, commands *cs2);

int max(int x, int y);
int min(int x, int y);
int num_digits(int x);
bool is_numeric(char *s);

#define ERROR_PRINT(fmt, args...) fprintf(stderr, C_RED "[ERROR] " fmt C_RESET, ##args)

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)                                                                                      \
  fprintf(stderr, C_YELLOW "[DEBUG] %s:%d:%s(): " fmt C_RESET, __FILE__, __LINE__, __func__, ##args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

#endif

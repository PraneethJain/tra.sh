#ifndef __COMMAND_H
#define __COMMAND_H

#include "base.h"
#include <stdbool.h>
typedef struct command
{
  int argc;
  char **argv;
  bool is_background;
} command;

void parse_input(string input);
void parse_command(string input);

#endif

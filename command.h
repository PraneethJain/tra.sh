#ifndef __COMMAND_H
#define __COMMAND_H

#include "base.h"
#include <stdbool.h>
typedef struct command
{
  string name;
  int argc;
  string *argv;
} command;

void parse_command(string input);

#endif

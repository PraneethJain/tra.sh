#ifndef __COMMAND_H
#define __COMMAND_H

#include "base.h"
#include <stdbool.h>

void parse_input(string input);
int exec_command(command input);

typedef struct pipeline_struct
{
  command *c;
  struct pipeline_struct *next;
} pipeline_struct;

typedef pipeline_struct *pipeline;

#endif

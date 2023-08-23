#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

#include "base.h"
#define HISTORY_SIZE 15

typedef struct history
{
  commands arr[HISTORY_SIZE];
  size_t cur_size;
} history;

int init_history();
int pastevents(command c);
int add_event(commands cs);
int destroy_history();

#endif

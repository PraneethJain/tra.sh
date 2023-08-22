#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

#include "base.h"
#define HISTORY_SIZE 15

typedef struct history
{
  commands arr[HISTORY_SIZE];
  size_t cur_size;
} history;

void init_history();
void pastevents(command c);
void add_event(commands cs);
void destroy_history();

#endif

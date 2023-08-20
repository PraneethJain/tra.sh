#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

#include "base.h"
#define HISTORY_SIZE 15

typedef struct history
{
  char arr[HISTORY_SIZE][MAX_STR_LEN];
  size_t cur_size;
} history;

void pastevents(command c);
void add_event(string s, commands cs);

#endif

#ifndef __PROCS_H
#define __PROCS_H

#include "base.h"


int insert_process(command c, pid_t pid);
void print_processes();
int remove_zombie_processes();

#endif

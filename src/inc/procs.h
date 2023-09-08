#ifndef __PROCS_H
#define __PROCS_H

#include "base.h"


process_list init_process_list();
int insert_process(command c, pid_t pid);
void print_process_list();
int remove_processes();
void free_process_list();

#endif

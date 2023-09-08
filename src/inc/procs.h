#ifndef __PROCS_H
#define __PROCS_H

#include "base.h"
#include <sys/types.h>
typedef struct process_list_st
{
  command c;
  pid_t pid;
  struct process_list_st *next;
} process_list_st;

typedef process_list_st *process_list;

process_list init_process_list();
int insert_process(command c, pid_t pid);
void print_process_list();
int remove_processes();
void free_process_list();

#endif

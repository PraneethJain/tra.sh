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
void insert_process(process_list p, command c, pid_t pid);
void remove_processes(process_list p);

#endif
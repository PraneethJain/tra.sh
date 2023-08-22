#include "../headers.h"

process_list init_process_list()
{
  process_list p = (process_list)malloc(sizeof(process_list_st));
  // Dummy node
  p->pid = -1;
  p->next = NULL;

  return p;
}

void insert_process(process_list p, command c, pid_t pid)
{
  process_list new = (process_list)malloc(sizeof(process_list_st));
  new->c = c;
  new->pid = pid;
  new->next = p->next;
  p->next = new;
}

void remove_process(process_list p, pid_t pid)
{
  process_list cur = p->next;
  process_list prev = p;

  while (cur != NULL)
  {
    if (cur->pid == pid)
    {
      prev->next = cur->next;
      return;
    }
    cur = cur->next;
  }
}

void remove_processes(process_list p)
{
  process_list cur = p->next;
  while (cur != NULL)
  {
    int status;
    waitpid(cur->pid, &status, WNOHANG | WUNTRACED);
    if (WIFEXITED(status) || WIFSTOPPED(status))
    {
      printf("%s exited normally (%i)\n", cur->c.argv[0], cur->pid);
      remove_process(p, cur->pid);
    }

    cur = cur->next;
  }
  printf("\n");
}

void free_process_list(process_list p)
{
  process_list cur = p->next;
  while (cur != NULL)
  {
    process_list temp = cur->next;
    free(cur);
    cur = temp;
  }
  free(p);
}

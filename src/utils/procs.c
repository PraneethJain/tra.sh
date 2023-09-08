#include "../headers.h"

process_list init_process_list()
{
  process_list p = (process_list)malloc(sizeof(process_list_st));
  if (p == NULL)
  {
    DEBUG_PRINT("malloc failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Ran out of memory!");
  }
  // Dummy node
  p->pid = -1;
  p->next = NULL;

  return p;
}

void print_process_list()
{
  process_list cur = p->next;
  while (cur != NULL)
  {
    printf("%i : ", cur->pid);
    print_command(&cur->c);
    printf(" - ");
    int status;
    if (waitpid(cur->pid, &status, WNOHANG) == 0)
    {
      printf("Running");
    }
    else
    {
      printf("Stopped");
    }
    printf("\n");
    cur = cur->next;
  }
}

int insert_process(command c, pid_t pid)
{
  process_list new = (process_list)malloc(sizeof(process_list_st));
  if (new == NULL)
  {
    DEBUG_PRINT("malloc failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Ran out of memory!");
    return FAILURE;
  }
  new->c = c;
  new->pid = pid;
  new->next = p->next;
  p->next = new;

  return SUCCESS;
}

int remove_process(pid_t pid)
{
  process_list cur = p->next;
  process_list prev = p;

  while (cur != NULL)
  {
    process_list next = cur->next;
    if (cur->pid == pid)
    {
      printf("%s exited normally (%i)\n", cur->c.argv[0], cur->pid);
      free(cur);
      prev->next = next;
      return SUCCESS;
    }
    prev = prev->next;
    cur = next;
  }

  return FAILURE;
}

int remove_processes()
{
  bool done = false;
  while (!done)
  {
    int status;
    done = true;
    for (process_list cur = p->next; cur != NULL; cur = cur->next)
    {
      if (waitpid(cur->pid, &status, WNOHANG | WUNTRACED) != 0)
      {
        if (remove_process(cur->pid) == FAILURE)
          return FAILURE;
        else
        {
          done = false;
          break;
        }
      }
    }
  }

  return SUCCESS;
}

void free_process_list()
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

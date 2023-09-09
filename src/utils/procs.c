#include "../headers.h"

void print_processes()
{
  for (size_t i = 0; i < state.procs.length; ++i)
  {
    printf("%i : ", state.procs.pid[i]);
    print_command(&state.procs.c[i]);
    printf(" - ");
    int status;
    if (waitpid(state.procs.pid[i], &status, WNOHANG) == 0)
    {
      printf("Running");
    }
    else
    {
      printf("Stopped");
    }
    printf("\n");
  }
}

int insert_process(command c, pid_t pid)
{
  bool inserted = false;
  for (size_t i = 0; i < state.procs.length; ++i)
  {
    if (state.procs.pid[i] > pid)
    {
      for (size_t j = state.procs.length; j > i; --j)
      {
        state.procs.c[j] = state.procs.c[j - 1];
        state.procs.pid[j] = state.procs.pid[j - 1];
      }
      state.procs.c[i] = c;
      state.procs.pid[i] = pid;
      inserted = true;
    }
  }

  if (!inserted)
  {
    state.procs.c[state.procs.length] = c;
    state.procs.pid[state.procs.length] = pid;
  }

  ++state.procs.length;
  return SUCCESS;
}

int remove_zombie_processes()
{
  bool done = false;
  while (!done)
  {
    int status;
    done = true;
    for (size_t i = 0; i < state.procs.length; ++i)
    {
      if (waitpid(state.procs.pid[i], &status, WNOHANG | WUNTRACED) != 0)
      {
        if (WIFEXITED(status))
          printf("%s exited normally (%i)\n", state.procs.c[i].argv[0], state.procs.pid[i]);
        else
          printf("%s exited abnormally (%i)\n", state.procs.c[i].argv[0], state.procs.pid[i]);

        for (size_t j = i; j < state.procs.length - 1; ++j)
        {
          state.procs.c[j] = state.procs.c[j + 1];
          state.procs.pid[j] = state.procs.pid[j + 1];
        }
        --state.procs.length;
        done = false;
        break;
      }
    }
  }

  return SUCCESS;
}

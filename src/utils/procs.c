#include "../headers.h"

void print_processes()
{
  for (size_t i = 0; i < state->procs.length; ++i)
  {
    printf("%i : ", state->procs.pid[i]);
    print_command(&state->procs.c[i]);
    printf(" - ");
    char status = '?';
    char process_path[MAX_STR_LEN] = {0};
    snprintf(process_path, MAX_STR_LEN, "/proc/%i/stat", state->procs.pid[i]);
    FILE *process_file = fopen(process_path, "r");
    if (process_file != NULL)
    {
      char buf[MAX_STR_LEN] = {0};
      fscanf(process_file, "%s %[^)]%c %c", buf, buf, &buf[0], &status);
    }
    fclose(process_file);

    if (status == 'Z' || status == 'T')
      printf("Stopped");
    else
      printf("Running");
    printf("\n");
  }
}

int insert_process(command c, pid_t pid)
{
  bool inserted = false;
  for (size_t i = 0; i < state->procs.length; ++i)
  {
    if (state->procs.pid[i] > pid)
    {
      for (size_t j = state->procs.length; j > i; --j)
      {
        state->procs.c[j] = state->procs.c[j - 1];
        state->procs.pid[j] = state->procs.pid[j - 1];
      }
      state->procs.c[i] = c;
      state->procs.pid[i] = pid;
      inserted = true;
    }
  }

  if (!inserted)
  {
    state->procs.c[state->procs.length] = c;
    state->procs.pid[state->procs.length] = pid;
  }

  ++state->procs.length;
  return SUCCESS;
}

int remove_zombie_processes()
{
  bool done = false;
  while (!done)
  {
    int status;
    done = true;
    for (size_t i = 0; i < state->procs.length; ++i)
    {
      if (waitpid(state->procs.pid[i], &status, WNOHANG | WUNTRACED) != 0)
      {
        if (WIFEXITED(status))
          printf("%s exited normally (%i)\n", state->procs.c[i].argv[0], state->procs.pid[i]);
        else
          printf("%s exited abnormally (%i)\n", state->procs.c[i].argv[0], state->procs.pid[i]);

        for (size_t j = i; j < state->procs.length - 1; ++j)
        {
          state->procs.c[j] = state->procs.c[j + 1];
          state->procs.pid[j] = state->procs.pid[j + 1];
        }
        --state->procs.length;
        done = false;
        break;
      }
    }
  }

  return SUCCESS;
}

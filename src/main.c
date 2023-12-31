#include "headers.h"

trash *state;

int init()
{

  state = mmap(NULL, sizeof(trash), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  if (init_signals() == FAILURE)
    return FAILURE;

  state->input[0] = '\0';
  state->lastpath[0] = '\0';
  strcpy(state->delimiters, " \t\n\v\f\r");
  state->procs.length = 0;
  state->max_time_taken = 0;
  state->child_running_in_fg = false;
  state->EXIT = false;
  if (getcwd(state->homepath, MAX_STR_LEN) == NULL)
  {
    DEBUG_PRINT("getcwd failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Could not get current directory\n");
    return FAILURE;
  }

  if (init_history() == FAILURE)
    return FAILURE;

  DEBUG_PRINT("Initialization Complete\n");

  return SUCCESS;
}

void destroy()
{
  munmap(state, sizeof(trash));
  DEBUG_PRINT("Destruction Complete\n");
}

int main()
{
  if (init() == FAILURE)
    return FAILURE;
  while (!state->EXIT)
  {
    remove_zombie_processes();
    prompt();
    if (read_line_raw() == 2)
      break;
    state->child_running_in_fg = true;
    parse_input();
    state->child_running_in_fg = false;
  }
  destroy();

  return SUCCESS;
}

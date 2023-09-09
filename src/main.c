#include "headers.h"

trash state;

int init()
{
  if (init_signals() == FAILURE)
    return FAILURE;
  state.EXIT = false;
  if (getcwd(state.homepath, MAX_STR_LEN) == NULL)
  {
    DEBUG_PRINT("getcwd failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Could not get current directory\n");
    return FAILURE;
  }

  state.input[0] = '\0';
  state.lastpath[0] = '\0';
  strcpy(state.tilde, "~\0");
  strcpy(state.delimiters, " \t\n\v\f\r");
  state.procs.length = 0;
  state.max_time_taken = 0;

  if (init_history() == FAILURE)
    return FAILURE;
  if (init_prompt() == FAILURE)
    return FAILURE;

  DEBUG_PRINT("Initialization Complete\n");

  return SUCCESS;
}

void destroy()
{
  destroy_prompt();
  destroy_history();
  DEBUG_PRINT("Destruction Complete\n");
}

int main()
{
  if (init() == FAILURE)
    return FAILURE;
  while (!state.EXIT)
  {
    remove_zombie_processes();
    prompt();
    read_line_raw();
    parse_input();
  }
  destroy();

  return SUCCESS;
}

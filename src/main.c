#include "headers.h"

bool EXIT;
string homepath;
string lastpath;
string tilde;
string delimiters;
process_list p;
history h;
time_t max_time_taken;
command slowest_command;

int init()
{
  EXIT = false;
  homepath = new_string(MAX_STR_LEN);
  if (!homepath.allocated)
    return FAILURE;

  if (getcwd(homepath.str, homepath.size) == NULL)
  {
    DEBUG_PRINT("getcwd failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Could not get current directory\n");
    return FAILURE;
  }

  lastpath = new_string(MAX_STR_LEN);
  if (!lastpath.allocated)
    return FAILURE;

  tilde = new_string(2);
  if (!lastpath.allocated)
    return FAILURE;
  strcpy(tilde.str, "~\0");

  delimiters = new_string(128);
  if (!delimiters.allocated)
    return FAILURE;
  strcpy(delimiters.str, " \t\n\v\f\r");
  p = init_process_list();
  if (p == NULL)
    return FAILURE;
  max_time_taken = 0;

  if (init_history() == FAILURE)
    return FAILURE;
  if (init_prompt() == FAILURE)
    return FAILURE;

  DEBUG_PRINT("Initialization Complete\n");

  return SUCCESS;
}

void destroy()
{
  free(homepath.str);
  free(lastpath.str);
  free(tilde.str);
  free(delimiters.str);
  free_process_list(p);

  destroy_prompt();
  destroy_history();
  DEBUG_PRINT("Destruction Complete\n");
}

int main()
{
  if (init() == FAILURE)
    return FAILURE;
  while (!EXIT)
  {
    remove_processes(p);
    prompt();
    string input = new_string(MAX_STR_LEN);
    fgets(input.str, input.size, stdin);
    parse_input(input);
    free(input.str);
  }
  destroy();

  return SUCCESS;
}

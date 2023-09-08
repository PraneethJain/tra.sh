#include "../headers.h"

int system_command(command c)
{
  char *argv[c.argc + 1];
  for (int i = 0; i < c.argc; ++i)
    argv[i] = c.argv[i];
  argv[c.argc] = NULL;

  if (execvp(c.argv[0], argv) == -1)
  {
    DEBUG_PRINT("execvp failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Couldn't execute %s\n", c.argv[0]);
    exit(1);
  }

  return SUCCESS;
}

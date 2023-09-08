#include "../headers.h"

int bg(command c)
{
  if (c.argc != 2)
  {
    ERROR_PRINT("bg expected 1 argument, found %i\n", c.argc - 1);
    return FAILURE;
  }

  if (!is_numeric(c.argv[1]))
  {
    ERROR_PRINT("bg found non integer PID %s\n", c.argv[1]);
    return FAILURE;
  }

  char *temp;
  pid_t pid = strtoll(c.argv[1], &temp, 10);
  if (getpgid(pid) < 0) // Checks if process id is invalid
  {
    ERROR_PRINT("No process with pid %i found\n", pid);
    return FAILURE;
  }

  return SUCCESS;
}
#include "../headers.h"

int ping(command c)
{
  if (c.argc != 3)
  {
    ERROR_PRINT("ping expects 2 arguments, found %i\n", c.argc - 1);
    return FAILURE;
  }

  if (!is_numeric(c.argv[1]))
  {
    ERROR_PRINT("ping found non integer PID %s\n", c.argv[1]);
    return FAILURE;
  }

  if (!is_numeric(c.argv[2]))
  {
    ERROR_PRINT("ping found non integer signal number %s\n", c.argv[2]);
    return FAILURE;
  }

  char *temp;
  pid_t pid = strtoll(c.argv[1], &temp, 10);
  int sig = strtoll(c.argv[2], &temp, 10);
  sig %= 32;

  if (getpgid(pid) < 0) // Checks if process id is invalid
  {
    ERROR_PRINT("No process with pid %i found\n", pid);
    return FAILURE;
  }

  if (kill(pid, sig) == -1)
  {
    DEBUG_PRINT("kill failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Failed to ping %i with signal %i\n", pid, sig);
    return FAILURE;
  }

  printf("Sent signal %i to process with pid %i\n", sig, pid);

  return SUCCESS;
}
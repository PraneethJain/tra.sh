#include "headers.h"

void system_command(command c)
{
  pid_t pid = fork();
  if (pid == 0)
  {
    if (execvp(c.argv[0], c.argv) == -1)
    {
      printf("Error\n");
    }
  }
  else
  {
    if (c.is_background)
    {
      printf("%i\n", pid);
    }
    else
    {
      int status;
      waitpid(pid, &status, 0);
    }
  }
}

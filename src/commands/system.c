#include "../headers.h"

void system_command(command c)
{
  pid_t pid = fork();
  if (pid == 0)
  {
    char *argv[c.argc + 1];
    for (int i = 0; i < c.argc; ++i)
      argv[i] = c.argv[i];
    argv[c.argc] = NULL;

    if (execvp(c.argv[0], argv) == -1)
    {
      printf("Error\n");
    }
  }
  else
  {
    if (c.is_background)
    {
      printf("%i\n", pid);
      insert_process(p, c, pid);
    }
    else
    {
      int status;
      waitpid(pid, &status, 0);
    }
  }
}

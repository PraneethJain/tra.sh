#include "../headers.h"

int system_command(command c)
{
  pid_t pid = fork();
  if (pid == 0)
  {
    char *argv[c.argc + 1];
    for (int i = 0; i < c.argc; ++i)
      argv[i] = c.argv[i];
    argv[c.argc] = NULL;

    setpgid(0, 0);
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    if (execvp(c.argv[0], argv) == -1)
    {
      DEBUG_PRINT("execvp failed with errno %i (%s)\n", errno, strerror(errno));
      ERROR_PRINT("Couldn't execute %s\n", c.argv[0]);
      exit(1);
    }
  }
  else
  {
    if (c.is_background)
    {
      printf("%i\n", pid);
      if (insert_process(c, pid) == FAILURE)
        return FAILURE;
    }
    else
    {
      setpgid(pid, 0);
      signal(SIGTTIN, SIG_IGN);
      signal(SIGTTOU, SIG_IGN);
      tcsetpgrp(STDIN_FILENO, pid);

      int status;
      state->child_running_in_fg = true;
      waitpid(pid, &status, WUNTRACED);
      state->child_running_in_fg = false;

      tcsetpgrp(STDIN_FILENO, getpgid(0));
      signal(SIGTTIN, SIG_DFL);
      signal(SIGTTOU, SIG_DFL);

      if (WIFSTOPPED(status)) // Ctrl+Z pressed
      {
        insert_process(c, pid);
      }
    }
  }

  return SUCCESS;
}

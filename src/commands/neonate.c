#include "../headers.h"

int time_arg;
bool terminate = true;

int kbhit()
{
  struct timeval tv = {0L, 0L};
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0, &fds);
  return select(1, &fds, NULL, NULL, &tv);
}

void SIGALRM_handler(int sig)
{
  DEBUG_PRINT("Inside SIGALRM_handler with signal %i\n", sig);
  if (terminate)
    return;

  char str[MAX_STR_LEN] = {0};
  pid_t pid = -1;

  FILE *loadavg_file = fopen("/proc/loadavg", "r");
  fscanf(loadavg_file, "%s %s %s %s %i", str, str, str, str, &pid);
  fclose(loadavg_file);

  printf("%i\n", pid);
  alarm(time_arg);
}

int neonate(command c)
{
  if (c.argc != 3)
  {
    ERROR_PRINT("Neonate expects 2 arguments, found %i\n", c.argc - 1);
    return FAILURE;
  }
  if (strcmp(c.argv[1], "-n") != 0)
  {
    ERROR_PRINT("Expected -n after neonate\n");
    return FAILURE;
  }
  if (!is_numeric(c.argv[2]))
  {
    ERROR_PRINT("Neonate found non integer time argument!\n");
    return FAILURE;
  }

  init_signal(SIGALRM, SIGALRM_handler);

  char *temp;
  time_arg = strtoll(c.argv[2], &temp, 10);
  terminate = false;
  alarm(time_arg);

  enable_raw_mode();
  char ch;
  if (time_arg == 0)
    while (!(kbhit() && read(STDIN_FILENO, &ch, 1) == 1 && ch == 'x'))
    {
      char str[MAX_STR_LEN] = {0};
      pid_t pid = -1;
      FILE *loadavg_file = fopen("/proc/loadavg", "r");
      fscanf(loadavg_file, "%s %s %s %s %i", str, str, str, str, &pid);
      fclose(loadavg_file);
      printf("%i\n", pid);
    }
  else
    while (read(STDIN_FILENO, &ch, 1) == 1 && ch != 'x')
      ;
  disable_raw_mode();
  terminate = true;

  return SUCCESS;
}

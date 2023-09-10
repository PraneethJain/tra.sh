#include "../headers.h"

int init_signals()
{
  if (init_signal(SIGINT, SIGINT_handler) == FAILURE)
    return FAILURE;
  if (init_signal(SIGTSTP, SIGTSTP_handler) == FAILURE)
    return FAILURE;
  return SUCCESS;
}

int init_signal(int sig, void (*func)(int))
{
  struct sigaction sa;
  sa.sa_handler = func;
  sa.sa_flags = SA_RESTART;
  if (sigaction(sig, &sa, NULL) == -1)
  {
    DEBUG_PRINT("signal failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Failed to initialize SIGINT handler\n");
    return FAILURE;
  }

  return SUCCESS;
}

void SIGINT_handler(int sig)
{
  DEBUG_PRINT("\nInside SIGINT_handler with sig %i\n", sig);
  write(STDOUT_FILENO, "\n", 1);
  if (!state->child_running_in_fg)
  {
    state->input_length = 0;
    state->input[0] = '\0';
    prompt();
  }
}

void SIGTSTP_handler(int sig)
{
  DEBUG_PRINT("\nInside SIGTSTP_handler with sig %i\n", sig);
}

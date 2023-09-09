#include "../headers.h"

int init_signals()
{
  struct sigaction sa;
  sa.sa_handler = &SIGINT_handler;
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &sa, NULL) == -1)
  {
    DEBUG_PRINT("signal failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Failed to initialize SIGINT handler\n");
    return FAILURE;
  }

  return SUCCESS;
}

void SIGINT_handler(int sig)
{
  sig += 1;
  DEBUG_PRINT("\nInside SIGINT_handler with sig %i\n", sig);
}

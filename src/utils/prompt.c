#include "../headers.h"

char username[MAX_STR_LEN] = {0};
char hostname[MAX_STR_LEN] = {0};
char cwd[MAX_STR_LEN] = {0};

int prompt()
{
  if (getlogin_r(username, MAX_STR_LEN) != 0)
  {
    DEBUG_PRINT("getlogin_r failed with errno %i (%s)", errno, strerror(errno));
    ERROR_PRINT("Could not get username\n");
    return FAILURE;
  }

  if (gethostname(hostname, MAX_STR_LEN) != 0)
  {
    DEBUG_PRINT("gethostname failed with errno %i (%s)", errno, strerror(errno));
    ERROR_PRINT("Could not get hostname\n");
    return FAILURE;
  }

  if (getcwd(cwd, MAX_STR_LEN) == NULL)
  {
    DEBUG_PRINT("getcwd failed with errno %i (%s)", errno, strerror(errno));
    ERROR_PRINT("Could not get current directory\n");
    return FAILURE;
  }

  homepath_to_tilde(cwd);

  if (state->max_time_taken >= 2)
  {
    printf("<" C_GREEN "%s" C_RESET "@" C_BLUE "%s" C_RESET ":" C_YELLOW "%s" C_RESET " %s : %lis"
           "> ",
           username, hostname, cwd, state->slowest_command.argv[0], state->max_time_taken);
  }
  else
  {
    printf("<" C_GREEN "%s" C_RESET "@" C_BLUE "%s" C_RESET ":" C_YELLOW "%s" C_RESET "> ", username, hostname, cwd);
  }

  return SUCCESS;
}

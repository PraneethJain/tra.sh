#include "../headers.h"

char username[MAX_STR_LEN];
char hostname[MAX_STR_LEN];
char cwd[MAX_STR_LEN];


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

  if (strstr(cwd, state->homepath) == cwd) // If cwd starts with homepath then replace it with tilde
  {
    size_t cwd_len = strlen(cwd);
    size_t homepath_len = strlen(state->homepath);
    for (size_t i = 1; i <= cwd_len - homepath_len + 1; ++i)
      cwd[i] = cwd[i + homepath_len - 1];
    cwd[0] = '~';
  }

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

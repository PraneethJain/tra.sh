#include "../headers.h"

string username;
string hostname;
string cwd;

int init_prompt()
{
  username = new_string(MAX_STR_LEN);
  if (!username.allocated)
    return FAILURE;
  hostname = new_string(MAX_STR_LEN);
  if (!hostname.allocated)
    return FAILURE;
  cwd = new_string(MAX_STR_LEN);
  if (!cwd.allocated)
    return FAILURE;

  return SUCCESS;
}

void destroy_prompt()
{
  free(username.str);
  free(hostname.str);
  free(cwd.str);
}

int prompt()
{
  if (getlogin_r(username.str, username.size) != 0)
  {
    DEBUG_PRINT("getlogin_r failed with errno %i (%s)", errno, strerror(errno));
    ERROR_PRINT("Could not get username\n");
    return FAILURE;
  }

  if (gethostname(hostname.str, hostname.size) != 0)
  {
    DEBUG_PRINT("gethostname failed with errno %i (%s)", errno, strerror(errno));
    ERROR_PRINT("Could not get hostname\n");
    return FAILURE;
  }

  if (getcwd(cwd.str, cwd.size) == NULL)
  {
    DEBUG_PRINT("getcwd failed with errno %i (%s)", errno, strerror(errno));
    ERROR_PRINT("Could not get current directory\n");
    return FAILURE;
  }

  if (strstr(cwd.str, homepath.str) == cwd.str) // If cwd starts with homepath
    replace(&cwd, homepath, tilde);

  if (max_time_taken >= 2)
  {
    printf("<" C_GREEN "%s" C_RESET "@" C_BLUE "%s" C_RESET ":" C_YELLOW "%s" C_RESET " %s : %lis"
           "> ",
           username.str, hostname.str, cwd.str, slowest_command.argv[0], max_time_taken);
  }
  else
  {
    printf("<" C_GREEN "%s" C_RESET "@" C_BLUE "%s" C_RESET ":" C_YELLOW "%s" C_RESET "> ", username.str, hostname.str,
           cwd.str);
  }

  return SUCCESS;
}

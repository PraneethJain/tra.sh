#include "../headers.h"

string username;
string hostname;
string cwd;

void init_prompt()
{
  username = new_string(MAX_STR_LEN);
  hostname = new_string(MAX_STR_LEN);
  cwd = new_string(MAX_STR_LEN);
}

void destroy_prompt()
{
  free(username.str);
  free(hostname.str);
  free(cwd.str);
}

void prompt()
{
  // username = new_string(256);
  if (getlogin_r(username.str, username.size) != 0)
  {
    // Do error handling
  }

  // hostname = new_string(256);
  if (gethostname(hostname.str, hostname.size) != 0)
  {
    // Do error handling
  }

  // cwd = new_string(MAX_STR_LEN);
  if (getcwd(cwd.str, cwd.size) == NULL)
  {
    // Do error handling
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
}

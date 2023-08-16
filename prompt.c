#include "headers.h"

void prompt()
{
  string username = new_string(256);
  if (getlogin_r(username.str, username.size) != 0)
  {
    // Do error handling
  }

  string hostname = new_string(256);
  if (gethostname(hostname.str, hostname.size) != 0)
  {
    // Do error handling
  }

  string cwd = new_string(PATH_MAX);
  if (getcwd(cwd.str, cwd.size) == NULL)
  {
    // Do error handling
  }

  // Replace /home/<username> with ~
  string homepath = new_string(512);
  sprintf(homepath.str, "/home/%s", username.str);
  if (strstr(cwd.str, homepath.str) == cwd.str) // If cwd starts with homepath
  {
    string tilde = new_string(2);
    tilde.str = "~\0";
    replace(&cwd, homepath, tilde);
  }

  printf("<%s@%s:%s> ", username.str, hostname.str, cwd.str);
}

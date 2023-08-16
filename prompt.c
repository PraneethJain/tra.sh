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
  printf("<%s@%s> ", username.str, hostname.str);
}

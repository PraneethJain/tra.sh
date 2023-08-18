#include "headers.h"

void warp(command c)
{
  string cwd = new_string(PATH_MAX);

  if (c.argc == 1)
  {
    chdir(homepath.str);
    if (getcwd(cwd.str, cwd.size) == NULL)
    {
      // Do error handling
    }
    printf("%s\n", cwd.str);
  }
  else
  {
    for (int i = 1; i < c.argc; ++i)
    {
      if (strcmp(c.argv[i], "~") == 0)
      {
        chdir(homepath.str);
      }
      else if (chdir(c.argv[i]) != 0)
      {
        printf("Cannot warp to %s\n", c.argv[i]);
        break;
      }
      if (getcwd(cwd.str, cwd.size) == NULL)
      {
        // Do error handling
      }
      printf("%s\n", cwd.str);
    }
  }
}

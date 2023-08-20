#include "headers.h"

void warp(command c)
{
  string cwd = new_string(PATH_MAX);

  if (c.argc == 1)
  {
    if (getcwd(cwd.str, cwd.size) == NULL)
    {
      // Do error handling
    }
    chdir(homepath.str);
    strcpy(lastpath.str, cwd.str);
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
      if (getcwd(cwd.str, cwd.size) == NULL)
      {
        // Do error handling
      }

      if (strcmp(c.argv[i], "~") == 0)
      {
        strcpy(lastpath.str, cwd.str);
        chdir(homepath.str);
      }
      else if (strcmp(c.argv[i], "-") == 0)
      {
        if (lastpath.str[0] != '\0')
        {
          chdir(lastpath.str);
          strcpy(lastpath.str, cwd.str);
        }
        else
        {
          printf("OLDPWD not set\n");
          break;
        }
      }
      else if (chdir(c.argv[i]) != 0)
      {
        printf("Cannot warp to %s\n", c.argv[i]);
        break;
      }
      else
      {
        strcpy(lastpath.str, cwd.str);
      }

      if (getcwd(cwd.str, cwd.size) == NULL)
      {
        // Do error handling
      }
      printf("%s\n", cwd.str);
    }
  }
}
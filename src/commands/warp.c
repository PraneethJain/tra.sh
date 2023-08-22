#include "../headers.h"

void warp(command c)
{
  string cwd = new_string(MAX_STR_LEN);

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

      if (c.argv[i][0] == '~')
      {
        strcpy(lastpath.str, cwd.str);
        string abs_path = new_string(MAX_STR_LEN);
        strcpy(abs_path.str, homepath.str);
        strcat(abs_path.str, c.argv[i] + 1);
        if (chdir(abs_path.str) != 0)
        {
          printf("Cannot warp to %s\n", c.argv[i]);
          free(abs_path.str);
          break;
        }
        free(abs_path.str);
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
  free(cwd.str);
}

#include "../headers.h"

int warp(command c)
{
  string cwd = new_string(MAX_STR_LEN);

  if (c.argc == 1)
  {
    if (getcwd(cwd.str, cwd.size) == NULL)
    {
      DEBUG_PRINT("getcwd failed with errno %i (%s)\n", errno, strerror(errno));
      ERROR_PRINT("Could not get current directory\n");
      return FAILURE;
    }
    chdir(state->homepath);
    strcpy(state->lastpath, cwd.str);
    if (getcwd(cwd.str, cwd.size) == NULL)
    {
      DEBUG_PRINT("getcwd failed with errno %i (%s)\n", errno, strerror(errno));
      ERROR_PRINT("Could not get current directory\n");
      return FAILURE;
    }
    printf("%s\n", cwd.str);
  }
  else
  {

    for (int i = 1; i < c.argc; ++i)
    {
      if (getcwd(cwd.str, cwd.size) == NULL)
      {
        DEBUG_PRINT("getcwd failed with errno %i (%s)\n", errno, strerror(errno));
        ERROR_PRINT("Could not get current directory\n");
        return FAILURE;
      }

      if (c.argv[i][0] == '~')
      {
        strcpy(state->lastpath, cwd.str);
        string abs_path = new_string(MAX_STR_LEN);
        strcpy(abs_path.str, state->homepath);
        strcat(abs_path.str, c.argv[i] + 1);
        if (chdir(abs_path.str) != 0)
        {
          ERROR_PRINT("Cannot warp to %s\n", c.argv[i]);
          free(abs_path.str);
          return FAILURE;
        }
        free(abs_path.str);
      }
      else if (strcmp(c.argv[i], "-") == 0)
      {
        if (state->lastpath[0] != '\0')
        {
          if (chdir(state->lastpath) == -1)
          {
            ERROR_PRINT("Cannot warp to %s\n", c.argv[i]);
            return FAILURE;
          }
          strcpy(state->lastpath, cwd.str);
        }
        else
        {
          ERROR_PRINT("OLDPWD not set\n");
          return FAILURE;
        }
      }
      else if (chdir(c.argv[i]) == -1)
      {
        ERROR_PRINT("Cannot warp to %s\n", c.argv[i]);
        return FAILURE;
      }
      else
      {
        strcpy(state->lastpath, cwd.str);
      }

      if (getcwd(cwd.str, cwd.size) == NULL)
      {
        DEBUG_PRINT("getcwd failed with errno %i (%s)\n", errno, strerror(errno));
        ERROR_PRINT("Could not get current directory\n");
        return FAILURE;
      }
      printf("%s\n", cwd.str);
    }
  }
  free(cwd.str);
  return SUCCESS;
}

#include "../headers.h"

#define GETCWD                                                                                                         \
  if (getcwd(cwd, MAX_STR_LEN) == NULL)                                                                                \
  {                                                                                                                    \
    DEBUG_PRINT("getcwd failed with errno %i (%s)\n", errno, strerror(errno));                                         \
    ERROR_PRINT("Could not get current directory\n");                                                                  \
    return FAILURE;                                                                                                    \
  }

#define CHDIR(path)                                                                                                    \
  if (chdir(path) == -1)                                                                                               \
  {                                                                                                                    \
    DEBUG_PRINT("chdir failed with errno %i (%s)\n", errno, strerror(errno));                                          \
    ERROR_PRINT("Cannot warp to %s\n", c.argv[i]);                                                                     \
    return FAILURE;                                                                                                    \
  }                                                                                                                    \
  strcpy(state->lastpath, cwd);

int warp(command c)
{
  char cwd[MAX_STR_LEN];

  if (c.argc == 1)
    strcpy(c.argv[c.argc++], "~");

  for (int i = 1; i < c.argc; ++i)
  {
    fix_tilde(c.argv[i]);
    GETCWD;
    if (strcmp(c.argv[i], "-") == 0)
    {
      if (state->lastpath[0] != '\0')
      {
        CHDIR(state->lastpath);
      }
      else
      {
        ERROR_PRINT("OLDPWD not set\n");
        return FAILURE;
      }
    }
    else
    {
      CHDIR(c.argv[i]);
    }

    GETCWD;
    printf("%s\n", cwd);
  }

  return SUCCESS;
}

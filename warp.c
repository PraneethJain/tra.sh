#include "headers.h"

void warp(command c)
{
  if (c.argc == 1)
  {
    chdir(homepath.str);
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
      }
    }
  }

  // string tok = new_string(4096);
  // string delimiters = new_string(128);
  // strcpy(delimiters.str, " \t\n\v\f\r");
  //
  // bool has_argument = false;
  // while ((tok.str = strtok(NULL, delimiters.str)) != NULL)
  // {
  //   has_argument = true;
  //   if (strcmp(tok.str, "~") == 0)
  //   {
  //     chdir(homepath.str);
  //   }
  //   else if (chdir(tok.str) != 0)
  //   {
  //     printf("Cannot warp to %s\n", tok.str);
  //   }
  // }
  //
  // if (!has_argument)
  //   chdir(homepath.str);
  //
  // printf("\n");
}

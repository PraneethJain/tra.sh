#include "headers.h"

void warp(string input)
{
  string tok = new_string(4096);
  string delimiters = new_string(128);
  strcpy(delimiters.str, " \t\n\v\f\r");

  while ((tok.str = strtok(NULL, delimiters.str)) != NULL)
  {
    if (strcmp(tok.str, "~") == 0)
    {
      chdir(homepath.str);
    }
    else if (chdir(tok.str) != 0)
    {
      printf("Cannot warp to %s\n", tok.str);
    }
  }
  printf("\n");
}

#include "headers.h"

void parse_command(string input)
{
  string tok = new_string(4096);
  string delimiters = new_string(128);
  strcpy(delimiters.str, " \t\n\v\f\r");

  tok.str = strtok(input.str, delimiters.str);
  if (strcmp(tok.str, "warp") == 0)
  {
    warp(input);
  }
  else
  {
    printf("Not a valid command!\n");
  }
}

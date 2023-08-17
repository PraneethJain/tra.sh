#include "headers.h"

void parse_command(string input)
{
  string tok = new_string(4096);
  string delimiters = new_string(128);
  strcpy(delimiters.str, " \t\n\v\f\r");

  string input_copy = new_string(input.size);
  strcpy(input_copy.str, input.str);
  tok.str = strtok(input.str, delimiters.str);
  if (strcmp(tok.str, "warp") == 0)
  {
    warp(input);
  }
  else
  {
    system_command(input_copy);
  }
}

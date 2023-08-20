#include "../headers.h"

void parse_input(string input)
{
  input.str[strcspn(input.str, "\r\n")] = 0;
  string input_copy = new_string(strlen(input.str));
  strcpy(input_copy.str, input.str);
  commands c;
  c.size = 0;

  size_t input_length = strlen(input.str);
  size_t cur_ptr = 0;
  string tok = new_string(MAX_STR_LEN);
  string delimiters = new_string(128);
  strcpy(delimiters.str, " \t\n\v\f\r");
  size_t cur_command_len;
  for (size_t cur_ptr = 0; cur_ptr < input_length; cur_ptr += cur_command_len + 1)
  {
    if ((cur_command_len = strcspn(input.str + cur_ptr, ";&")) > 0)
    {
      c.arr[c.size].is_background = input.str[cur_ptr + cur_command_len] == '&';

      input.str[cur_ptr + cur_command_len] = '\0';
      string current_command = new_string(strlen(input.str + cur_ptr) + 1);
      strcpy(current_command.str, input.str + cur_ptr);

      string arguments[MAX_ARGS];
      int argc = 0;
      while ((tok.str = strtok(argc == 0 ? current_command.str : NULL, delimiters.str)) != NULL)
      {
        arguments[argc] = new_string(strlen(tok.str));
        strcpy(arguments[argc++].str, tok.str);
      }
      c.arr[c.size].argc = argc;
      c.arr[c.size].argv = to_cstring_array(arguments, argc);
      c.size += argc != 0;
    }
  }

  // printf("%i c\n\n", command_count);
  // for (int i = 0; i < command_count; ++i)
  // {
  //   printf("Command name: %s\n", c[i].argv[0]);
  //   for (int j = 1; j < c[i].argc; ++j)
  //   {
  //     printf("arg%i: %s\n", j, c[i].argv[j]);
  //   }
  //   printf("Is background: %i\n", c[i].is_background);
  //   printf("\n");
  // }

  add_event(input_copy, c); // strtok modifies the input string, so a copy is used here.
  for (int i = 0; i < c.size; ++i)
  {
    exec_command(c.arr[i]);
  }
}

void exec_command(command c)
{
  if (strcmp(c.argv[0], "warp") == 0)
  {
    warp(c);
  }
  else if (strcmp(c.argv[0], "peek") == 0)
  {
    peek(c);
  }
  else if (strcmp(c.argv[0], "proclore") == 0)
  {
    proclore(c);
  }
  else if (strcmp(c.argv[0], "pastevents") == 0)
  {
    pastevents(c);
  }
  else
  {
    system_command(c);
  }
}

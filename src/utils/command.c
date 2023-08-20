#include "../headers.h"

void parse_input(string input)
{
  command commands[128];
  int command_count = 0;

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
      commands[command_count].is_background = input.str[cur_ptr + cur_command_len] == '&';

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
      commands[command_count].argc = argc;
      commands[command_count].argv = to_cstring_array(arguments, argc);
      command_count += argc != 0;
    }
  }

  // printf("%i Commands\n\n", command_count);
  // for (int i = 0; i < command_count; ++i)
  // {
  //   printf("Command name: %s\n", commands[i].argv[0]);
  //   for (int j = 1; j < commands[i].argc; ++j)
  //   {
  //     printf("arg%i: %s\n", j, commands[i].argv[j]);
  //   }
  //   printf("Is background: %i\n", commands[i].is_background);
  //   printf("\n");
  // }

  for (int i = 0; i < command_count; ++i)
  {
    exec_command(commands[i]);
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

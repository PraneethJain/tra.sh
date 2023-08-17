#include "headers.h"

void parse_input(string input)
{
  command commands[128];
  int command_count = 0;

  string delimiters = new_string(128);
  strcpy(delimiters.str, " \t\n\v\f\r");
  string tok = new_string(MAX_STR_LEN);

  size_t offset = 0;
  while (1)
  {
    bool to_exit = false;
    char *next = NULL;
    bool is_background;
    for (next = input.str + offset; *next != '\0'; ++next)
    {
      if (*next == '&')
      {
        is_background = true;
        break;
      }

      if (*next == ';')
      {
        is_background = false;
        break;
      }
    }

    if (*next == '\0')
      to_exit = true;

    size_t sz = (next - (input.str + offset));
    string current_command = new_string(sz + 1);
    strncpy(current_command.str, input.str + offset, sz);
    current_command.str[sz] = '\0';
    offset = next - input.str + 1;

    string arguments[MAX_ARGS];
    int argc = 0;
    tok.str = strtok(current_command.str, delimiters.str);
    if (tok.str == NULL)
      break;
    arguments[argc] = new_string(strlen(tok.str));
    strcpy(arguments[argc++].str, tok.str);
    while ((tok.str = strtok(NULL, delimiters.str)) != NULL)
    {
      arguments[argc] = new_string(strlen(tok.str));
      strcpy(arguments[argc++].str, tok.str);
    }
    commands[command_count].argc = argc;
    commands[command_count].argv = to_cstring_array(arguments, argc);
    commands[command_count].is_background = is_background;
    ++command_count;

    if (to_exit)
      break;
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
}

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

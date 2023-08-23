#include "../headers.h"

bool commandify(command *c, string *current_command)
{
  c->argc = 0;
  for (char *tok = strtok(current_command->str, delimiters.str); tok != NULL; tok = strtok(NULL, delimiters.str))
    strcpy(c->argv[c->argc++], tok);

  return c->argc != 0;
}

void parse_input(string input)
{
  input.str[strcspn(input.str, "\r\n")] = '\0';
  commands c;
  c.size = 0;

  size_t input_length = strlen(input.str);
  size_t cur_command_len;
  for (size_t cur_ptr = 0; cur_ptr < input_length; cur_ptr += cur_command_len + 1)
  {
    if ((cur_command_len = strcspn(input.str + cur_ptr, ";&")) > 0)
    {
      c.arr[c.size].is_background = input.str[cur_ptr + cur_command_len] == '&';

      input.str[cur_ptr + cur_command_len] = '\0';
      string current_command = new_string(strlen(input.str + cur_ptr) + 1);
      strcpy(current_command.str, input.str + cur_ptr);

      // If command is valid then move to next index, otherwise stay at same index.
      c.size += commandify(&c.arr[c.size], &current_command);
      free(current_command.str);
    }
  }

  max_time_taken = 0;
  for (size_t i = 0; i < c.size; ++i)
  {
    time_t t_start = time(NULL);
    exec_command(c.arr[i]);
    time_t t_end = time(NULL);

    if (t_end - t_start > max_time_taken)
    {
      max_time_taken = t_end - t_start;
      slowest_command = c.arr[i];
    }
  }

  // printf("%zu Commands\n\n", c.size);
  // for (int i = 0; i < c.size; ++i)
  // {
  //   printf("Command name: %s\n", c.arr[i].argv[0]);
  //   for (int j = 1; j < c.arr[i].argc; ++j)
  //   {
  //     printf("arg%i: %s\n", j, c.arr[i].argv[j]);
  //   }
  //   printf("Is background: %i\n", c.arr[i].is_background);
  //   printf("\n");
  // }
  //
  add_event(c);
}

int exec_command(command c)
{
  int status;
  if (strcmp(c.argv[0], "exit") == 0)
  {
    EXIT = true;
    status = SUCCESS;
  }
  else if (strcmp(c.argv[0], "warp") == 0)
  {
    status = warp(c);
  }
  else if (strcmp(c.argv[0], "peek") == 0)
  {
    status = peek(c);
  }
  else if (strcmp(c.argv[0], "proclore") == 0)
  {
    status = proclore(c);
  }
  else if (strcmp(c.argv[0], "pastevents") == 0)
  {
    status = pastevents(c);
  }
  else if (strcmp(c.argv[0], "seek") == 0)
  {
    status = seek(c);
  }
  else
  {
    status = system_command(c);
  }

  return status;
}

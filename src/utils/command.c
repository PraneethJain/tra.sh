#include "../headers.h"

bool commandify(command *c, string *current_command)
{
  c->argc = 0;
  for (char *tok = strtok(current_command->str, delimiters.str); tok != NULL; tok = strtok(NULL, delimiters.str))
    strcpy(c->argv[c->argc++], tok);

  return c->argc != 0;
}

void sanitize(string s)
{
  if (s.str[0] == '\0')
    return;

  bool fixed = false;
  while (!fixed)
  {
    size_t len = strlen(s.str);
    fixed = true;
    for (size_t i = 1; i < len - 1; ++i)
    {
      if (s.str[i] == '|' && !(s.str[i - 1] == ' ' && s.str[i + 1] == ' '))
      {
        fixed = false;
        for (size_t j = len + 2; j > i; --j)
          s.str[j] = s.str[j - 2];
        s.str[i] = ' ';
        s.str[i + 1] = '|';
        s.str[i + 2] = ' ';
        break;
      }
    }
  }
}

void parse_input(string input)
{
  input.str[strcspn(input.str, "\r\n")] = '\0';
  sanitize(input);
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
  DEBUG_PRINT("Number of commands: %zu\n", c.size);
  for (size_t i = 0; i < c.size; ++i)
  {
    time_t t_start = time(NULL);
    DEBUG_PRINT("%zu. %s with %i arguments\n", i + 1, c.arr[i].argv[0], c.arr[i].argc);
    exec_command(c.arr[i]);
    time_t t_end = time(NULL);
    time_t time_taken = t_end - t_start;
    DEBUG_PRINT("Completed in %li seconds\n", time_taken);

    if (time_taken > max_time_taken)
    {
      max_time_taken = time_taken;
      slowest_command = c.arr[i];
    }
  }

  add_event(c);
}

int exec_singular(command c)
{
  int status;
  if (strcmp(c.argv[0], "exit") == 0)
  {
    *EXIT = true;
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

int exec_command(command c)
{
  commands subcommands;
  int num_subcommands = 0;
  subcommands.arr[num_subcommands].argc = 0;
  for (int i = 0; i < c.argc; ++i)
    if (strcmp(c.argv[i], "|") == 0)
      subcommands.arr[++num_subcommands].argc = 0;
    else
      strcpy(subcommands.arr[num_subcommands].argv[subcommands.arr[num_subcommands].argc++], c.argv[i]);
  ++num_subcommands;

  int saved_stdin = dup(STDIN_FILENO);
  int saved_stdout = dup(STDOUT_FILENO);

  int fd[2];
  int prev_pipe = STDIN_FILENO;
  for (int i = 0; i < num_subcommands - 1; ++i)
  {
    if (pipe(fd) == -1)
    {
      ERROR_PRINT("Failed to create pipe!\n");
      return FAILURE;
    } 
    pid_t pid = fork();
    if (pid == 0)
    {
      if (prev_pipe != STDIN_FILENO)
      {
        dup2(prev_pipe, STDIN_FILENO);
        close(prev_pipe);
      }
      dup2(fd[1], STDOUT_FILENO);
      close(fd[1]);

      exec_singular(subcommands.arr[i]);
      exit(1);
    }
    else
    {
      close(prev_pipe);
      close(fd[1]);
      prev_pipe = fd[0];
    }
  }


  pid_t pid = fork();
  if (pid == 0)
  {
    if (prev_pipe != STDIN_FILENO)
    {
      dup2(prev_pipe, STDIN_FILENO);
      close(prev_pipe);
    }
    exec_singular(subcommands.arr[num_subcommands - 1]);
    exit(1);
  }
  else
  {
    if (c.is_background)
    {
      printf("%i\n", pid);
      if (insert_process(p, subcommands.arr[num_subcommands - 1], pid) == FAILURE)
        return FAILURE;
    }
    else
    {
      int status;
      waitpid(pid, &status, 0);
    }
  }

  dup2(saved_stdout, STDOUT_FILENO);
  dup2(saved_stdin, STDIN_FILENO);

  return SUCCESS;
}

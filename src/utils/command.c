#include "../headers.h"

bool commandify(command *c, char *current_command)
{
  c->argc = 0;
  for (char *tok = strtok(current_command, state->delimiters); tok != NULL; tok = strtok(NULL, state->delimiters))
    strcpy(c->argv[c->argc++], tok);
  return c->argc != 0;
}

void sanitize()
{
  if (state->input[0] == '\0')
    return;

  bool fixed = false;
  while (!fixed)
  {
    fixed = true;
    for (size_t i = 1; i < state->input_length - 1; ++i)
    {
      if (state->input[i] == '|' && !(state->input[i - 1] == ' ' && state->input[i + 1] == ' '))
      {
        fixed = false;
        for (size_t j = state->input_length + 2; j > i; --j)
          state->input[j] = state->input[j - 2];
        state->input[i] = ' ';
        state->input[i + 1] = '|';
        state->input[i + 2] = ' ';
        state->input_length = strlen(state->input);
        break;
      }
    }
  }
}

void parse_input()
{
  sanitize();
  bool add_to_history = true;
  commands c;
  c.size = 0;

  size_t cur_command_len;
  for (size_t cur_ptr = 0; cur_ptr < state->input_length; cur_ptr += cur_command_len + 1)
  {
    if ((cur_command_len = strcspn(state->input + cur_ptr, ";&")) > 0)
    {
      c.arr[c.size].is_background = state->input[cur_ptr + cur_command_len] == '&';

      state->input[cur_ptr + cur_command_len] = '\0';
      char current_command[MAX_STR_LEN] = {0};
      strcpy(current_command, state->input + cur_ptr);
      // If command is valid then move to next index, otherwise stay at same index.
      c.size += commandify(&c.arr[c.size], current_command);
    }
  }

  state->max_time_taken = 0;
  DEBUG_PRINT("Number of commands: %zu\n", c.size);
  for (size_t i = 0; i < c.size; ++i)
  {
    time_t t_start = time(NULL);
    DEBUG_PRINT("%zu. %s with %i arguments\n", i + 1, c.arr[i].argv[0], c.arr[i].argc);
    int res = exec_command(c.arr[i]);
    add_to_history &= res != 2;
    time_t t_end = time(NULL);
    time_t time_taken = t_end - t_start;
    DEBUG_PRINT("Completed in %li seconds\n", time_taken);

    if (time_taken > state->max_time_taken)
    {
      state->max_time_taken = time_taken;
      state->slowest_command = c.arr[i];
    }
  }

  if (add_to_history)
    add_event(c);
}

int exec_singular(command c)
{
  int status;
  if (strcmp(c.argv[0], "exit") == 0)
  {
    state->EXIT = true;
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
  else if (strcmp(c.argv[0], "activities") == 0)
  {
    status = activities(c);
  }
  else if (strcmp(c.argv[0], "ping") == 0)
  {
    status = ping(c);
  }
  else if (strcmp(c.argv[0], "fg") == 0)
  {
    status = fg(c);
  }
  else if (strcmp(c.argv[0], "bg") == 0)
  {
    status = bg(c);
  }
  else if (strcmp(c.argv[0], "neonate") == 0)
  {
    status = neonate(c);
  }
  else if (strcmp(c.argv[0], "iman") == 0)
  {
    status = iman(c);
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
  subcommands.arr[0].infile = -1;
  subcommands.arr[0].outfile = -1;
  subcommands.arr[num_subcommands].argc = 0;
  int i = 0;
  while (i <= c.argc)
  {
    if (i == c.argc || strcmp(c.argv[i], "|") == 0)
    {
      if (subcommands.arr[num_subcommands].argc == 0)
      {
        ERROR_PRINT("Found empty pipe!\n");
        return FAILURE;
      }
      ++num_subcommands;
      subcommands.arr[num_subcommands].argc = 0;
      subcommands.arr[num_subcommands].outfile = -1;
      subcommands.arr[num_subcommands].infile = -1;
      ++i;
    }
    else
    {
      if (strcmp(c.argv[i], "<") == 0)
      {
        if (subcommands.arr[num_subcommands].infile != -1)
        {
          ERROR_PRINT("Multiple inputs found!\n");
          return FAILURE;
        }
        if (i + 1 >= c.argc)
        {
          ERROR_PRINT("No input file provided!\n");
          return FAILURE;
        }
        subcommands.arr[num_subcommands].infile = open(c.argv[i + 1], O_RDONLY);
        if (subcommands.arr[num_subcommands].infile == -1)
        {
          ERROR_PRINT("Failed to read %s\n", c.argv[i + 1]);
          return FAILURE;
        }
        i += 2;
      }
      else if (strcmp(c.argv[i], ">") == 0)
      {
        if (subcommands.arr[num_subcommands].outfile != -1)
        {
          ERROR_PRINT("Multiple outputs found!\n");
          return FAILURE;
        }
        if (i + 1 >= c.argc)
        {
          ERROR_PRINT("No output file provided!\n");
          return FAILURE;
        }
        subcommands.arr[num_subcommands].outfile = open(c.argv[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (subcommands.arr[num_subcommands].outfile == -1)
        {
          ERROR_PRINT("Failed to write %s\n", c.argv[i + 1]);
          return FAILURE;
        }
        i += 2;
      }
      else if (strcmp(c.argv[i], ">>") == 0)
      {
        if (subcommands.arr[num_subcommands].outfile != -1)
        {
          ERROR_PRINT("Multiple outputs found!\n");
          return FAILURE;
        }
        if (i + 1 >= c.argc)
        {
          ERROR_PRINT("No output file provided!\n");
          return FAILURE;
        }
        subcommands.arr[num_subcommands].outfile = open(c.argv[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (subcommands.arr[num_subcommands].outfile == -1)
        {
          ERROR_PRINT("Failed to write %s\n", c.argv[i + 1]);
          return FAILURE;
        }
        i += 2;
      }
      else
      {
        strcpy(subcommands.arr[num_subcommands].argv[subcommands.arr[num_subcommands].argc++], c.argv[i]);
        subcommands.arr[num_subcommands].is_background = c.is_background;
        i += 1;
      }
    }
  }
  int saved_stdin = dup(STDIN_FILENO);
  int saved_stdout = dup(STDOUT_FILENO);

  int fd[2];
  int prev_pipe = STDIN_FILENO;
  for (int i = 0; i < num_subcommands; ++i)
  {
    if (pipe(fd) == -1)
    {
      dup2(saved_stdin, STDIN_FILENO);
      dup2(saved_stdout, STDOUT_FILENO);

      close(saved_stdin);
      close(saved_stdout);
      return FAILURE;
    }

    if (i > 0 && strcmp(subcommands.arr[i].argv[0], "pastevents") == 0)
    {
      dup2(saved_stdin, STDIN_FILENO);
      dup2(saved_stdout, STDOUT_FILENO);

      close(saved_stdin);
      close(saved_stdout);
      ERROR_PRINT("Cannot pipe with pastevents!\n");
      return 2;
    }

    dup2(prev_pipe, STDIN_FILENO);                   // old fd[0]
    if (subcommands.arr[i].infile != -1)             // If infile is present
      dup2(subcommands.arr[i].infile, STDIN_FILENO); // Then set stdin to infile

    if (i == num_subcommands - 1)        // If last command
      dup2(saved_stdout, STDOUT_FILENO); // Then reset stdout
    else
      dup2(fd[1], STDOUT_FILENO); // Otherwise fd[1] is stdout

    if (subcommands.arr[i].outfile != -1) // If outfile is present
    {
      dup2(subcommands.arr[i].outfile, STDOUT_FILENO); // Then set stdout to outfile
      close(subcommands.arr[i].outfile);
    }

    if (exec_singular(subcommands.arr[i]) == FAILURE)
    {
      DEBUG_PRINT("Failed to execute %s\n", subcommands.arr[i].argv[0]);
      dup2(saved_stdin, STDIN_FILENO);
      dup2(saved_stdout, STDOUT_FILENO);

      close(saved_stdin);
      close(saved_stdout);
      return FAILURE;
    }
    close(fd[1]);
    prev_pipe = fd[0]; // fd[0] is output of current, which will be input of next
  }

  dup2(saved_stdin, STDIN_FILENO);
  dup2(saved_stdout, STDOUT_FILENO);

  close(saved_stdin);
  close(saved_stdout);

  return SUCCESS;
}

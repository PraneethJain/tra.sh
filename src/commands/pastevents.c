#include "../headers.h"

char history_path[MAX_STR_LEN];

int init_history()
{
  strcpy(history_path, state->homepath);

  strcat(history_path, "/.trash_history");
  FILE *history_file = fopen(history_path, "rb");
  if (history_file == NULL)
  {
    state->h.cur_size = 0;
  }
  else
  {
    fread(&state->h, sizeof(history), 1, history_file);
    fclose(history_file);
  }
  DEBUG_PRINT("Initialized History\n");

  return SUCCESS;
}

int write_history()
{
  DEBUG_PRINT("Attempting to write history at %s\n", history_path);
  FILE *history_file = fopen(history_path, "wb");
  if (history_file == NULL)
  {
    DEBUG_PRINT("fopen failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Failed to write history");
    return FAILURE;
  }
  fwrite(&state->h, sizeof(history), 1, history_file);
  fclose(history_file);

  return SUCCESS;
}

int pastevents(command c)
{
  if (c.argc == 1)
  {
    for (int i = (int)state->h.cur_size - 1; i >= 0; --i)
    {
      print_commands(&state->h.arr[i]);
    }
  }
  else if (c.argc == 2)
  {
    if (strcmp(c.argv[1], "purge") != 0)
    {
      if (strcmp(c.argv[1], "execute") == 0)
        ERROR_PRINT("Missing index for pastevents execute\n");
      else
        ERROR_PRINT("Invalid subcommand for pastevents: %s\n", c.argv[1]);
      return FAILURE;
    }

    state->h.cur_size = 0;
    write_history();
  }
  else if (c.argc == 3)
  {
    if (!is_numeric(c.argv[2]))
    {
      ERROR_PRINT("Index must be an integer for pastevents execute, found %s\n", c.argv[2]);
      return FAILURE;
    }

    char *temp;
    int idx = strtoll(c.argv[2], &temp, 10) - 1;
    if (idx < 0 || idx > (int)state->h.cur_size - 1)
    {
      ERROR_PRINT("Index %i out of bounds for pastevents execute\n", idx);
      return FAILURE;
    }

    commands replacement = state->h.arr[idx];
    for (size_t i = 0; i < replacement.size; ++i)
    {
      exec_command(replacement.arr[i]);
    }
  }

  return SUCCESS;
}

void shift_one(commands *cs, int start_idx)
{
  for (int i = cs->size - 1; i >= start_idx; --i)
    cs->arr[i + 1] = cs->arr[i];
  ++cs->size;
}

void insert(commands *cs, int h_idx, int cs_idx)
{
  command temp = cs->arr[cs_idx];
  cs->arr[cs_idx] = state->h.arr[h_idx].arr[0];
  cs->arr[cs_idx].is_background = temp.is_background;
  for (int i = 3; i < temp.argc; ++i)
  {
    strcpy(cs->arr[cs_idx].argv[cs->arr[cs_idx].argc++], temp.argv[i]);
  }

  for (size_t i = 1; i < state->h.arr[h_idx].size; ++i)
  {
    shift_one(cs, cs_idx + i);
    cs->arr[cs_idx + i] = state->h.arr[h_idx].arr[i];
  }
}

int add_event(commands cs)
{
  if (cs.size == 0)
    return SUCCESS;

  bool to_add = true;

  for (size_t i = 0; i < cs.size; ++i)
  {
    command c = cs.arr[i];
    if (strcmp(c.argv[0], "pastevents") == 0)
    {
      if (c.argc >= 3 && strcmp(c.argv[1], "execute") == 0 && is_numeric(c.argv[2]))
      {
        char *temp;
        int idx = strtoll(c.argv[2], &temp, 10) - 1;
        if (idx >= 0 && idx <= 14)
        {
          insert(&cs, idx, i);
          i += state->h.arr[idx].size - 1;
          continue;
        }
        else
        {
          to_add = false;
        }
      }
      else
      {
        return SUCCESS;
      }
    }
  }

  if (!to_add)
    return SUCCESS;

  if (state->h.cur_size > 0 && commands_equal(&cs, &state->h.arr[0]))
    return SUCCESS;

  for (int i = min((int)state->h.cur_size - 1, HISTORY_SIZE - 2); i >= 0; --i)
    state->h.arr[i + 1] = state->h.arr[i];
  state->h.cur_size = min(state->h.cur_size + 1, HISTORY_SIZE);
  state->h.arr[0] = cs;

  int status = write_history();
  return status;
}

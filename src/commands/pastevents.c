#include "../headers.h"

string history_path;

int init_history()
{
  history_path = new_string(MAX_STR_LEN);
  if (!history_path.allocated)
    return FAILURE;
  strcpy(history_path.str, homepath.str);

  strcat(history_path.str, "/.trash_history");
  FILE *history_file = fopen(history_path.str, "rb");
  if (history_file == NULL)
  {
    h.cur_size = 0;
  }
  else
  {
    fread(&h, sizeof(history), 1, history_file);
    fclose(history_file);
  }
  DEBUG_PRINT("Initialized History\n");

  return SUCCESS;
}

int write_history()
{
  DEBUG_PRINT("Attempting to write history at %s\n", history_path.str);
  FILE *history_file = fopen(history_path.str, "wb");
  if (history_file == NULL)
  {
    DEBUG_PRINT("fopen failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Failed to write history");
    return FAILURE;
  }
  fwrite(&h, sizeof(history), 1, history_file);
  fclose(history_file);

  return SUCCESS;
}

int pastevents(command c)
{
  if (c.argc == 1)
  {
    for (int i = (int)h.cur_size - 1; i >= 0; --i)
    {
      print_commands(&h.arr[i]);
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

    h.cur_size = 0;
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
    if (idx < 0 || idx > (int)h.cur_size - 1)
    {
      ERROR_PRINT("Index %i out of bounds for pastevents execute\n", idx);
      return FAILURE;
    }

    commands replacement = h.arr[idx];
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
  cs->arr[cs_idx] = h.arr[h_idx].arr[0];
  for (int i = 3; i < temp.argc; ++i)
  {
    strcpy(cs->arr[cs_idx].argv[cs->arr[cs_idx].argc++], temp.argv[i]);
  }

  for (size_t i = 1; i < h.arr[h_idx].size; ++i)
  {
    shift_one(cs, cs_idx + i);
    cs->arr[cs_idx + i] = h.arr[h_idx].arr[i];
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
          i += h.arr[idx].size - 1;
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

  if (h.cur_size > 0 && commands_equal(&cs, &h.arr[0]))
    return SUCCESS;

  for (int i = min((int)h.cur_size - 1, HISTORY_SIZE - 2); i >= 0; --i)
    h.arr[i + 1] = h.arr[i];
  h.cur_size = min(h.cur_size + 1, HISTORY_SIZE);
  h.arr[0] = cs;

  int status = write_history();
  return status;
}

int destroy_history()
{
  int status = write_history();
  free(history_path.str);

  return status;
}

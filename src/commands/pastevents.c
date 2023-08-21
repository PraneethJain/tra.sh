#include "../headers.h"

void write_history()
{
  FILE *history_file = fopen(history_path, "wb");
  if (history_file == NULL)
  {
    // Do error handling
    return;
  }
  fwrite(&h, sizeof(history), 1, history_file);
  fclose(history_file);
}

void pastevents(command c)
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
      // Do error handling
      return;
    }

    h.cur_size = 0;
    write_history();
  }
  else if (c.argc == 3)
  {
    if (!is_numeric(c.argv[2]))
    {
      // Do error handling
      return;
    }

    char *temp;
    int idx = strtoll(c.argv[2], &temp, 10) - 1;
    if (idx < 0 || idx > 14)
    {
      // Do error handling
      return;
    }

    commands replacement = h.arr[idx];
    for (int i = 0; i < replacement.size; ++i)
    {
      exec_command(replacement.arr[i]);
    }
  }
}

void shift_one(commands *cs, int start_idx)
{
  for (int i = cs->size - 1; i >= start_idx; --i)
    cs->arr[i + 1] = cs->arr[i];
  ++cs->size;
}

void insert(commands *cs, int h_idx, int cs_idx)
{
  cs->arr[cs_idx] = h.arr[h_idx].arr[0];
  for (int i = 1; i < h.arr[h_idx].size; ++i)
  {
    shift_one(cs, cs_idx + i);
    cs->arr[cs_idx + i] = h.arr[h_idx].arr[i];
  }
}

void add_event(commands cs)
{
  for (int i = 0; i < cs.size; ++i)
  {
    command c = cs.arr[i];
    if (strcmp(c.argv[0], "pastevents") == 0)
    {
      if (c.argc == 3 && strcmp(c.argv[1], "execute") == 0 && is_numeric(c.argv[2]))
      {
        char *temp;
        int idx = strtoll(c.argv[2], &temp, 10) - 1;
        if (idx >= 0 && idx <= 14)
        {
          insert(&cs, idx, i);
          i += h.arr[idx].size;
          continue;
        }
      }
      else
      {
        // Dont add to pastevents
        return;
      }
    }
  }

  if (h.cur_size > 0 && commands_equal(&cs, &h.arr[0]))
    return;

  for (int i = min((int)h.cur_size - 1, HISTORY_SIZE - 2); i >= 0; --i)
    h.arr[i + 1] = h.arr[i];
  h.cur_size = min(h.cur_size + 1, HISTORY_SIZE);
  h.arr[0] = cs;
  write_history();
}

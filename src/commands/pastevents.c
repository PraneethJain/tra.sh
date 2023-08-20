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
      printf("%s\n", h.arr[i]);
    }
  }
  else if (c.argc == 2)
  {
    if (strcmp(c.argv[1], "purge") == 0)
    {
      h.cur_size = 0;
      write_history();
    }
    else
    {
      // Do error handling
    }
  }
  else if (c.argc == 3)
  {
  }
  else
  {
    // Do error handling
  }
}

bool is_valid(string s, commands cs)
{
  if (h.cur_size > 0 && strcmp(s.str, h.arr[0]) == 0)
    return false;

  for (int i = 0; i < cs.size; ++i)
  {
    command c = cs.arr[i];
    if (strcmp(c.argv[0], "pastevents") == 0)
    {
      if (c.argc == 1)
        return false;
      else if (strcmp(c.argv[1], "purge") == 0)
        return false;
    }
  }

  return true;
}

void add_event(string s, commands cs)
{
  if (!is_valid(s, cs))
    return;

  for (int i = min((int)h.cur_size - 1, HISTORY_SIZE - 2); i >= 0; --i)
  {
    strcpy(h.arr[i + 1], h.arr[i]);
  }
  h.cur_size = min(h.cur_size + 1, HISTORY_SIZE);
  strcpy(h.arr[0], s.str);

  write_history();
}

#include "../headers.h"

bool flag_d;
bool flag_f;
bool flag_e;
string name;

void find(string path)
{
  if (path.str[strlen(path.str) - 1] != '/')
  {
    strcat(path.str, "/");
  }
  DIR *dir = opendir(path.str);
  if (dir == NULL)
  {
    // Do error handling
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    if (entry->d_type == DT_DIR)
    {
      string next_path = new_string(MAX_STR_LEN);
      snprintf(next_path.str, next_path.size, "%s%s", path.str, entry->d_name);
      if ((flag_d || (!flag_d && !flag_f)) && strstr(entry->d_name, name.str) != NULL)
      {
        printf(C_BLUE "%s\n" C_RESET, next_path.str);
      }
      find(next_path);
    }
    else
    {
      string next_name = new_string(MAX_STR_LEN);
      snprintf(next_name.str, next_name.size, "%s%s", path.str, entry->d_name);
      if ((flag_f || (!flag_d && !flag_f)) && strstr(entry->d_name, name.str) != NULL)
      {
        printf(C_GREEN "%s\n" C_RESET, next_name.str);
      }
    }
  }

  if (closedir(dir))
  {
    // Do error handling
  }
}

void seek(command c)
{
  flag_d = false;
  flag_f = false;
  flag_e = false;
  name = new_string(MAX_STR_LEN);
  string path = new_string(MAX_STR_LEN);
  bool found_name = false;
  bool found_path = false;
  for (int i = 1; i < c.argc; ++i)
  {
    if (c.argv[i][0] == '-')
    {
      size_t len = strlen(c.argv[i]);
      for (size_t j = 1; j < len; ++j)
      {
        flag_d |= c.argv[i][j] == 'd';
        flag_f |= c.argv[i][j] == 'f';
        flag_e |= c.argv[i][j] == 'e';
      }
    }
    else if (!found_name)
    {
      strcpy(name.str, c.argv[i]);
      found_name = true;
    }
    else if (!found_path)
    {
      strcpy(path.str, c.argv[i]);
      if (path.str[0] == '~')
      {
        string tilde = new_string(2);
        strcpy(tilde.str, "~\0");
        replace(&path, tilde, homepath);
      }
      found_path = true;
    }
    else
    {
      // Do error handling
      return;
    }
  }

  if (flag_d && flag_f)
  {
    printf("Invalid flags!\n");
    return;
  }

  if (!found_name)
  {
    // Do error handling
    return;
  }

  find(path);
}

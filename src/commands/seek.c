#include "../headers.h"

bool flag_d;
bool flag_f;
bool flag_e;
string name;
string last_found_path;
int count;
size_t path_length;

typedef enum Status
{
  found_dir,
  found_file,
  found_none
} Status;

Status status;

void find(string path)
{
  if (path.str[strlen(path.str) - 1] != '/')
  {
    strcat(path.str, "/");
  }
  DIR *dir = opendir(path.str);
  if (dir == NULL)
  {
    ERROR_PRINT("No directory %s while seeking\n", path.str);
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    if (entry->d_name[0] == '.') // Doubt document q124
      continue;

    if (entry->d_type == DT_DIR)
    {
      string next_path = new_string(MAX_STR_LEN);
      snprintf(next_path.str, next_path.size, "%s%s", path.str, entry->d_name);
      if ((flag_d || (!flag_d && !flag_f)) && strstr(entry->d_name, name.str) != NULL)
      {
        ++count;
        status = found_dir;
        strcpy(last_found_path.str, next_path.str);
        printf(C_BLUE "%s\n" C_RESET, next_path.str + path_length);
      }
      find(next_path);
      free(next_path.str);
    }
    else
    {
      string next_name = new_string(MAX_STR_LEN);
      snprintf(next_name.str, next_name.size, "%s%s", path.str, entry->d_name);
      if ((flag_f || (!flag_d && !flag_f)) && strstr(entry->d_name, name.str) != NULL)
      {
        ++count;
        status = found_file;
        strcpy(last_found_path.str, next_name.str);
        printf(C_GREEN "%s\n" C_RESET, next_name.str + path_length);
      }
      free(next_name.str);
    }
  }

  if (closedir(dir))
  {
    ERROR_PRINT("Couldn't close %s while seeking\n", path.str);
  }
}

int seek(command c)
{
  flag_d = false;
  flag_f = false;
  flag_e = false;
  name = new_string(MAX_STR_LEN);
  last_found_path = new_string(MAX_STR_LEN);
  string path = new_string(MAX_STR_LEN);
  status = found_none;
  count = 0;
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
        replace(&path, state.tilde, state.homepath);
      found_path = true;
    }
    else
    {
      ERROR_PRINT("Invalid arguments for seek\n");
      free(path.str);
      free(last_found_path.str);
      free(name.str);
      return FAILURE;
    }
  }

  if (flag_d && flag_f)
  {
    ERROR_PRINT("Invalid flags!\n");
    free(path.str);
    free(last_found_path.str);
    free(name.str);
    return FAILURE;
  }

  if (!found_name)
  {
    ERROR_PRINT("No file/directory to search for in seek\n");
    free(path.str);
    free(last_found_path.str);
    free(name.str);
    return FAILURE;
  }

  if (!found_path)
  {
    strcpy(path.str, ".");
  }

  path_length = strlen(path.str);
  path_length += path.str[path_length - 1] != '/';
  find(path);

  if (flag_e && count == 1)
  {
    if (status == found_dir)
    {
      if (chdir(last_found_path.str) != 0)
      {
        ERROR_PRINT("Missing permissions for -e in seek\n");
        free(path.str);
        free(last_found_path.str);
        free(name.str);
        return FAILURE;
      }
    }
    else if (status == found_file)
    {
      FILE *f = fopen(last_found_path.str, "r");
      if (f == NULL)
      {
        ERROR_PRINT("Missing permissions for -e in seek\n");
        free(path.str);
        free(last_found_path.str);
        free(name.str);
        return FAILURE;
      }
      else
      {
        char c = 0;
        while ((c = fgetc(f)) != EOF)
          printf("%c", c);
      }
    }
  }
  free(path.str);
  free(last_found_path.str);
  free(name.str);

  return SUCCESS;
}

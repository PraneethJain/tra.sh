#include "../headers.h"

bool flag_d;
bool flag_f;
bool flag_e;
char name[MAX_STR_LEN] = {0};
char last_found_path[MAX_STR_LEN] = {0};
int count;
size_t path_length;

typedef enum Status
{
  found_dir,
  found_file,
  found_none
} Status;

Status status;

void find(char *path)
{
  if (path[strlen(path) - 1] != '/')
    strcat(path, "/");

  DIR *dir = opendir(path);
  if (dir == NULL)
  {
    ERROR_PRINT("No directory %s while seeking\n", path);
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
      char next_path[MAX_STR_LEN];
      snprintf(next_path, MAX_STR_LEN, "%s%s", path, entry->d_name);
      if ((flag_d || (!flag_d && !flag_f)) && strstr(entry->d_name, name) != NULL)
      {
        ++count;
        status = found_dir;
        strcpy(last_found_path, next_path);
        printf(C_BLUE "%s\n" C_RESET, next_path + path_length);
      }
      find(next_path);
    }
    else
    {
      char next_name[MAX_STR_LEN];
      snprintf(next_name, MAX_STR_LEN, "%s%s", path, entry->d_name);
      if ((flag_f || (!flag_d && !flag_f)) && strstr(entry->d_name, name) != NULL)
      {
        ++count;
        status = found_file;
        strcpy(last_found_path, next_name);
        printf(C_GREEN "%s\n" C_RESET, next_name + path_length);
      }
    }
  }

  if (closedir(dir))
    ERROR_PRINT("Couldn't close %s while seeking\n", path);
}

int seek(command c)
{
  flag_d = false;
  flag_f = false;
  flag_e = false;
  char path[MAX_STR_LEN];
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
      strcpy(name, c.argv[i]);
      found_name = true;
    }
    else if (!found_path)
    {
      strcpy(path, c.argv[i]);
      tilde_to_homepath(path);
      found_path = true;
    }
    else
    {
      ERROR_PRINT("Invalid arguments for seek\n");
      return FAILURE;
    }
  }

  if (flag_d && flag_f)
  {
    ERROR_PRINT("Invalid flags!\n");
    return FAILURE;
  }

  if (!found_name)
  {
    ERROR_PRINT("No file/directory to search for in seek\n");
    return FAILURE;
  }

  if (!found_path)
    strcpy(path, ".");

  path_length = strlen(path);
  path_length += path[path_length - 1] != '/';
  find(path);

  if (flag_e && count == 1)
  {
    if (status == found_dir)
    {
      if (chdir(last_found_path) != 0)
      {
        ERROR_PRINT("Missing permissions for -e in seek\n");
        return FAILURE;
      }
    }
    else if (status == found_file)
    {
      FILE *f = fopen(last_found_path, "r");
      if (f == NULL)
      {
        ERROR_PRINT("Missing permissions for -e in seek\n");
        return FAILURE;
      }
      else
      {
        char c = 0;
        while ((c = fgetc(f)) != EOF)
          printf("%c", c);
      }
      fclose(f);
    }
  }

  return SUCCESS;
}

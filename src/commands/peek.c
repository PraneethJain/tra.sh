#include "../headers.h"

int get_block_size_and_max_filesize(struct dirent **entries, int n, string *path, bool flag_a, size_t *filesize)
{
  int res = 0;
  for (int i = 0; i < n; ++i)
  {
    if (path->str[strlen(path->str) - 1] != '/')
      strcat(path->str, "/");

    if (!flag_a && entries[i]->d_name[0] == '.')
      continue;

    string cur = new_string(MAX_STR_LEN);
    if (!cur.allocated)
      return FAILURE;
    strcat(cur.str, path->str);
    strcat(cur.str, entries[i]->d_name);

    struct stat info;
    int status = lstat(cur.str, &info);

    if (status == -1)
    {
      DEBUG_PRINT("lstat failed with errno %i (%s)\n", errno, strerror(errno));
      ERROR_PRINT("Couldn't peek into %s\n", cur.str);
      free(cur.str);
      return FAILURE;
    }
    free(cur.str);

    res += info.st_blocks;

    if (*filesize < (size_t)info.st_size)
      *filesize = info.st_size;
  }

  return res; // ls -l uses 1024 block size on my system, but this result assumes 512 block size according to q117
}

string get_perms(struct stat *info)
{
  string perms = new_string(MAX_STR_LEN);
  if (!perms.allocated)
    return perms;
  // First character in permissions:
  // -    Regular file
  // b    Block special file
  // c    Character special file
  // d    Directory
  // l    Symbolic link
  // p    FIFO
  // s    Socket
  if (S_ISDIR(info->st_mode))
    strcat(perms.str, C_BLUE "d" C_RESET);
  else if (S_ISCHR(info->st_mode))
    strcat(perms.str, "c");
  else if (S_ISBLK(info->st_mode))
    strcat(perms.str, "b");
  else if (S_ISREG(info->st_mode))
    strcat(perms.str, ".");
  else if (S_ISFIFO(info->st_mode))
    strcat(perms.str, "p");
  else if (S_ISLNK(info->st_mode))
    strcat(perms.str, C_CYAN "l" C_RESET);
  else if (S_ISSOCK(info->st_mode))
    strcat(perms.str, "s");
  else
    strcat(perms.str, "?");

  strcat(perms.str, info->st_mode & S_IRUSR ? C_GREEN "r" C_RESET : "-");
  strcat(perms.str, info->st_mode & S_IWUSR ? C_YELLOW "w" C_RESET : "-");
  strcat(perms.str, info->st_mode & S_IXUSR ? C_RED "x" C_RESET : "-");
  strcat(perms.str, info->st_mode & S_IRGRP ? C_GREEN "r" C_RESET : "-");
  strcat(perms.str, info->st_mode & S_IWGRP ? C_YELLOW "w" C_RESET : "-");
  strcat(perms.str, info->st_mode & S_IXGRP ? C_RED "x" C_RESET : "-");
  strcat(perms.str, info->st_mode & S_IROTH ? C_GREEN "r" C_RESET : "-");
  strcat(perms.str, info->st_mode & S_IWOTH ? C_YELLOW "w" C_RESET : "-");
  strcat(perms.str, info->st_mode & S_IXOTH ? C_RED "x" C_RESET : "-");

  return perms;
}

string get_datetime(struct stat *info)
{
  // When last modification is more than 6 months ago, year is shown instead of time
  string datetime = new_string(128);
  if (!datetime.allocated)
    return datetime;
  strftime(datetime.str, datetime.size, time(0) - info->st_mtime < 15768000 ? "%h %d %H:%M" : "%h %d %Y ",
           localtime(&info->st_mtime));

  return datetime;
}

int print_long(char *path, const char *name, int size_width)
{

  string cur = new_string(MAX_STR_LEN);
  if (!cur.allocated)
    return FAILURE;
  strcat(cur.str, path);
  strcat(cur.str, name);

  struct stat info;
  if (lstat(cur.str, &info) == -1)
  {
    DEBUG_PRINT("lstat failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Couldn't peek into %s\n", cur.str);
    free(cur.str);
    return FAILURE;
  }

  string perms = get_perms(&info);
  if (!perms.allocated)
    return FAILURE;
  string datetime = get_datetime(&info);
  if (!datetime.allocated)
    return FAILURE;
  printf("%s ", perms.str);
  printf(" %4li ", info.st_nlink);
  printf("%8s ", getpwuid(info.st_uid)->pw_name);
  printf("%8s ", getgrgid(info.st_gid)->gr_name);
  printf("%*li ", size_width, info.st_size);
  printf("%s ", datetime.str);
  if (S_ISDIR(info.st_mode))
    printf(C_BLUE "%s" C_RESET, name);
  else if (info.st_mode & S_IXUSR)
    printf(C_GREEN "%s" C_RESET, name);
  else
    printf(C_WHITE "%s" C_RESET, name);
  printf("\n");

  free(cur.str);
  free(perms.str);
  free(datetime.str);

  return SUCCESS;
}

int peek(command c)
{
  bool flag_a = false;
  bool flag_l = false;
  bool found_path = false;
  string path = new_string(MAX_STR_LEN);
  if (!path.allocated)
    return FAILURE;

  for (int i = 1; i < c.argc; ++i)
  {
    if (c.argv[i][0] == '-')
    {
      size_t len = strlen(c.argv[i]);
      for (size_t j = 1; j < len; ++j)
      {
        flag_l |= c.argv[i][j] == 'l';
        flag_a |= c.argv[i][j] == 'a';
      }
    }
    else if (found_path)
    {
      ERROR_PRINT("Multiple paths found for peek: %s and %s\n", path.str, c.argv[i]);
      free(path.str);
      return FAILURE;
    }
    else
    {
      found_path = true;
      strcpy(path.str, c.argv[i]);
    }
  }

  if (!found_path)
  {
    strcpy(path.str, ".");
  }

  if (path.str[0] == '~')
    replace(&path, state->tilde, state->homepath);

  struct dirent **entries;
  DEBUG_PRINT("Attempting to scandir %s\n", path.str);
  int n = scandir(path.str, &entries, 0, alphasort);
  if (n < 0)
  {
    ERROR_PRINT("Failed to read %s\n", path.str);
    free(path.str);
    return FAILURE;
  }
  int size_width = 0;
  if (flag_l)
  {
    size_t filesize = 0;
    size_t block_size = get_block_size_and_max_filesize(entries, n, &path, flag_a, &filesize);
    printf("total %zu\n", block_size);
    size_width = num_digits(filesize);
  }
  for (int i = 0; i < n; ++i)
  {
    if (!flag_a && entries[i]->d_name[0] == '.')
      continue;

    if (flag_l)
    {
      if (print_long(path.str, entries[i]->d_name, size_width) == FAILURE)
      {
        free(path.str);
        free(entries);
        return FAILURE;
      }
    }
    else
      printf("%s\n", entries[i]->d_name);

    free(entries[i]);
  }
  free(entries);
  free(path.str);

  return SUCCESS;
}

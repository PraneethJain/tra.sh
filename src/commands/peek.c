#include "../headers.h"

int get_block_size_and_max_filesize(struct dirent **entries, int n, char *path, bool flag_a, size_t *filesize)
{
  int res = 0;
  for (int i = 0; i < n; ++i)
  {
    if (path[strlen(path) - 1] != '/')
      strcat(path, "/");

    if (!flag_a && entries[i]->d_name[0] == '.')
      continue;

    char cur[MAX_STR_LEN];
    strcpy(cur, path);
    strcat(cur, entries[i]->d_name);

    struct stat info;
    int status = lstat(cur, &info);

    if (status == -1)
    {
      DEBUG_PRINT("lstat failed with errno %i (%s)\n", errno, strerror(errno));
      ERROR_PRINT("Couldn't peek into %s\n", cur);
      return FAILURE;
    }

    res += info.st_blocks;

    if (*filesize < (size_t)info.st_size)
      *filesize = info.st_size;
  }

  return res; // ls -l uses 1024 block size on my system, but this result assumes 512 block size according to q117
}

void get_perms(struct stat *info, char *perms)
{
  // First character in permissions:
  // -    Regular file
  // b    Block special file
  // c    Character special file
  // d    Directory
  // l    Symbolic link
  // p    FIFO
  // s    Socket
  perms[0] = '\0';
  if (S_ISDIR(info->st_mode))
    strcat(perms, C_BLUE "d" C_RESET);
  else if (S_ISCHR(info->st_mode))
    strcat(perms, "c");
  else if (S_ISBLK(info->st_mode))
    strcat(perms, "b");
  else if (S_ISREG(info->st_mode))
    strcat(perms, ".");
  else if (S_ISFIFO(info->st_mode))
    strcat(perms, "p");
  else if (S_ISLNK(info->st_mode))
    strcat(perms, C_CYAN "l" C_RESET);
  else if (S_ISSOCK(info->st_mode))
    strcat(perms, "s");
  else
    strcat(perms, "?");

  strcat(perms, info->st_mode & S_IRUSR ? C_GREEN "r" C_RESET : "-");
  strcat(perms, info->st_mode & S_IWUSR ? C_YELLOW "w" C_RESET : "-");
  strcat(perms, info->st_mode & S_IXUSR ? C_RED "x" C_RESET : "-");
  strcat(perms, info->st_mode & S_IRGRP ? C_GREEN "r" C_RESET : "-");
  strcat(perms, info->st_mode & S_IWGRP ? C_YELLOW "w" C_RESET : "-");
  strcat(perms, info->st_mode & S_IXGRP ? C_RED "x" C_RESET : "-");
  strcat(perms, info->st_mode & S_IROTH ? C_GREEN "r" C_RESET : "-");
  strcat(perms, info->st_mode & S_IWOTH ? C_YELLOW "w" C_RESET : "-");
  strcat(perms, info->st_mode & S_IXOTH ? C_RED "x" C_RESET : "-");
}

int print_long(char *path, const char *name, int size_width)
{
  char cur[MAX_STR_LEN];
  strcpy(cur, path);
  strcat(cur, name);

  struct stat info;
  if (lstat(cur, &info) == -1)
  {
    DEBUG_PRINT("lstat failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Couldn't peek into %s\n", cur);
    return FAILURE;
  }

  char perms[128];
  get_perms(&info, perms);
  char datetime[128];
  strftime(datetime, sizeof(datetime), time(0) - info.st_mtime < 15768000 ? "%h %d %H:%M" : "%h %d %Y ",
           localtime(&info.st_mtime));
  printf("%s ", perms);
  printf(" %4li ", info.st_nlink);
  printf("%8s ", getpwuid(info.st_uid)->pw_name);
  printf("%8s ", getgrgid(info.st_gid)->gr_name);
  printf("%*li ", size_width, info.st_size);
  printf("%s ", datetime);
  if (S_ISDIR(info.st_mode))
    printf(C_BLUE "%s" C_RESET, name);
  else if (info.st_mode & S_IXUSR)
    printf(C_GREEN "%s" C_RESET, name);
  else
    printf(C_WHITE "%s" C_RESET, name);
  printf("\n");

  return SUCCESS;
}

int peek(command c)
{
  bool flag_a = false;
  bool flag_l = false;
  bool found_path = false;
  char path[MAX_STR_LEN];

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
      ERROR_PRINT("Multiple paths found for peek: %s and %s\n", path, c.argv[i]);
      return FAILURE;
    }
    else
    {
      found_path = true;
      strcpy(path, c.argv[i]);
    }
  }

  if (!found_path)
    strcpy(path, ".");

  tilde_to_homepath(path);

  struct dirent **entries;
  DEBUG_PRINT("Attempting to scandir %s\n", path);
  int n = scandir(path, &entries, 0, alphasort);
  if (n < 0)
  {
    ERROR_PRINT("Failed to read %s\n", path);
    return FAILURE;
  }
  int size_width = 0;
  int status = SUCCESS;
  if (flag_l)
  {
    size_t filesize = 0;
    size_t block_size = get_block_size_and_max_filesize(entries, n, path, flag_a, &filesize);
    printf("total %zu\n", block_size);
    size_width = num_digits(filesize);
  }
  for (int i = 0; i < n; ++i)
  {
    if (!flag_a && entries[i]->d_name[0] == '.')
      continue;

    if (flag_l)
    {
      if (print_long(path, entries[i]->d_name, size_width) == FAILURE)
      {
        status = FAILURE;
        break;
      }
    }
    else
      printf("%s\n", entries[i]->d_name);
  }
  for (int i = 0; i < n; ++i)
    free(entries[i]);
  free(entries);

  return status;
}

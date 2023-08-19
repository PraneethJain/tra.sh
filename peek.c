#include "headers.h"

string get_perms(struct stat *info)
{
  string perms = new_string(11);
  // First character in permissions:
  // -    Regular file
  // b    Block special file
  // c    Character special file
  // d    Directory
  // l    Symbolic link
  // p    FIFO
  // s    Socket
  if (S_ISDIR(info->st_mode))
    strcat(perms.str, "d");
  else if (S_ISCHR(info->st_mode))
    strcat(perms.str, "c");
  else if (S_ISBLK(info->st_mode))
    strcat(perms.str, "b");
  else if (S_ISREG(info->st_mode))
    strcat(perms.str, "-");
  else if (S_ISFIFO(info->st_mode))
    strcat(perms.str, "p");
  else if (S_ISLNK(info->st_mode))
    strcat(perms.str, "l");
  else if (S_ISSOCK(info->st_mode))
    strcat(perms.str, "s");
  else
    strcat(perms.str, "?");

  strcat(perms.str, info->st_mode & S_IRUSR ? "r" : "-");
  strcat(perms.str, info->st_mode & S_IWUSR ? "w" : "-");
  strcat(perms.str, info->st_mode & S_IXUSR ? "x" : "-");
  strcat(perms.str, info->st_mode & S_IRGRP ? "r" : "-");
  strcat(perms.str, info->st_mode & S_IWGRP ? "w" : "-");
  strcat(perms.str, info->st_mode & S_IXGRP ? "x" : "-");
  strcat(perms.str, info->st_mode & S_IROTH ? "r" : "-");
  strcat(perms.str, info->st_mode & S_IWOTH ? "w" : "-");
  strcat(perms.str, info->st_mode & S_IXOTH ? "x" : "-");

  return perms;
}

string get_datetime(struct stat *info)
{
  // When last modification is more than 6 months ago, year is shown instead of time
  string datetime = new_string(128);
  strftime(datetime.str, datetime.size, time(0) - info->st_mtime < 15768000 ? "%h %d %H:%M" : "%h %d %Y",
           localtime(&info->st_mtime));

  return datetime;
}

void print_long(char *path, const char *name)
{

  string cur = new_string(PATH_MAX);
  strcat(cur.str, path);
  strcat(cur.str, name);

  struct stat info;
  lstat(cur.str, &info);

  printf("%s ", get_perms(&info).str);
  printf(" %4li ", info.st_nlink);
  printf("%8s ", getpwuid(info.st_uid)->pw_name);
  printf("%8s ", getgrgid(info.st_gid)->gr_name);
  printf("%12li ", info.st_size); // Enough space for a terabyte
  printf("%s ", get_datetime(&info).str);
  printf("%s", name);
  printf("\n");
}

void peek(command c)
{
  bool flag_a = false;
  bool flag_l = false;
  bool found_path = false;
  string path = new_string(PATH_MAX);
  for (int i = 1; i < c.argc; ++i)
  {
    if (c.argv[i][0] == '-')
    {
      size_t len = strlen(c.argv[i]);
      for (int j = 1; j < len; ++j)
      {
        flag_l |= c.argv[i][j] == 'l';
        flag_a |= c.argv[i][j] == 'a';
      }
    }
    else if (found_path)
    {
      // Do error handling
      printf("Multiple paths found: %s and %s\n", path.str, c.argv[i]);
      return;
    }
    else
    {
      found_path = true;
      strcpy(path.str, c.argv[i]);
    }
  }

  printf("flag l is %i\n", flag_l);
  printf("flag a is %i\n", flag_a);
  if (!found_path)
  {
    strcpy(path.str, ".");
  }

  struct dirent **entries;
  int n = scandir(path.str, &entries, 0, alphasort);
  if (n < 0)
  {
    // Do error handling
  }
  else
  {
    for (int i = 0; i < n; ++i)
    {
      if (path.str[strlen(path.str) - 1] != '/')
      {
        strcat(path.str, "/");
      }

      if (!flag_a && entries[i]->d_name[0] == '.')
        continue;
      if (flag_l)
        print_long(path.str, entries[i]->d_name);
      else
        printf("%s\n", entries[i]->d_name);
    }
  }
}

#include "headers.h"

bool flag_a = false;
bool flag_l = true;

void print_long(char *path, const char *name)
{
  // When last modification is more than 6 months ago, year is shown instead of time

  string cur = new_string(PATH_MAX);
  strcat(cur.str, path);
  strcat(cur.str, name);
  struct stat info;
  lstat(cur.str, &info);

  // First character in permissions:
  // -    Regular file
  // b    Block special file
  // c    Character special file
  // d    Directory
  // l    Symbolic link
  // p    FIFO
  // s    Socket
  char filetype;
  if (S_ISDIR(info.st_mode))
    filetype = 'd';
  else if (S_ISCHR(info.st_mode))
    filetype = 'c';
  else if (S_ISBLK(info.st_mode))
    filetype = 'b';
  else if (S_ISREG(info.st_mode))
    filetype = '-';
  else if (S_ISFIFO(info.st_mode))
    filetype = 'p';
  else if (S_ISLNK(info.st_mode))
    filetype = 'l';
  else if (S_ISSOCK(info.st_mode))
    filetype = 's';
  else
    filetype = '?';

  string datetime = new_string(128);
  time_t cur_time = time(0);
  if (cur_time - info.st_mtime < 15768000)
  {
    strftime(datetime.str, datetime.size, "%h %d %H:%M", localtime(&info.st_mtime));
  }
  else
  {
    strftime(datetime.str, datetime.size, "%h %d %Y", localtime(&info.st_mtime));
  }

  printf("%c", filetype);
  printf(info.st_mode & S_IRUSR ? "r" : "-");
  printf(info.st_mode & S_IWUSR ? "w" : "-");
  printf(info.st_mode & S_IXUSR ? "x" : "-");
  printf(info.st_mode & S_IRGRP ? "r" : "-");
  printf(info.st_mode & S_IWGRP ? "w" : "-");
  printf(info.st_mode & S_IXGRP ? "x" : "-");
  printf(info.st_mode & S_IROTH ? "r" : "-");
  printf(info.st_mode & S_IWOTH ? "w" : "-");
  printf(info.st_mode & S_IXOTH ? "x" : "-");
  printf(" %4li ", info.st_nlink);
  printf("%8s ", getpwuid(info.st_uid)->pw_name);
  printf("%8s ", getgrgid(info.st_gid)->gr_name);
  printf("%12li ", info.st_size); // Enough space for a terabyte
  printf("%s ", datetime.str);
  printf("%s", name);
  printf("\n");
}

void peek(command c)
{
  struct dirent **entries;
  int n = scandir(c.argv[1], &entries, 0, alphasort);
  if (n < 0)
  {
    // Do error handling
  }
  else
  {
    for (int i = 0; i < n; ++i)
    {
      char *path = c.argv[1];
      if (path[strlen(path) - 1] != '/')
      {
        strcat(path, "/");
      }

      if (!flag_a && entries[i]->d_name[0] == '.')
        continue;
      if (flag_l)
        print_long(path, entries[i]->d_name);
      else
        printf("%s\n", entries[i]->d_name);
    }
  }
}

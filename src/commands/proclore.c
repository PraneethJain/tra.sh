#include "../headers.h"

int proclore(command c)
{
  pid_t pid;
  char *temp;
  if (c.argc == 1)
    pid = getpid();
  else if (c.argc == 2)
  {
    if (!is_numeric(c.argv[1]))
    {
      ERROR_PRINT("Invalid pid %s for proclore\n", c.argv[1]);
      return FAILURE;
    }
    pid = strtol(c.argv[1], &temp, 10);
  }
  else
  {
    ERROR_PRINT("Proclore takes at most 1 argument\n");
    return FAILURE;
  }

  char process_path[MAX_STR_LEN] = {0};
  snprintf(process_path, MAX_STR_LEN, "/proc/%i/stat", pid);
  FILE *process_file = fopen(process_path, "r");
  if (process_file == NULL)
  {
    ERROR_PRINT("No process with pid %i\n", pid);
    return FAILURE;
  }

  char status = '?';
  pid_t process_gid = 0;
  size_t vmem = 0;
  char buf[MAX_STR_LEN] = {0};
  fscanf(process_file, "%s %[^)]%c %c %s %i %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %zu", buf, buf, &buf[0],
         &status, buf, &process_gid, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf,
         buf, &vmem);
  fclose(process_file);
  char exe_proc_path[MAX_STR_LEN] = {0};
  snprintf(exe_proc_path, MAX_STR_LEN, "/proc/%i/exe", pid);
  char exe_path[MAX_STR_LEN] = {0};
  int res = readlink(exe_proc_path, exe_path, MAX_STR_LEN);
  if (res == -1)
    DEBUG_PRINT("Couldn't read exe path\n");
  else
    exe_path[res] = '\0';

  homepath_to_tilde(exe_path);

  printf("pid: %i\n", pid);
  printf("Process Status: %c%c\n", status, pid == getpid() ? '+' : ' ');
  printf("Process Group: %i\n", process_gid);
  printf("Virtual Memory: %zu\n", vmem);
  printf("Executable Path: %s\n", exe_path);

  return SUCCESS;
}

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

  string process_path = new_string(MAX_STR_LEN);
  snprintf(process_path.str, process_path.size, "/proc/%i/stat", pid);
  FILE *process_file = fopen(process_path.str, "r");
  free(process_path.str);
  if (process_file == NULL)
  {
    ERROR_PRINT("No process with pid %i\n", pid);
    return FAILURE;
  }

  char status = '?';
  pid_t process_gid = 0;
  size_t vmem = 0;
  char buf[MAX_STR_LEN];
  fscanf(process_file, "%s %[^)]%c %c %s %i %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %zu", buf, buf, &buf[0],
         &status, buf, &process_gid, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf, buf,
         buf, &vmem);

  string exe_proc_path = new_string(MAX_STR_LEN);
  snprintf(exe_proc_path.str, exe_proc_path.size, "/proc/%i/exe", pid);
  string exe_path = new_string(MAX_STR_LEN);
  int res = readlink(exe_proc_path.str, exe_path.str, exe_path.size);
  free(exe_proc_path.str);
  if (res == -1)
  {
    DEBUG_PRINT("Couldn't read exe path\n");
  }
  else
  {
    exe_path.str[res] = '\0';
  }

  if (strstr(exe_path.str, homepath.str) == exe_path.str) // If cwd starts with homepath
    replace(&exe_path, homepath, tilde);

  printf("pid: %i\n", pid);
  printf("Process Status: %c%c\n", status, pid == getpid() ? '+' : ' ');
  printf("Process Group: %i\n", process_gid);
  printf("Virtual Memory: %zu\n", vmem);
  printf("Executable Path: %s\n", exe_path.str);
  free(exe_path.str);

  return SUCCESS;
}

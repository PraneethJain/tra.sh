#include "headers.h"

void proclore(command c)
{
  pid_t pid;
  char *temp;
  if (c.argc == 1)
    pid = getpid();
  else if (c.argc == 2)
    pid = strtol(c.argv[1], &temp, 10);
  else
  {
    // Do error handling
    return;
  }

  string process_path = new_string(PATH_MAX);
  snprintf(process_path.str, process_path.size, "/proc/%i/stat", pid);
  FILE *process_file = fopen(process_path.str, "r");
  if (process_file == NULL)
  {
    printf("No such process\n");
    return;
  }

  string process_file_contents = new_string(MAX_STR_LEN);
  if (fread(process_file_contents.str, 1, process_file_contents.size, process_file) == 0)
  {
    printf("Could not access process details\n");
    fclose(process_file);
    return;
  }
  fclose(process_file);

  string tok = new_string(MAX_STR_LEN);
  tok.str = strtok(process_file_contents.str, " ");
  int i = 2;
  char status;
  pid_t process_gid;
  size_t vmem;
  while ((tok.str = strtok(NULL, " ")) != NULL)
  {
    switch (i)
    {
    case 3:
      status = tok.str[0];
      break;

    case 5:
      process_gid = strtol(tok.str, &temp, 10);
      break;

    case 23:
      vmem = strtol(tok.str, &temp, 10);
      break;
    }
    ++i;
  }

  string exe_proc_path = new_string(32);
  snprintf(exe_proc_path.str, exe_proc_path.size, "/proc/%i/exe", pid);
  string exe_path = new_string(PATH_MAX);
  if (readlink(exe_proc_path.str, exe_path.str, exe_path.size) == -1)
  {
    // Do error handling
  }
  pid_t term_gid = getpgrp();
  printf("pid: %i\n", pid);
  printf("Process Status: %c%c\n", status, term_gid == process_gid ? '+' : ' ');
  printf("Process Group: %i\n", process_gid);
  printf("Virtual Memory: %zu\n", vmem);
  printf("Executable Path: %s\n", exe_path.str);
}

#include "../headers.h"

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

  string process_path = new_string(MAX_STR_LEN);
  snprintf(process_path.str, process_path.size, "/proc/%i/stat", pid);
  FILE *process_file = fopen(process_path.str, "r");
  free(process_path.str);
  if (process_file == NULL)
  {
    printf("No such process\n");
    return;
  }

  char status = '?';
  pid_t process_gid = 0;
  size_t vmem = 0;
  char buf[MAX_STR_LEN];
  for (int i = 2; i < 24; ++i)
  {
    if (i == 3)
      fscanf(process_file, "%c", &status);
    else if (i == 5)
      fscanf(process_file, "%i", &process_gid);
    else if (i == 23)
      fscanf(process_file, "%zu", &vmem);
    else
      fscanf(process_file, "%s", buf);
  }

  // string process_file_contents = new_string(MAX_STR_LEN);
  // if (fread(process_file_contents.str, 1, process_file_contents.size, process_file) == 0)
  // {
  //   printf("Could not access process details\n");
  //   fclose(process_file);
  //   return;
  // }
  // fclose(process_file);

  // char *tok;
  // tok = strtok(process_file_contents.str, " ");
  // int i = 2;
  // char status = '?';
  // pid_t process_gid = 0;
  // size_t vmem = 0;
  // while ((tok = strtok(NULL, " ")) != NULL)
  // {
  //   switch (i)
  //   {
  //   case 3:
  //     status = tok[0];
  //     break;
  //
  //   case 5:
  //     process_gid = strtol(tok, &temp, 10);
  //     break;
  //
  //   case 23:
  //     vmem = strtol(tok, &temp, 10);
  //     break;
  //   }
  //   ++i;
  // }
  // free(process_file_contents.str);

  string exe_proc_path = new_string(32);
  snprintf(exe_proc_path.str, exe_proc_path.size, "/proc/%i/exe", pid);
  string exe_path = new_string(MAX_STR_LEN);
  int res = readlink(exe_proc_path.str, exe_path.str, exe_path.size);
  free(exe_proc_path.str);
  if (res == -1)
  {
    // Do error handling
  }
  else
  {
    exe_path.str[res] = '\0';
  }

  if (strstr(exe_path.str, homepath.str) == exe_path.str) // If cwd starts with homepath
    replace(&exe_path, homepath, tilde);

  pid_t term_gid = getpgrp();
  printf("pid: %i\n", pid);
  printf("Process Status: %c%c\n", status, term_gid == process_gid ? '+' : ' ');
  printf("Process Group: %i\n", process_gid);
  printf("Virtual Memory: %zu\n", vmem);
  printf("Executable Path: %s\n", exe_path.str);
  free(exe_path.str);
}

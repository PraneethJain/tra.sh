#include "headers.h"

void system_command(command c)
{
  // string arguments[MAX_ARGS];
  // string tok = new_string(4096);
  // string delimiters = new_string(128);
  // strcpy(delimiters.str, " \t\n\v\f\r");
  //
  // tok.str = strtok(input.str, delimiters.str);
  // int argc = 0;
  // arguments[argc] = new_string(MAX_STR_LEN);
  // strcpy(arguments[argc++].str, tok.str);
  // while ((tok.str = strtok(NULL, delimiters.str)) != NULL)
  // {
  //   arguments[argc] = new_string(MAX_STR_LEN);
  //   strcpy(arguments[argc++].str, tok.str);
  // }
  //
  // char **temp = to_cstring_array(arguments, argc);
  pid_t pid = fork();
  if (pid == 0)
  {
    if (execvp(c.argv[0], c.argv) == -1)
    {
      // Do error handling
    }
  }
  else
  {
    int status;
    waitpid(pid, &status, 0);
  }
}

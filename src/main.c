#include "headers.h"

bool EXIT;
string homepath;
string lastpath;
string tilde;
string delimiters;
process_list p;
history h;
time_t max_time_taken;
command slowest_command;

void init()
{
  EXIT = false;
  homepath = new_string(MAX_STR_LEN);
  getcwd(homepath.str, homepath.size);
  lastpath = new_string(MAX_STR_LEN);
  tilde = new_string(2);
  strcpy(tilde.str, "~\0");
  delimiters = new_string(128);
  strcpy(delimiters.str, " \t\n\v\f\r");
  p = init_process_list();
  max_time_taken = 0;

  init_history();
  init_prompt();
}

void destroy()
{
  free(homepath.str);
  free(lastpath.str);
  free(tilde.str);
  free(delimiters.str);
  free_process_list(p);

  destroy_prompt();
  destroy_history();
}

int main()
{
  init();
  while (!EXIT)
  {
    remove_processes(p);
    prompt();
    string input = new_string(MAX_STR_LEN);
    fgets(input.str, input.size, stdin);
    parse_input(input);
    free(input.str);
  }
  destroy();
}

#include "headers.h"

bool EXIT;
string homepath;
string lastpath;
string tilde;
string delimiters;
process_list p;
const char *history_path = ".trash_history";
history h;

void init()
{
  EXIT = false;
  homepath = new_string(PATH_MAX);
  getcwd(homepath.str, homepath.size);
  lastpath = new_string(PATH_MAX);
  tilde = new_string(2);
  strcpy(tilde.str, "~\0");
  delimiters = new_string(128);
  strcpy(delimiters.str, " \t\n\v\f\r");
  p = init_process_list();

  FILE *history_file = fopen(history_path, "rb");
  if (history_file == NULL)
  {
    h.cur_size = 0;
  }
  else
  {
    fread(&h, sizeof(history), 1, history_file);
    fclose(history_file);
  }

  init_prompt();
}

void destroy()
{
  free(homepath.str);
  free(lastpath.str);
  free(tilde.str);
  free_process_list(p);

  destroy_prompt();
}

int main()
{

  // To do (till Part A):
  // time taken in prompt
  // peek block size
  // seek

  init();
  while (!EXIT)
  {
    remove_processes(p);
    prompt();
    string input = new_string(MAX_STR_LEN);
    fgets(input.str, input.size, stdin);
    parse_input(input);
  }
  destroy();
}

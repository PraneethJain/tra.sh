#include "headers.h"

bool EXIT;
string homepath;
string lastpath;
process_list p;
const char *history_path = ".trash_history";
history h;

int main()
{
  EXIT = false;
  homepath = new_string(PATH_MAX);
  getcwd(homepath.str, homepath.size);
  lastpath = new_string(PATH_MAX);
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

  // To do (till Part A):
  // time taken in prompt
  // peek block size
  // seek

  while (!EXIT)
  {
#ifdef DEBUG
    printf("IN DEBUG MODE\n");
#endif
    remove_processes(p);
    prompt();
    string input = new_string(MAX_STR_LEN);
    fgets(input.str, input.size, stdin);
    parse_input(input);
  }
}

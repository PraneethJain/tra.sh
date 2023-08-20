#include "headers.h"

string homepath;
string lastpath;
process_list p;
const char *history_path = ".trash_history";
history h;

int main()
{

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

  while (1)
  {
    remove_processes(p);
    prompt();
    string input = new_string(4096);
    fgets(input.str, 4096, stdin);
    parse_input(input);
  }
}

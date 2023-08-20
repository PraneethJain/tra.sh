#include "headers.h"

string homepath;
string lastpath;
process_list p;

int main()
{

  homepath = new_string(PATH_MAX);
  getcwd(homepath.str, homepath.size);
  lastpath = new_string(PATH_MAX);
  p = init_process_list();

  while (1)
  {
    remove_processes(p);
    prompt();
    string input = new_string(4096);
    fgets(input.str, 4096, stdin);
    parse_input(input);
  }
}

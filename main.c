#include "headers.h"

string homepath;
string lastpath;

int main()
{

  homepath = new_string(PATH_MAX);
  getcwd(homepath.str, homepath.size);

  lastpath = new_string(PATH_MAX);

  while (1)
  {
    prompt();
    string input = new_string(4096);
    fgets(input.str, 4096, stdin);
    parse_input(input);
  }
}

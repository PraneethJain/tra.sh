#include "headers.h"

string homepath;

int main()
{

  homepath = new_string(PATH_MAX);
  getcwd(homepath.str, homepath.size);

  while (1)
  {
    prompt();
    string input = new_string(4096);
    fgets(input.str, 4096, stdin);
    parse_input(input);
  }
}

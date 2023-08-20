#ifndef __BASE_H
#define __BASE_H

#include <stdio.h>
#define MAX_ARGS 1024
#define MAX_STR_LEN 1024

typedef struct string
{
  char *str;
  size_t size;
} string;

typedef struct command
{
  int argc;
  char **argv;
  bool is_background;
} command;

string new_string(size_t size);
void replace(string *s, string a, string b);
char **to_cstring_array(string *s, size_t len);

int max(int x, int y);

#endif

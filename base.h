#ifndef __BASE_H
#define __BASE_H

#include <stdio.h>

typedef struct string
{
  char *str;
  size_t size;
} string;

string new_string(size_t size);
void replace(string *s, string a, string b);
char **to_cstring_array(string *s, size_t len);

#endif

#include "../headers.h"

string new_string(size_t size)
{
  string s;
  s.str = (char *)malloc(size * sizeof(char));
  s.size = size;
  s.str[0] = '\0';

  return s;
}

// Replaces the first occurence of a in s by b
void replace(string *s, string a, string b)
{
  char *ptr = strstr(s->str, a.str);
  if (ptr == NULL)
    return;

  size_t idx = ptr - s->str;
  string temp = new_string(s->size);
  strncpy(temp.str, s->str, idx);
  strcat(temp.str, b.str);
  strcat(temp.str, s->str + idx + strlen(a.str));

  s->str = temp.str;
}

char **to_cstring_array(string *s, size_t len)
{
  char **res = (char **)malloc(sizeof(char *) * (len + 1));
  for (int i = 0; i < len; ++i)
    res[i] = s[i].str;
  res[len] = NULL;
  return res;
}

int max(int x, int y)
{
  return x > y ? x : y;
}

int min(int x, int y)
{
  return x < y ? x : y;
}
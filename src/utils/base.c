#include "../headers.h"

void tilde_to_homepath(char *s)
{
  char temp[MAX_STR_LEN];
  temp[0] = '\0';
  if (s[0] == '~')
  {
    strcpy(temp, state->homepath);
    strcat(temp, s + 1);
    strcpy(s, temp);
  }
}

void homepath_to_tilde(char *s)
{
  if (strstr(s, state->homepath) == s)
  {
    size_t s_len = strlen(s);
    size_t homepath_len = strlen(state->homepath);
    for (size_t i = 1; i <= s_len - homepath_len + 1; ++i)
      s[i] = s[i + homepath_len - 1];
    s[0] = '~';
  }
}

void print_command(command *c)
{
  for (int i = 0; i < c->argc; ++i)
    printf("%s ", c->argv[i]);
  printf(c->is_background ? "& " : "; ");
}

void print_commands(commands *cs)
{
  for (size_t i = 0; i < cs->size; ++i)
    print_command(&cs->arr[i]);
  printf("\n");
}

bool command_equal(command *c1, command *c2)
{
  if (c1->argc != c2->argc || c1->is_background != c2->is_background)
    return false;

  for (int i = 0; i < c1->argc; ++i)
    if (strcmp(c1->argv[i], c2->argv[i]) != 0)
      return false;

  return true;
}

bool commands_equal(commands *cs1, commands *cs2)
{
  if (cs1->size != cs2->size)
    return false;

  for (size_t i = 0; i < cs1->size; ++i)
    if (!command_equal(&cs1->arr[i], &cs2->arr[i]))
      return false;

  return true;
}

int max(int x, int y)
{
  return x > y ? x : y;
}

int min(int x, int y)
{
  return x < y ? x : y;
}

int num_digits(int x)
{
  int res = 0;

  do
  {
    x /= 10;
    ++res;
  } while (x);

  return res;
}

bool is_numeric(char *s)
{
  for (int i = 0; s[i] != '\0'; ++i)
    if (s[i] < '0' || s[i] > '9')
      return false;

  return true;
}

#include "../headers.h"

string new_string(size_t size)
{
  string s;
  s.size = size;
  s.str = (char *)malloc(size * sizeof(char));
  if (s.str == NULL)
  {
    DEBUG_PRINT("Bad malloc\n");
    ERROR_PRINT("Ran out of memory!");
    s.allocated = false;
  }
  else
  {
    s.allocated = true;
    s.str[0] = '\0';
  }

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
  strcpy(s->str, temp.str);
  free(temp.str);
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

process_list merge(process_list list1, process_list list2)
{
  process_list result = NULL;

  if (list1 == NULL)
    return list2;
  if (list2 == NULL)
    return list1;

  if (list1->pid <= list2->pid)
  {
    result = list1;
    result->next = merge(list1->next, list2);
  }
  else
  {
    result = list2;
    result->next = merge(list1, list2->next);
  }

  return result;
}

void split(process_list source, process_list *front, process_list *back)
{
  process_list slow = source;
  process_list fast = source->next;

  while (fast != NULL)
  {
    fast = fast->next;
    if (fast != NULL)
    {
      slow = slow->next;
      fast = fast->next;
    }
  }

  *front = source;
  *back = slow->next;
  slow->next = NULL;
}

void merge_sort(process_list *head)
{
  process_list temp = *head;
  if (temp == NULL || temp->next == NULL)
    return;

  process_list a;
  process_list b;

  split(temp, &a, &b);

  merge_sort(&a);
  merge_sort(&b);

  *head = merge(a, b);
}
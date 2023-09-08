#include "../headers.h"

int activities(command c)
{
  if (c.argc != 1)
  {
    ERROR_PRINT("Activites accepts no arguments, found %i arguments\n", c.argc - 1);
    return FAILURE;
  }

  print_process_list();
  return SUCCESS;
}
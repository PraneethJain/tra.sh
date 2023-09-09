#include "../headers.h"

struct termios orig_termios;

int enable_raw_mode()
{
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
  {
    DEBUG_PRINT("tcgetattr failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Failed to enable raw mode\n");
    return FAILURE;
  }

  atexit((void *)disable_raw_mode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ICANON | ECHO);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
  {
    DEBUG_PRINT("tcsetattr failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Failed to enable raw mode\n");
    return FAILURE;
  }

  return SUCCESS;
}

int disable_raw_mode()
{
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
  {
    DEBUG_PRINT("tcsetattr failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Failed to disable raw mode\n");
    return FAILURE;
  }

  return SUCCESS;
}

int read_line_raw()
{
  setbuf(stdout, NULL);
  if (enable_raw_mode() == FAILURE)
    return FAILURE;

  input.length = 0;
  input.str[0] = '\0';
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1)
  {
    if (iscntrl(c))
    {
      if (c == 10) // newline
      {
        printf("\n");
        break;
      }
      else if (c == 9) // tab
      {
        for (int i = 0; i < 8; ++i)
          printf(" ");
        input.str[input.length++] = c;
      }
      else if (c == 127 && input.length > 0) // backspace
      {
        if (input.str[input.length - 1] == 9) // In case of tab, remove whole tab
          for (int i = 0; i < 7; i++)
            printf("\b");
        input.str[--input.length] = '\0';
        printf("\b \b");
      }
      else if (c == 4) // END OF TRANSMISSION
      {
        exit(0);
      }
    }
    else // normal character
    {
      input.str[input.length++] = c;
      printf("%c", c);
    }
  }
  input.str[input.length++] = '\0';
  if (disable_raw_mode() == FAILURE)
    return FAILURE;

  return SUCCESS;
}

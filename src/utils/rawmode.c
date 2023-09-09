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

string read_line_raw()
{
  setbuf(stdout, NULL);
  enable_raw_mode();
  string inp = new_string(MAX_STR_LEN);
  int cur_len = 0;
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
        inp.str[cur_len++] = c;
      }
      else if (c == 127 && cur_len > 0) // backspace
      {
        if (inp.str[cur_len - 1] == 9) // In case of tab, remove whole tab
          for (int i = 0; i < 7; i++)
            printf("\b");
        inp.str[--cur_len] = '\0';
        printf("\b \b");
      }
      else if (c == 4) // END OF TRANSMISSION
      {
        exit(0);
      }
    }
    else
    {
      inp.str[cur_len++] = c;
      printf("%c", c);
    }
  }
  inp.str[cur_len++] = '\0';
  disable_raw_mode();
  return inp;
}

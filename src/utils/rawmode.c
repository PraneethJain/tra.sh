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

  state.input[0] = '\0';
  state.input_length = 0;
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
        state.input[state.input_length++] = c;
      }
      else if (c == 127 && state.input_length > 0) // backspace
      {
        if (state.input[state.input_length - 1] == 9) // In case of tab, remove whole tab
          for (int i = 0; i < 7; i++)
            printf("\b");
        state.input[--state.input_length] = '\0';
        printf("\b \b");
      }
      else if (c == 4) // END OF TRANSMISSION
      {
        exit(0);
      }
    }
    else // normal character
    {
      state.input[state.input_length++] = c;
      printf("%c", c);
    }
  }
  state.input[state.input_length++] = '\0';
  if (disable_raw_mode() == FAILURE)
    return FAILURE;

  return SUCCESS;
}

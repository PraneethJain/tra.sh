#ifndef HEADERS_H_
#define HEADERS_H_

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <limits.h>
#include <math.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "inc/activities.h"
#include "inc/base.h"
#include "inc/bg.h"
#include "inc/colors.h"
#include "inc/command.h"
#include "inc/fg.h"
#include "inc/pastevents.h"
#include "inc/peek.h"
#include "inc/ping.h"
#include "inc/proclore.h"
#include "inc/procs.h"
#include "inc/prompt.h"
#include "inc/rawmode.h"
#include "inc/seek.h"
#include "inc/signals.h"
#include "inc/system.h"
#include "inc/warp.h"

typedef struct trash
{
  bool EXIT;
  char input[MAX_STR_LEN];
  size_t input_length;
  char homepath[MAX_STR_LEN];
  char lastpath[MAX_STR_LEN];
  char tilde[2];
  char delimiters[128];
  processes procs;
  history h;
  time_t max_time_taken;
  command slowest_command;
} trash;

extern trash *state;

#endif

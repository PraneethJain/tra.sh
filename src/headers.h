#ifndef HEADERS_H_
#define HEADERS_H_

#define MAX_ARGS 1024
#define MAX_STR_LEN 1024

#include <assert.h>
#include <dirent.h>
#include <grp.h>
#include <limits.h>
#include <linux/limits.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "inc/base.h"
#include "inc/colors.h"
#include "inc/command.h"
#include "inc/peek.h"
#include "inc/proclore.h"
#include "inc/procs.h"
#include "inc/prompt.h"
#include "inc/system.h"
#include "inc/warp.h"

extern string homepath;
extern string lastpath;
extern process_list p;

#endif

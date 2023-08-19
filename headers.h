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

#include "base.h"
#include "command.h"
#include "peek.h"
#include "prompt.h"
#include "system.h"
#include "warp.h"

extern string homepath;
extern string lastpath;

#endif

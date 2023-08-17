#ifndef HEADERS_H_
#define HEADERS_H_

#define MAX_ARGS 1024
#define MAX_STR_LEN 1024

#include <assert.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "base.h"
#include "command.h"
#include "prompt.h"
#include "system.h"
#include "warp.h"

extern string homepath;

#endif

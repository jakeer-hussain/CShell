#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>
#include "pwd.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <time.h>
#include "prompt.h"
#include "warp.h"
#include "peek.h"
#include "pastevents.h"
#include "proclore.h"
#define MAX_PATH_LENGTH 4096
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define MAX_HISTORY_SIZE 15
#define MAX_INT_LENGTH 4096
#define MAX_PATH 256
#define MAX_LINE 256
#endif
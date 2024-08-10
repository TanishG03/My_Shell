#ifndef __PIPE_H
#define __PIPE_H


#include "headers.h"

#define MAX_SIZE 1000


int piping(char *segment_token,char history[HISTORY_MAX_SIZE][4096], int *history_count, char *og_home, char *cwd);

#endif
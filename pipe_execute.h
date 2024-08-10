#ifndef __PIPE_EXECUTE_H
#define __PIPE_EXECUTE_H

#include "headers.h"

// void pipe_execute(char * pipe_command,char history[HISTORY_MAX_SIZE][4096], int *history_count, char *og_home, char *cwd);

void pipe_execute(char *og_home, char *cwd , char *args[],char history[HISTORY_MAX_SIZE][4096], int *history_count);


#endif
#ifndef REDIRECTION_H_
#define REDIRECTION_H_

#include "headers.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>


void execute_command(char* args[], int input_fd, int output_fd,char history[15][4096], int *history_count);

int redirection(char * command,char * input,char history[15][4096], int *history_count);
#endif
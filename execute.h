#ifndef __EXECUTE_H
#define __EXECUTE_H
#include "peek.h"
#include "warp.h"
#include "headers.h"
#include"history.h"
#include"peek.h"
#include "warp.h"
#include"proclore.h"
#include "prompt.h"

#define MAX_ARGS 100
#define MAX_BG_PROCESSES 100

// typedef struct BackgroundProcess {
//     pid_t pid;
//     char *command;
// }BackgroundProcess;

// typedef struct bg_process {
//     pid_t pid_process;
//     char *process_name;
//     struct bg_process *next;
// } bg_process;

// extern bg_process *head;
// extern bg_process *tail;
// extern int bg_process_size;

// extern int restart_loop;
// void sigchld_handler(int signo);

typedef struct {
    pid_t pid_process;
    char *process_name;
} bg_process;

// bg_process bg_processes[MAX_BG_PROCESSES];
// int bg_process_count = 0;

void sigchld_handler(int signo);


void execute_background(char * token);
void execute_foreground(char *command);
void execute_segments(char *input); 

void execute (char history[HISTORY_MAX_SIZE][4096], int *history_count, char *input, char *og_home, char *cwd);

#endif
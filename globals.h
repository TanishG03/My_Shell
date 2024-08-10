// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H
#include"headers.h"
#include"activities.h"

extern char *arguments[100];
extern int arguementCount;
extern pid_t shell_pid;  // Declare the global variable
extern int exec_success;
extern double proc_time;
extern int input_fd;
extern int output_fd;

extern int stdin_fileno;
extern int stdout_fileno;

extern node bg_proc_list_head;
extern pid_t latest_pid;

extern volatile int exit_requested;
extern volatile int neonatePID;  // Initialize with 0
extern char myHome[1000];

#endif

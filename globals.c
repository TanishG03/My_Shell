#include "globals.h"
pid_t shell_pid;
int exec_success;
double proc_time;
int input_fd;
int output_fd;
char *arguments[100];
int arguementCount;
 int stdin_fileno;
int stdout_fileno;
//  int bg_proc_count;

node bg_proc_list_head;
pid_t latest_pid;
char myHome[1000];

// volatile int exit_requested;
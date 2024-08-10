#ifndef __NEONATE_H
#define __NEONATE_H

#include "headers.h"

char* input_cmd_2();

void sendExitSignalToNeonate(int sig);
int findLatestPID();
void runInfiniteLoop();

void kill_proc(pid_t pid);

int kbhit(void);

int neonate();

#endif
#ifndef __PEEK_H
#define __PEEK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
int compare(const void *arg1, const void *arg2);
char file_type(mode_t mode);
int ls_print(char* file);
int ls_l_print(char* file);

int peek(char * token , char * cwd, char *og_home);

#endif



#ifndef __WARP_H
#define __WARP_H

#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<string.h>


void get_parent_directory(const char *current_path, char *parent_path);
int warp(char *token,char * cwd, char * og_home);
#endif
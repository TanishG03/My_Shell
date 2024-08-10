
#ifndef __SEEK_H
#define __SEEK_H
#include "peek.h"
#include "warp.h"
#include "headers.h"
#include"history.h"
#include"peek.h"
#include "warp.h"
#include"proclore.h"
#include "prompt.h"


void list_files(const char *path, const char *search,int dir_only,int file_only);

int seek(char *token, char *og_dir, char *cwd);

#endif
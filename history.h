#ifndef __HISTORY_H
#define __HISTORY_H

#include "peek.h"
#include "warp.h"
#include "headers.h"
#define HISTORY_MAX_SIZE 15


void removeNewline(char *str);

void saveHistoryToFile(const char history[HISTORY_MAX_SIZE][4096], int history_count);

void loadHistoryFromFile(char history[HISTORY_MAX_SIZE][4096], int *history_count);

void addToHistory(char history[HISTORY_MAX_SIZE][4096], int *history_count, const char *command);

// Function to print history
void printHistory(const char history[HISTORY_MAX_SIZE][4096], int history_count);
void init_history(char *token,char history[HISTORY_MAX_SIZE][4096], int *history_count, char *og_home, char *cwd);
#endif
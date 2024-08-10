#ifndef __ACTIVITIES_H
#define __ACTIVITIES_H

#include "headers.h"
#include "globals.h"


typedef struct list_node
{
    int proc_num;
    pid_t pid;
    char *command;
    struct list_node *next;
} * node;

typedef node  jobs_node;

typedef struct jobs_node
{
    int proc_num;
    char state;
    char *command;
    pid_t pid;
} * jobs_node_ptr;

node init_node();
node init_list();

void add_node(pid_t pid);

void remove_node(pid_t pid);
char bg_proc_status(pid_t pid);
int cmp_jobs(const void *a, const void *b);
extern int bg_proc_count;


int activities(char *token);
#endif
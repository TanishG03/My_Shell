#include "headers.h"


int ping ()
{
    if(arguementCount < 3)
    {
        perror("Less number of arguements!\n");
        return 0;
    }

     if(arguementCount>3)
    {
        perror("More number of arguements!\n");
        return 0;
    }

    int job_num = atoi(arguments[1]);
    int sig_num = atoi(arguments[2]);

    sig_num=sig_num%32;


    if (bg_proc_count == 0)
    {
        perror("Error: sig: no background job with the given job number exists\n");
        return 0;
    }

    pid_t pid = -1;
    node itr = bg_proc_list_head;

    while (itr != NULL)
    {
        if (itr->pid == job_num)
        {
            pid = itr->pid;
            break;
        }

        itr = itr->next;
    }

    if (pid == -1)
    {
        perror("Error: sig: no background job with the given job number exists\n");
        return 0;
    }

    if (kill(pid, sig_num) < 0)
    {
        perror("Error: sig");
    }

    return 1;


}
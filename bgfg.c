#include "headers.h"
int bg()
{
    if (arguementCount != 2)
    {
        if (arguementCount < 2)
        {
            perror("Error: bg: too less arguments\n");
        }
        else
        {
            perror("Error: bg: too many arguments\n");
        }

        return 0;
    }

    int job_num = atoi(arguments[1]);
    if (job_num <= 0)
    {
        perror("Error: bg: invalid argument type\n");
        return 0;
    }

    // printf("%d\n", bg_proc_count);
    if (bg_proc_count == 0)
    {
        perror("Error: bg: no background job with this job number exists\n");
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
    // printf("%d\n", pid);
    if (pid == -1)
    {
        perror("Error: bg: No background job with this job number exists\n");
        return 0;
    }
    // else{
    //     execute_segments(itr->command);
    //     return;
    // }

    if (kill(pid, SIGCONT) < 0)
    {
        perror("Error: bg");
    }

    return 1;
}

int fg()
{
    if (arguementCount != 2)
    {
        if (arguementCount < 2)
        {
            perror("Error: fg: Less arguments\n");
        }
        else
        {
            perror("Error: fg: Many arguments\n");
        }

        return 0;
    }

    int job_num = atoi(arguments[1]);
    if (job_num <= 0)
    {
        perror("Error: fg: invalid argument type\n");
        return 0;
    }

    if (bg_proc_count == 0)
    {
        perror("Error: fg: no background job with the given job number exists\n");
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
        perror("Error: fg: no background job with the given job number exists\n");
        return 0;
    }


    

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, getpgid(pid));

    if (kill(pid, SIGCONT) < 0)
    {
        perror("Error: fg");
        return 0;
    }

    int status;
    waitpid(pid, &status, WUNTRACED);

    // if (WIFSTOPPED(status))
    // {
    //     // Move the stopped process to the background list
    //     // add_node(pid);
    //     return 1;
    // }
        if (WIFSTOPPED(status))
    {
        // Do not remove the node; it's just stopped
        tcsetpgrp(0, getpgid(0));
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        return 1;
    }
    tcsetpgrp(0, getpgid(0));
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    remove_node(pid);

    return 1;
}
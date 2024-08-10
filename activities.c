#include "headers.h"

#include "globals.h"


node init_node()
{
    node head = (node)malloc(sizeof(struct list_node));
    head->proc_num = 1;
    head->pid = -1;
    head->command = (char *)malloc(MAX_SIZE);
    head->next = NULL;
    return head;
}

node init_list()
{
    bg_proc_count = 0;
    node head = init_node();
    return head;
}

void add_node(pid_t pid) {
    node itr = bg_proc_list_head;
    bool found_empty = false;

    if (bg_proc_count == 0) {
        itr->pid = pid;
        bg_proc_count++;

        strcpy(itr->command, arguments[0]);
        for (int i = 1; i < arguementCount; i++) {
            strcat(itr->command, " ");
            // if (strcmp(arguments[i], "&") != 0) {
                strcat(itr->command, arguments[i]);
            // }
        }
    } else {
        while (itr->next != NULL) {
            if (itr->pid == -1) {
                found_empty = true;
                break;
            }
            itr = itr->next;
        }

        if (!found_empty && itr->pid != -1) {
            node head = init_node();

            head->proc_num = ++bg_proc_count;
            head->pid = pid;

            strcpy(head->command, arguments[0]);
            for (int i = 1; i < arguementCount; i++) {
                strcat(head->command, " ");
                strcat(head->command, arguments[i]);
            }

            itr->next = head;
        } else {
            itr->pid = pid;
            bg_proc_count++;

            strcpy(itr->command, arguments[0]);
            for (int i = 1; i < arguementCount; i++) {
                strcat(itr->command, " ");
                // if (strcmp(arguments[i], "&") != 0) {
                    strcat(itr->command, arguments[i]);
                // }
            }
        }
    }
}


// void add_node(pid_t pid)
// {
//     node itr = bg_proc_list_head;

//     if (bg_proc_count == 0)
//     {
//         itr->pid = pid;
//         bg_proc_count++;

//         strcpy(itr->command, arguments[0]);
//         for (int i = 1; i < arguementCount; i++)
//         {
//             strcat(itr->command, " ");
//             if(strcmp(arguments[i],"&")!=0)
//             {
//                 strcat(itr->command, arguments[i]);
//             }

//         }

//         return;
//     }

//     bool flag = false;
//     while (itr->next != NULL)
//     {
//         if (itr->pid == -1)
//         {
//             flag = true;
//             break;
//         }

//         itr = itr->next;
//     }

//     if (!flag)
//     {
//         if (itr->pid == -1)
//         {
//             flag = true;
//         }
//     }

//     if (flag)
//     {
//         itr->pid = pid;
//         bg_proc_count++;

//         strcpy(itr->command, arguments[0]);
//         for (int i = 1; i < arguementCount; i++)
//         {
//             strcat(itr->command, " ");
//             strcat(itr->command, arguments[i]);
//         }
//     }
//     else
//     {
//         node head = init_node();

//         head->proc_num = ++bg_proc_count;
//         head->pid = pid;

//         strcpy(head->command, arguments[0]);
//         for (int i = 1; i < arguementCount; i++)
//         {
//             strcat(head->command, " ");
//             strcat(head->command, arguments[i]);
//         }

//         itr->next = head;
//     }

//     return;
// }

void remove_node(pid_t pid)
{
    node itr = bg_proc_list_head;

    while (itr != NULL && itr->pid != pid)
    {
        itr = itr->next;
    }

    if (itr == NULL)
    {
        return;
    }

    itr->pid = -1;
    free(itr->command);
    itr->command = (char *)malloc(MAX_SIZE);
    bg_proc_count--;

    return;
}

char bg_proc_status(pid_t pid)
{
    char proc_stat[MAX_SIZE];
    char procFile[MAX_SIZE];
    sprintf(procFile, "/proc/%d/stat", pid);
    FILE *statFile = fopen(procFile, "r");

    if (statFile == NULL)
    {

        return 'c';
    }


    for(int i=0;i<3;++i)
    {
        char head[MAX_SIZE];
        fscanf(statFile, "%s", head);
        if(i==2)
        {
            strcpy(proc_stat, head);
        }
    }

    return proc_stat[0];
}

int cmp_jobs(const void *a, const void *b)
{
    jobs_node_ptr p1 = (jobs_node_ptr)a;
    jobs_node_ptr p2 = (jobs_node_ptr)b;

    return strcmp(p1->command, p2->command);
}

int activities(char * token)
{

    int argc=0;
    char * args[100];
    // printf("%s\n",token);
    args[argc]=token;
    argc++;

    while ((token = strtok(NULL, " \t\n")) != NULL) {
                    args[argc] = token;
                    argc++;
    }
  

    if (argc <= 0 || argc > 1)
    {

        fprintf(stderr,"Error: Invalid number of arguments\n");
        return 0;
    }

    struct jobs_node bg_procs[bg_proc_count];
    node itr = bg_proc_list_head;

    if(bg_proc_count==0)
    {
        printf("No background processes\n");
        return 1;
    }
    // printf("%d\n",bg_proc_count);
    for (int i = 0; i < bg_proc_count; i++)
    {
        while (itr->pid == -1)
        {
            // printf("%d\n", itr->pid);
            itr = itr->next;
        }

        bg_procs[i].proc_num = itr->proc_num;
        bg_procs[i].state = bg_proc_status(itr->pid);
        bg_procs[i].command = (char *)malloc(sizeof(MAX_SIZE));
        strcpy(bg_procs[i].command, itr->command);
        bg_procs[i].pid = itr->pid;
        itr = itr->next;
    }

    // printf("%d\n",bg_proc_count);
    qsort(bg_procs, bg_proc_count, sizeof(struct jobs_node), cmp_jobs);
    for (int i = 0; i < bg_proc_count; i++)
    {
        // printf("%c\n",bg_procs[i].state);
        // if(bg_procs[i].pid == -1)
        // {
        //     continue;
        // }
         if (bg_procs[i].state == 'T')
        {
            printf("%d : ", bg_procs[i].pid);
            printf("%s ", bg_procs[i].command);

            printf("- Stopped\n");
        }
         if (bg_procs[i].state == 'S' || bg_procs[i].state == 'R')
        {
            printf("%d : ", bg_procs[i].pid);
            printf("%s ", bg_procs[i].command);

            // printf("[%d] ", bg_procs[i].proc_num);
            printf("- Running\n");
            // printf("%s ", bg_procs[i].command);
        }

        else
        {
            // remove_node(bg_procs[i].pid);
            continue;
        }
    }

    return 1;
}

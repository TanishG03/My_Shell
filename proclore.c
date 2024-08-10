#include "headers.h"
#include "globals.h"


int proclore(char* token1,char *home_dir,char *cwd){
    pid_t pid;
    char* status;
    // char* memory;
    char* executable = (char*)malloc(sizeof(char*)*300);

        char *args[4096];
    int argCount = 0;
    
    while ((token1 = strtok(NULL, " \t\n")) != NULL) {
                    args[argCount] = token1;
                    argCount++;
    }
    if(argCount == 0) pid = shell_pid;
    else{
        pid = atoi(args[0]);
        
    }

    char proc_stat[1000];
    int memory;
    char exec_path[1000];
    pid_t pgpid;
    pid_t tgpid;
      pid_t process_group;
    char procFile[1000];
    sprintf(procFile, "/proc/%d/stat", pid);
    FILE *statFile = fopen(procFile, "r");

    if (statFile == NULL)
    {
        perror("Error: wrong pid");
        return 0;
    }


    for(int i=0;i<23;i++)
    {
        char temp[1000];
        fscanf(statFile, "%s", temp);
        if (i == 2)
        {
            strcpy(proc_stat, temp);
        }
        if (i == 4)
        {
            pgpid = atoi(temp);
        }
        if (i == 7)
        {
            tgpid = atoi(temp);
        }

        if (i == 8) 
        {
            process_group = atoi(temp); // Read process group from stat file
        }

        if (i == 22)
        {
            memory = atoi(temp);
        }

    }

    printf("pid : %d\n", pid);
    if (pgpid == tgpid)
    {
        printf("Process Status : %c+\n", proc_stat[0]);
    }
    else
    {
        printf("Process Status : %c\n", proc_stat[0]);
    }
        printf("Process Group ID : %d\n", process_group); // Print process group ID

    printf("Virtual memory : %d \n", memory);

    char exeFile[1000];
    sprintf(exeFile, "/proc/%d/exe", pid);
    FILE *execFile = fopen(exeFile, "r");

    int len = readlink(exeFile, exec_path, 1000);

    if (len < 0)
    {
        perror("Error");
        return 0;
    }

    exec_path[len] = '\0';

    if (strcmp(exec_path, home_dir) == 0)
    {
        printf("executable path : ~\n");
    }
    else if (strlen(exec_path) < strlen(home_dir) || strlen(exec_path) >= strlen(home_dir) && strstr(exec_path, home_dir) == NULL)
    {
        printf("executable path : %s\n", exec_path);
    }
    else
    {
        int bp = strlen(home_dir);
        char extension[1000];
        strcpy(extension, &exec_path[bp]);
        printf("executable path : ~%s\n", extension);
    }

    return 1;
}


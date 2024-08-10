#include "headers.h"
#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<string.h>



void prompt(char *og_home) {
    // Do not hardcode the prompt

        struct passwd *pw = getpwuid(getuid());  // Get user-related information
        char *username = pw->pw_name;
    // char *username = getenv("USER");
    char systemname[1024];
    char cwd[1024];

    char *color_reset = "\x1B[0m";   // Reset to default color
    char *color_green = "\x1B[32m";  // Green text color
    char *color_blue = "\x1B[34m"; 

    char *home = cwd;
    getcwd(cwd, sizeof(cwd));
    gethostname(systemname, sizeof(systemname));

    if (strcmp(og_home, cwd) == 0) {
        home = "~"; // Set home to ~ if they match
    }

    // If the current working directory starts with the original home path,
    // replace the original home path in cwd with ~ to maintain the formatting
    if (strncmp(cwd, og_home, strlen(og_home)) == 0) {
        char temp[1024];
        sprintf(temp, "~%s", cwd + strlen(og_home));
        strcpy(cwd, temp);
    }
    if(proc_time>2)
    {
        printf("<%s%s@%s%s:%s%s %lfs> ", color_green, username, color_blue, systemname, color_reset, cwd,proc_time);
        proc_time=0;
        // return;
    }
    else
    printf("<%s%s@%s%s:%s%s> ", color_green, username, color_blue, systemname, color_reset, cwd);
    // return;
}
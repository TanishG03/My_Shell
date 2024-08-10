#include"warp.h"
#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<string.h>

char prev_dir[1024] = "";


void get_parent_directory(const char *current_path, char *parent_path) {
    int len = strlen(current_path);
    
    // Find the last occurrence of '/'
    int last_slash = len - 1;
    while (last_slash >= 0 && current_path[last_slash] != '/') {
        last_slash--;
    }

    // Copy the parent directory path
    strncpy(parent_path, current_path, last_slash);
    parent_path[last_slash] = '\0';
}

int warp(char *token,char * cwd, char * og_home)
{
    char *args[4096];
    int argCount = 0;

    while ((token = strtok(NULL, " \t\n")) != NULL) {
                    args[argCount] = token;
                    argCount++;
    }
    // printf("%s\n",args[0]);
    // for(int i=0;i<argCount;i++){
    //     printf("%s\n",args[i]);
    // }
    
    if (argCount == 0) { // No arguments provided
        chdir(og_home);
        printf("Changed directory to home\n");
        return 1;
    }

char *purani_dir = malloc(1024);
    strcpy(purani_dir, cwd);

for (int i = 0; i < argCount; i++) {

        // printf("%s\n",cwd);
        // printf("%s\n",args[i]);

    if(strcmp(args[i],"..")==0){
    char parent_dir[1024];

    get_parent_directory(cwd, parent_dir);
    if (chdir(parent_dir) == 0) {
        // printf("Changed to parent directory: %s\n", parent_dir);
        strcpy(cwd, parent_dir);

    } else {
        perror("chdir");
    }
    }

   else if(strcmp(args[i],".")==0)
    {
        chdir(og_home);
        printf("Changed directory to home\n");
        strcpy(cwd, og_home);
    }

   else if(strcmp(args[0],"-")==0)
    {
          
        if (strlen(prev_dir) > 0) {
            if (chdir(prev_dir) == 0) {
                printf("Changed to previous directory: %s\n", prev_dir);
                strcpy(cwd, prev_dir);
            } else {
                perror("chdir");
                return 0;
            }
        } else {
            printf("No previous directory available\n");
            return 0;
        }
    

    }



else if (args[i][0] == '~') {
    char temp[100000];
    strcpy(temp, og_home);
    strcat(temp, args[i] + 1); // Skip the tilde character
    // printf("%s\n", temp);
    if (chdir(temp) == 0) {
        printf("Changed to parent directory: %s\n", temp);
        cwd=temp;
    } else {
        perror("chdir");
        return 0;
    }
}


else {
char temp[10000];
    strcpy(temp, cwd);
    strcat(temp, "/");
    strcat(temp, args[i]); // Use args[i] directly for concatenation
    // printf("Changed to directory: %s\n", args[i]);
    // chdir(temp);

        if (chdir(temp) == 0) {
        printf("Changed to directory: %s\n", temp);
                    strcpy(cwd, temp);
        
    } else {
        perror("chdir");
            strcpy(cwd, temp);
        return 0;
    }
 // Update cwd with the concatenated path

}


}

strcpy(prev_dir, purani_dir);
free(purani_dir);

return 1;
}
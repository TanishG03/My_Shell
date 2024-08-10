
#include "execute.h"
#include <ctype.h>
#define HISTORY_MAX_SIZE 15
#define MAX_ARGS 100

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_background(char *token) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        char *args[MAX_ARGS];
        int argCount = 0;

        char *arg = strtok(token, " ,\n\t");
        while (arg != NULL && argCount < MAX_ARGS - 1) {
            args[argCount] = arg;
            argCount++;
            arg = strtok(NULL, " ,\n\t");
        }
        args[argCount] = NULL; 
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    }
    printf("[%d] Background process started: %s\n", pid, token);
}

void execute_foreground(char *token) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        char *args[MAX_ARGS];
        int argCount = 0;


        char *arg = strtok(token, " ,\n\t");
        while (arg != NULL && argCount < MAX_ARGS - 1) {
            args[argCount] = arg;
            argCount++;
            arg = strtok(NULL, " ,\n\t");
        }
        args[argCount] = NULL; 

        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else {
        // In the parent process
        int status;
        waitpid(pid, &status, 0);
        printf("[%d] Foreground process %s exited\n", pid, WIFEXITED(status) ? "normally" : "abnormally");
    }
}

void execute_segments(char *input) {
    // printf("%s\n", input);
    while (isspace(*input)) {
        input++;
    }

    if (*input == '\0') {
        // Input is null or just whitespace
        return;
    }
    char *ampersand = strchr(input, '&');
    if (ampersand != NULL) {
        *ampersand = '\0'; // Terminate the left segment
        char *right_segment = ampersand + 1; // Move to the right segment
        // printf("%s\n", right_segment);
            // printf("%s\n", input);
        // execute_background(input);
        // // execute_foreground(right_segment);
        // execute_segments(right_segment); // Recurse on the right segment

        pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        char *args[MAX_ARGS];
        int argCount = 0;


        char *arg = strtok(input, " ,\n\t");
        while (arg != NULL && argCount < MAX_ARGS - 1) {
            args[argCount] = arg;
            argCount++;
            arg = strtok(NULL, " ,\n\t");
        }
        args[argCount] = NULL; 

        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else {
        // In the parent process
        // execute_segments(right_segment); // Recurse on the right segment
        int status;
        // waitpid(pid, &status, 0);

            execute_segments(right_segment); // Recurse on the right segment

        printf("[%d] Background process %s exited\n", pid, WIFEXITED(status) ? "normally" : "abnormally");
    }

    } else {

        char *args[MAX_ARGS];
        int argCount = 0;

        char *arg = strtok(input, " ,\n\t");
        while (arg != NULL && argCount < MAX_ARGS - 1) {
            args[argCount] = arg;
            argCount++;
            arg = strtok(NULL, " ,\n\t");
        }
        args[argCount] = NULL; 

        execvp(args[0], args);
        perror("execvp");
        exit(1);

    }


}



void execute (char history[HISTORY_MAX_SIZE][4096], int *history_count, char *input, char *og_home, char *cwd)
{
    
        const char semicolon_delim[] = ";\n";
        char *segment_tokens[100]; // Array to store segment tokens
        int num_segments = 0;



        char *input_copy = strdup(input);  // Create a copy of the input to avoid modifying it
        char *badi_copy= strdup(input);
        if(badi_copy[strlen(badi_copy)-1]=='\n')
        {
        badi_copy[strlen(badi_copy)-1]='\0';
        }
        char *saveptr1;  // Saveptr for strtok_r

        char *segment = strtok_r(input_copy, semicolon_delim, &saveptr1);

        while (segment != NULL && num_segments < 100) {
            // Trim leading whitespaces
            while (isspace(*segment)) {
                segment++;
            }

            // Trim trailing whitespaces
            char *end = segment + strlen(segment) - 1;
            while (end > segment && isspace(*end)) {
                end--;
            }
            *(end + 1) = '\0';

            segment_tokens[num_segments] = segment;
            num_segments++;

            segment = strtok_r(NULL, semicolon_delim, &saveptr1); // Move to the next segment
        }

        int history_added = 1;
        // loadHistoryFromFile(history, history_count);
        for (int i = 0; i < num_segments; i++) {
        char history_instance[10000];
        strcpy(history_instance,segment_tokens[i]);
        const char word_delim[] = " ,\n\t"; // Delimiters for tokenizing each segment
        char *token = strtok(segment_tokens[i], word_delim);

        if(token)
        {
            if(strcmp(token,"warp")==0)
            {
                getcwd(cwd, sizeof(cwd));
                // printf("%s\n",cwd);
                if(warp(token,cwd,og_home))
                {
                    // addToHistory(history, history_count, badi_copy);
                     int history_added = 1;
                }
                else{
                    printf("not added to history\n");}
            }

            else if(strcmp(token,"peek")==0)
            {
                
                if(peek(token,cwd,og_home))
                {
                     int history_added = 1;
                    // addToHistory(history, history_count, badi_copy);
                }
                else{
                    printf("not added to history\n");}

            }

            else if(strcmp(token,"pastevents")==0)
            {
                history_added = 0;
                init_history(token,history, history_count,og_home,cwd);
                // printHistory(history, *history_count);
            }

            else if(strcmp(token,"warp")!=0&&strcmp(token,"peek")!=0&&strcmp(token,"pastevents")!=0)
            {
                // printf("%s\n",history_instance);
                execute_segments(history_instance);
            }
        }
    }
    if(history_added)
    {
        // printf("%s\n",badi_copy);
        addToHistory(history, history_count, badi_copy);
    }
    
    saveHistoryToFile(history, *history_count);

    free(input_copy); 


}

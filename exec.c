
#include "execute.h"
#include <ctype.h>
#define HISTORY_MAX_SIZE 15
#define MAX_ARGS 100
#include "headers.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<time.h>
#include "proclore.h"


typedef struct Alias {
    char *name;
    char *command;
} Alias;

Alias aliases[100];  // Assuming a maximum of 100 aliases
int alias_count = 0;

void load_aliases(const char *rc_file_path) {
    FILE *file = fopen(rc_file_path, "r");
    if (!file) {
        perror("Failed to open rc file");
        return;
    }

    char line[4096];
    while (fgets(line, sizeof(line), file)) {
        char *name = strtok(line, "=");
        char *command = strtok(NULL, "\n");

        if (name && command) {
            aliases[alias_count].name = strdup(name);
            aliases[alias_count].command = strdup(command);
            alias_count++;
        }
    }

    fclose(file);
}


char* get_alias_command(const char *token) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(aliases[i].name, token) == 0) {
            return aliases[i].command;
        }
    }
    return NULL;
}

void replace_aliases(char *token) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(token, aliases[i].name) == 0) {
            strcpy(token, aliases[i].command);
            break;
        }
    }
}

void replace_aliases_0(char **arguments, int *arguementCount) {
    for (int i = 0; i < *arguementCount; i++) {
        char *alias_command = get_alias_command(arguments[i]);
        if (alias_command) {
            arguments[i] = alias_command;
        }
    }
}

// bg_process *head = NULL;
// bg_process *tail = NULL;
// int bg_process_size = 0;


// typedef struct {
//     pid_t pid_process;
//     char *process_name;
// } bg_process;

bg_process bg_processes[MAX_BG_PROCESSES];
int bg_process_count = 0;

// void sigchld_handler(int signo) {
//     int status;
//     pid_t pid;
    
//     for (int i = 0; i < bg_process_count; i++) {
//         pid = waitpid(bg_processes[i].pid_process, &status, WNOHANG);
//         if (pid > 0) {
//             if (WIFEXITED(status)) {
//                 printf("Background process %s exited normally with pid [%d]\n", bg_processes[i].process_name, bg_processes[i].pid_process);
//             }
//             // Remove the process from the array
//             bg_processes[i] = bg_processes[bg_process_count - 1];
//             bg_process_count--;
//             i--; // Re-check the current index in the next iteration
//         }
//     }
// }




// void execute_background(char *token) {

//         char *args[MAX_ARGS];
//         int argCount = 0;

//         char *arg = strtok(token, " ,\n\t");
//         while (arg != NULL && argCount < MAX_ARGS - 1) {
//             args[argCount] = arg;
//             argCount++;
//             arg = strtok(NULL, " ,\n\t");
//         }
//         args[argCount] = NULL; 
//          pid_t child_pid;
//     int child_status;
//     child_pid = fork();
//     if (child_pid == 0)
//     {
//             setsid(); // Create a new session and detach from the terminal
//     // Redirect stdin, stdout, and stderr to /dev/null
//     int devnull = open("/dev/null", O_RDWR);
//     dup2(devnull, STDIN_FILENO);
//     dup2(devnull, STDOUT_FILENO);
//     dup2(devnull, STDERR_FILENO);
//     close(devnull);

//     if(strcmp(args[0],"vim")==0)
//     {
//             char *vim_args[MAX_ARGS];
//     vim_args[0] = "vim";
//     vim_args[1] = "-y";
//     vim_args[2] = NULL;

//     execvp("vim", vim_args);
    
//     // If execvp fails, handle the error
//     perror("execvp");
//     // exit(1);
//     }
//         execvp(args[0], args);
//         exec_success=-1;
//         printf("Unknown command\n");
//         // exit(0);
//     }
//     else
//     {

//     add_node(child_pid);
//     // printf("%d\n",bg_proc_count);
//     // printf("hi\n");
 
//         // node itr = bg_proc_list_head;
//         // // while(itr->next!=NULL)
//         // // {
//         // //     printf("[%d] %s [%d]\n",itr->proc_num,itr->command,itr->pid);
//         // //     itr=itr->next;
//         // // }
//         // while (itr->pid != child_pid)
//         // {
//         //     itr = itr->next;
//         // }

//         // bg_process new_bg_process;
//         // new_bg_process.pid_process = child_pid;
//         // new_bg_process.process_name = strdup(token);
        
//         // if (bg_process_count < MAX_BG_PROCESSES) {
//         //     bg_processes[bg_process_count] = new_bg_process;
//         //     bg_process_count++;

//         //     printf("[%d] Background process started: %s\n", child_pid, token);
//         // } else {
//         //     printf("Maximum number of background processes reached\n");
//         // }
        
//     }

// }

void execute_background(char * token)
{

            char *args[MAX_ARGS];
        int argCount = 0;

        // char *arg = strtok(token, " ,\n\t");
        // while (arg != NULL && argCount < MAX_ARGS - 1) {
        //     args[argCount] = arg;
        //     argCount++;
        //     arg = strtok(NULL, " ,\n\t");
        // }
        // args[argCount] = NULL; 

        // Initialize a buffer to store the concatenated string
char concatenated[1000];  // You should define MAX_LENGTH according to your needs
strcpy(concatenated, "");  // Initialize the buffer as an empty string

char *arg = strtok(token, " ,\n\t");
while (arg != NULL && argCount < MAX_ARGS - 1) {
    args[argCount] = arg;
    argCount++;

    // Concatenate the argument with a space
    strcat(concatenated, arg);
    if (argCount < MAX_ARGS - 1) {
        strcat(concatenated, " ");  // Add a space unless it's the last argument
    }

    arg = strtok(NULL, " ,\n\t");
}

args[argCount] = NULL;

    pid_t pid = fork();

    if (pid == 0)
    {
        setpgid(pid, pid);

        if (execvp(args[0], args) < 0)
        {

            perror("Error");
            kill(pid, SIGTERM);
            return;
        }
    }
    else if (pid < 0)
    {
        perror("Error");
        return;
    }
    else
    {
        // add_node(pid);
        // node itr = bg_proc_list_head;

        // while (itr->pid != pid)
        // {
        //     itr = itr->next;
        // }

        // printf("[%d] %d\n", itr->proc_num, itr->pid);

            node itr = bg_proc_list_head;
    bool found_empty = false;

    if (bg_proc_count == 0) {
        itr->pid = pid;
        bg_proc_count++;

        // strcpy(itr->command, args[0]);
                    strcpy(itr->command, concatenated);

        // for (int i = 1; i < argCount; i++) {
        //     strcat(itr->command, " ");
        //     // if (strcmp(arguments[i], "&") != 0) {
        //         strcat(itr->command, args[i]);
        //     // }
        // }
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

            strcpy(head->command, concatenated);
        //     while(strtok(token, " \n")!=NULL)
        // {
        //     strcat(itr->command, " ");
        //     strcat(itr->command, token);
        //     token=strtok(NULL, " \n");
        // }
        //  strcpy(itr->command, args[0]);
        
        // for (int i = 1; i < argCount; i++) {
        //     strcat(itr->command, " ");
        //     // if (strcmp(arguments[i], "&") != 0) {
        //         strcat(itr->command, args[i]);
        //     // }
        // }
            // for (int i = 1; i < arguementCount; i++) {
            //     strcat(head->command, " ");
            //     strcat(head->command, arguments[i]);
            // }

            itr->next = head;
        } else {
            itr->pid = pid;
            bg_proc_count++;
            strcpy(itr->command, concatenated);

        //     strcpy(itr->command, token);
        //     while(strtok(token, " \n")!=NULL)
        // {
        //     strcat(itr->command, " ");
        //     strcat(itr->command, token);
        //     token=strtok(NULL, " \n");
        // }

        //  strcpy(itr->command, args[0]);
        
        // for (int i = 1; i < argCount; i++) {
        //     strcat(itr->command, " ");
        //     // if (strcmp(arguments[i], "&") != 0) {
        //         strcat(itr->command, args[i]);
        //     // }
        // }
            // for (int i = 1; i < arguementCount; i++) {
            //     strcat(itr->command, " ");
            //     // if (strcmp(arguments[i], "&") != 0) {
            //         strcat(itr->command, arguments[i]);
            //     // }
            // }
        }
    }
    }
}

void execute_foreground(char *token) {


        char *args[MAX_ARGS];
        int argCount = 0;
    int insideQuotes = 0; 
    
            int doubleQuoteCount = 0;
        int singleQuoteCount = 0;
        for (int i = 0; i < strlen(token); i++) {
            if (token[i] == '\"') {
                doubleQuoteCount++;
            } else if (token[i] == '\'') {
                singleQuoteCount++;
            }
        }

        if(doubleQuoteCount==2||singleQuoteCount==2)
        {
                char* arg = strtok(token, "\"'\n");
            while (arg != NULL && argCount < MAX_ARGS - 1) {
                // printf("%d\n",strlen(arg));

            if(arg[strlen(arg)-1]==' ')
            {
                // printf("YES\n");
                arg[strlen(arg)-1]='\0';
            }

            if(strlen(arg)>0)
            {
                            args[argCount] = arg;
        
                    argCount++;
            }


        arg = strtok(NULL, "\"'\n");
    }
    // printf("%d\n",argCount);
        }
        
        else{
                    char *arg = strtok(token, " ,\n\t");
        while (arg != NULL && argCount < MAX_ARGS - 1) {
 
            args[argCount] = arg;
            argCount++;
            arg = strtok(NULL, " ,\n\t");
        }
   
        }


        // for(int i=0;i<argCount;i++)
        // {
        //     printf("%s %d\n",args[i],strlen(args[i]));
        // }
        // argCount++;
        args[argCount] = NULL; 


    pid_t child_pid;
    int child_status;
    proc_time=0;
        struct timespec start_time, end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

     child_pid = fork();
    if (child_pid == 0)
    {

        setpgid(child_pid, child_pid);

        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
   

    // execvp(args[0], args);

        if (execvp(args[0], args) < 0)
        {
            perror("Error");
            printf("%s is an Unknown command\n",args[0]);

            kill(child_pid, SIGTERM);
            exec_success = -1;  // Indicate failure

            return;
        }

        signal(SIGINT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        
  
    }
    else
    {
        int status;
            // add_node(child_pid);
        latest_pid=child_pid;
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);

        tcsetpgrp(STDIN_FILENO, child_pid);
        waitpid(child_pid,&status,WUNTRACED);

        tcsetpgrp(STDIN_FILENO, getpgid(0));

        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);

        if (WIFSTOPPED(status))
        {
            // for(int i=0;i<arguementCount;i++)
            // {
            //     printf("%s\n",arguments[i]);
            // }
            add_node(child_pid);
        }

        
        clock_gettime(CLOCK_REALTIME, &end_time);
        double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                              (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

        if (elapsed_time > 2.0) {
            // printf("%s : %.0fs\n", token, elapsed_time);
            proc_time=elapsed_time;
        }

        // remove_node(child_pid);

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

        execute_background(input);
        execute_segments(right_segment); // Recurse on the right segment

    } else {
        execute_foreground(input);
        // printf("hi\n");
    }


}


void execute(char history[HISTORY_MAX_SIZE][4096], int *history_count, char *input, char *og_home, char *cwd) {
    // Load aliases from the rc file
    load_aliases(".myshrc");

    const char semicolon_delim[] = ";\n";
    char *segment_tokens[100]; // Array to store segment tokens
    int num_segments = 0;

    char *input_copy = strdup(input);  // Create a copy of the input to avoid modifying it
    char *badi_copy= strdup(input);
    if (badi_copy[strlen(badi_copy)-1] == '\n') {
        badi_copy[strlen(badi_copy)-1] = '\0';
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

    int history_added = 0;

    input_fd = STDIN_FILENO;
    output_fd = STDOUT_FILENO;
    int all_segs_exec = 1;
    int segment_status[100] = {0}; // Initialize all elements to 0

    for (int i = 0; i < num_segments; i++) {
        int count_pipes = 0;
        for (int j = 0; j < strlen(segment_tokens[i]); j++) {
            if (segment_tokens[i][j] == '|') {
                count_pipes++;
            }
        }
        if (count_pipes > 0) {
            piping(segment_tokens[i], history, history_count, og_home, cwd);
            char history_instance[10000];
            strcpy(history_instance, segment_tokens[i]);
        } else {
            char history_instance[10000];
            strcpy(history_instance, segment_tokens[i]);
            char boken[1000];
            strcpy(boken, segment_tokens[i]);
            char copy[1000];

            strcpy(copy, segment_tokens[i]);
            arguementCount = 0; // Reset arguementCount for this iteration
            char *lund = strtok(copy, " \n");

            while (lund != NULL && arguementCount < MAX_ARGS) {
                arguments[arguementCount] = lund; // Store the token in the arguments array
                arguementCount++;
                lund = strtok(NULL, " \n");
            }

            // Replace command tokens with aliases
            replace_aliases_0(arguments, &arguementCount);
            // for(int i=0;i<arguementCount;i++)
            // {
            //     printf("%s\n",arguments[i]);
            // }
            // printf("%s\n", arguments[0]);

            const char word_delim[] = " ,\n\t"; // Delimiters for tokenizing each segment
            char *token = strtok(segment_tokens[i], word_delim);
            // printf("%s\n", token);  
                            replace_aliases(token);
                            replace_aliases(boken);


            if (token) {
                if (redirection(token, history_instance, history, history_count) == 1) {
                    continue;
                }

                char *token = strtok(boken, word_delim);
                printf("%s\n", token);

                if (strcmp(token, "warp") == 0) {
                    getcwd(cwd, sizeof(cwd));
                    if (warp(token, cwd, og_home)) {
                        history_added = 1;
                        exec_success = 1;
                    } else {
                        exec_success = -1;
                    }
                } else if (strcmp(token, "peek") == 0) {
                    if (peek(token, cwd, og_home)) {
                        history_added = 1;
                        exec_success = 1;
                    } else {
                        exec_success = -1;
                    }
                } else if (strcmp(token, "pastevents") == 0) {
                    history_added = -1;
                    init_history(token, history, history_count, og_home, cwd);
                } else if (strcmp(token, "proclore") == 0) {
                    if (proclore(token, og_home, cwd)) {
                        history_added = 1;
                        exec_success = 1;
                    } else {
                        exec_success = -1;
                    }
                } else if (strcmp(token, "seek") == 0) {
                    if (seek(token, og_home, cwd)) {
                        history_added = 1;
                        exec_success = 1;
                    } else {
                        exec_success = -1;
                    }
                } else if (strcmp(token, "activities") == 0) {
                    if (activities(token)) {
                        history_added = 1;
                        exec_success = 1;
                    } else {
                        exec_success = -1;
                    }
                } else if (strcmp(token, "bg") == 0) {
                    if (bg()) {
                        history_added = 1;
                        exec_success = 1;
                    } else {
                        exec_success = -1;
                    }
                } else if (strcmp(token, "fg") == 0) {
                    if (fg()) {
                        history_added = 1;
                        exec_success = 1;
                    } else {
                        exec_success = -1;
                    }
                } else if (strcmp(token, "ping") == 0) {
                    if (ping()) {
                        history_added = 1;
                        exec_success = 1;
                    } else {
                        exec_success = -1;
                    }
                } else if (strcmp(token, "neonate") == 0) {
                    if (neonate(og_home, history, history_count) == 0) {
                        exec_success = -1;
                        continue;
                    }
                    history_added = 1;
                    exec_success = 1;
                } else if (strcmp(token, "iMan") == 0) {
                    if (man(token)) {
                        history_added = 1;
                        exec_success = 1;
                    } else {
                        exec_success = -1;
                    }
                } else {
                    execute_segments(history_instance);
                }
            }
        }

        if ((history_added || exec_success == 0) && history_added != -1) {
            segment_status[i] = 1; // Set the status for this segment to 1
        } else {
            segment_status[i] = 0; // Set the status for this segment to 0
        }
        history_added = 0;
        exec_success = 0;
    }

    int all_segments_successful = 1;
    for (int i = 0; i < num_segments; i++) {
        if (segment_status[i] == 0) {
            all_segments_successful = 0;
            break; // If any segment returned 0, exit the loop
        }
    }

    if (all_segments_successful) {
        addToHistory(history, history_count, badi_copy);
    } else {
        int i = 0;
        char *token = strtok(badi_copy, semicolon_delim);
        while (token != NULL) {
            char *trimmed_token = token;
            while (isspace(*trimmed_token)) {
                trimmed_token++;
            }
            char *end = trimmed_token + strlen(trimmed_token) - 1;
            while (end > trimmed_token && isspace(*end)) {
                end--;
            }
            *(end + 1) = '\0';

            // Check if this segment was successful (1) and print it if so
            if (segment_status[i] == 1) {
                addToHistory(history, history_count, token);
            }
            token = strtok(NULL, semicolon_delim);
            i++;
        }
    }

    saveHistoryToFile(history, *history_count);

    free(input_copy);
}

#include "execute.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define HISTORY_MAX_SIZE 15
#define MAX_ARGS 100

bg_process *head = NULL;
bg_process *tail = NULL;
int bg_process_size = 0;


void sigchld_handler(int signo) {
    int status;
    int exit_flag;
    pid_t pid;
    // printf("SIGCHLD received\n");
    bg_process *ptr = head->next;
    bg_process *prev = head;


    // while (ptr != NULL) {
    //     if (waitpid(ptr->pid_process, &status, WNOHANG) != 0) {
    //         if (WIFEXITED(status)) {
    //             exit_flag = WEXITSTATUS(status);
    //         }
    //             printf("Background process %s exited normally with status %d\n", ptr->process_name, exit_flag);
    //             // prev->next = ptr->next;
    //             bg_process_size--;
    //             prev->next=ptr->next;
    //             ptr=ptr->next;
    //             if (ptr == tail) {
    //                 tail = prev;
    //             }
    //         }
    //      else {

    //             prev->next=ptr->next;
    //             ptr=ptr->next;
    //         // prev = ptr;
    //     }
    //     // ptr = ptr->next;
    // }

    while (ptr != NULL) {
    if (waitpid(ptr->pid_process, &status, WNOHANG) != 0) {
        if (WIFEXITED(status)) {
            exit_flag = WEXITSTATUS(status);
        }
        printf("Background process %s exited normally with status %d\n", ptr->process_name, exit_flag);
        bg_process_size--;
        prev->next = ptr->next;
        bg_process *temp = ptr;
        ptr = ptr->next;
        free(temp);  // Free memory for the exited process node
        if (ptr == NULL) {
            tail = prev;
        }
    } else {
        prev = ptr;
        ptr = ptr->next;
    }
}

}


void execute_background(char *token) {

        char *args[MAX_ARGS];
        int argCount = 0;

        char *arg = strtok(token, " ,\n\t");
        while (arg != NULL && argCount < MAX_ARGS - 1) {
            args[argCount] = arg;
            argCount++;
            arg = strtok(NULL, " ,\n\t");
        }
        args[argCount] = NULL; 
         pid_t child_pid;
    int child_status;
    child_pid = fork();
    if (child_pid == 0)
    {
        execvp(args[0], args);
        printf("Unknown command\n");
        exit(0);
    }
    else
    {

        // printf("%d\n", child_pid);
        // printf("[%d] Background process started: %s\n", child_pid, token);
        // // Add process to tracking
        // bg_processes[num_bg_processes].pid = child_pid;
        // bg_processes[num_bg_processes].command = strdup(token);
        // num_bg_processes++;

        // bg_process *new_bg_process = (bg_process *)malloc(sizeof(bg_process));
        // new_bg_process->pid_process = child_pid;
        // new_bg_process->process_name = strdup(token);
        // new_bg_process->next = NULL;

        // if (head == NULL) {
        //     head = new_bg_process;
        //     head->next= new_bg_process;
        //     tail = new_bg_process;
        // } else {
        //     tail->next = new_bg_process;
        //     tail = new_bg_process;
        // }

        // bg_process_size++;

        bg_process *new_bg_process = (bg_process *)malloc(sizeof(bg_process));
        new_bg_process->pid_process = child_pid;
        new_bg_process->process_name = strdup(token);
        new_bg_process->next = NULL;

        // Use a dummy node as the head for the linked list
        if (head == NULL) {
            head = (bg_process *)malloc(sizeof(bg_process));  // Create a dummy node
            head->next = NULL;
            tail = head;
        }

        tail->next = new_bg_process;
        tail = new_bg_process;

        bg_process_size++;

        printf("[%d] Background process started: %s\n", child_pid, token);
        
    }

}

void execute_foreground(char *token) {


        char *args[MAX_ARGS];
        int argCount = 0;

        char *arg = strtok(token, " ,\n\t");
        while (arg != NULL && argCount < MAX_ARGS - 1) {
            args[argCount] = arg;
            argCount++;
            arg = strtok(NULL, " ,\n\t");
        }
        args[argCount] = NULL; 


    pid_t child_pid;
    int child_status;

        struct timespec start_time, end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

     child_pid = fork();
    if (child_pid == 0)
    {
        execvp(args[0], args);
        printf("Unknown command\n");
        exit(0);
    }
    else
    {

        waitpid(child_pid,&child_status,WCONTINUED|WUNTRACED);
        
        clock_gettime(CLOCK_REALTIME, &end_time);
        double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                              (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

        if (elapsed_time > 2.0) {
            printf("%s : %.0fs\n", token, elapsed_time);
        }

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
        execute_background(input);
        // // execute_foreground(right_segment);
        execute_segments(right_segment); // Recurse on the right segment

    } else {

        execute_foreground(input);

    }


}

void execute(char history[HISTORY_MAX_SIZE][4096], int *history_count, char *input, char *og_home, char *cwd) {

    const char semicolon_delim[] = ";\n";
    char *segment_tokens[100]; // Array to store segment tokens
    int num_segments = 0;

    char *input_copy = strdup(input); // Create a copy of the input to avoid modifying it
    char *badi_copy = strdup(input);
    if (badi_copy[strlen(badi_copy) - 1] == '\n') {
        badi_copy[strlen(badi_copy) - 1] = '\0';
    }
    char *saveptr1; // Saveptr for strtok_r

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
        strcpy(history_instance, segment_tokens[i]);
        const char word_delim[] = " ,\n\t"; // Delimiters for tokenizing each segment
        char *token = strtok(segment_tokens[i], word_delim);

        if (token) {
            if (strcmp(token, "warp") == 0) {
                // ... (existing code for warp)
            } else if (strcmp(token, "peek") == 0) {
                // ... (existing code for peek)
            } else if (strcmp(token, "pastevents") == 0) {
                // ... (existing code for pastevents)
            } else {
                // Execute system commands using execvp
            // char *args[MAX_ARGS];
            // int argCount = 0;
            // args[0]=token;
            // argCount++;
            // // Tokenize the command and arguments
            // char *arg = strtok(NULL, word_delim);
            // while (arg != NULL && argCount < MAX_ARGS - 1) {
            //     args[argCount] = arg;
            //     argCount++;
            //     arg = strtok(NULL, word_delim);
            // }
            // args[argCount] = NULL; // Terminate the array with NULL

            // pid_t pid = fork();

            // if (pid < 0) {
            //     perror("fork");
            // } else if (pid == 0) { // Child process
            //     // Set up time measurement
            //     clock_t start_time = clock();

            //     execvp(args[0], args);
            //     perror("execvp"); // This line is reached only if execvp fails
            //     exit(1);
            // } else { // Parent process
            //     clock_t start_time = clock();

            //     // Check if the command is a background command (ends with "&")
            //     int is_background = 0;
            //     size_t token_len = strlen(token);
            //     if (token_len > 0 && token[token_len - 1] == '&') {
            //         is_background = 1;
            //         token[token_len - 1] = '\0'; // Remove the "&" from the token
            //     }

            //     if (!is_background) {
            //         // Foreground process: wait for child process to complete
            //         int status;
            //         waitpid(pid, &status, 0);

            //         // Calculate time taken by the foreground process
            //         clock_t end_time = clock();
            //         double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            //         if (elapsed_time > 2.0) {
            //             printf("%s : %.0fs\n", token, elapsed_time);
            //         }
            //     } else {
            //         // Background process: print its PID and continue
            //         printf("%s\n", token);
            //     }

            int is_background = 0;
        size_t token_len = strlen(token);
        if (token_len > 0 && token[token_len - 1] == '&') {
            is_background = 1;
            token[token_len - 1] = '\0'; // Remove the "&" from the token
        }

        // Execute system commands using execvp
        char *args[MAX_ARGS];
        int argCount = 0;
        args[argCount] = token;
        argCount++;

        // Tokenize the command and arguments
        char *arg = strtok(NULL, " ,\n\t");
        while (arg != NULL && argCount < MAX_ARGS - 1) {
            args[argCount] = arg;
            argCount++;
            arg = strtok(NULL, " ,\n\t");
        }
        args[argCount] = NULL; // Terminate the array with NULL

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) { // Child process
            // Set up time measurement
            clock_t start_time = clock();

            execvp(args[0], args);
            perror("execvp"); // This line is reached only if execvp fails
            exit(1);
        } else { // Parent process
            clock_t start_time = clock();

            if (!is_background) {
                // Foreground process: wait for child process to complete
                int status;
                waitpid(pid, &status, 0);

                // Calculate time taken by the foreground process
                clock_t end_time = clock();
                double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
                if (elapsed_time > 2.0) {
                    printf("%s : %.0fs\n", token, elapsed_time);
                }
            } else {
                // Background process: print its PID and continue
                printf("[%d]\n", pid);
            }
            }
        }
    }

    if (history_added) {
        // printf("%s\n",badi_copy);
        addToHistory(history, history_count, badi_copy);
    }

    saveHistoryToFile(history, *history_count);

    free(input_copy);
}
}

// #include "execute.h"
// #include <ctype.h>
// #define HISTORY_MAX_SIZE 15


// void execute (char history[HISTORY_MAX_SIZE][4096], int *history_count, char *input, char *og_home, char *cwd)
// {
    
//         const char semicolon_delim[] = ";\n";
//         char *segment_tokens[100]; // Array to store segment tokens
//         int num_segments = 0;



//         char *input_copy = strdup(input);  // Create a copy of the input to avoid modifying it
//         char *badi_copy= strdup(input);
//         if(badi_copy[strlen(badi_copy)-1]=='\n')
//         {
//         badi_copy[strlen(badi_copy)-1]='\0';
//         }
//         char *saveptr1;  // Saveptr for strtok_r

//         char *segment = strtok_r(input_copy, semicolon_delim, &saveptr1);

//         while (segment != NULL && num_segments < 100) {
//             // Trim leading whitespaces
//             while (isspace(*segment)) {
//                 segment++;
//             }

//             // Trim trailing whitespaces
//             char *end = segment + strlen(segment) - 1;
//             while (end > segment && isspace(*end)) {
//                 end--;
//             }
//             *(end + 1) = '\0';

//             segment_tokens[num_segments] = segment;
//             num_segments++;

//             segment = strtok_r(NULL, semicolon_delim, &saveptr1); // Move to the next segment
//         }

//          int history_added = 1;
//         // loadHistoryFromFile(history, history_count);
//         for (int i = 0; i < num_segments; i++) {
//         char history_instance[10000];
//         strcpy(history_instance,segment_tokens[i]);
//         const char word_delim[] = " ,\n\t"; // Delimiters for tokenizing each segment
//         char *token = strtok(segment_tokens[i], word_delim);

//         if(token)
//         {
//             if(strcmp(token,"warp")==0)
//             {
//                 getcwd(cwd, sizeof(cwd));
//                 // printf("%s\n",cwd);
//                 if(warp(token,cwd,og_home))
//                 {
//                     // addToHistory(history, history_count, badi_copy);
//                      int history_added = 1;
//                 }
//                 else{
//                     printf("not added to history\n");}
//             }

//             if(strcmp(token,"peek")==0)
//             {
                
//                 if(peek(token,cwd,og_home))
//                 {
//                      int history_added = 1;
//                     // addToHistory(history, history_count, badi_copy);
//                 }
//                 else{
//                     printf("not added to history\n");}

//             }

//             if(strcmp(token,"pastevents")==0)
//             {
//                 history_added = 0;
//                 init_history(token,history, history_count,og_home,cwd);
//                 // printHistory(history, *history_count);
//             }

//             else
//             {
                
//             }
//         }
//     }
//     if(history_added)
//     {
//         // printf("%s\n",badi_copy);
//         addToHistory(history, history_count, badi_copy);
//     }
    
//     saveHistoryToFile(history, *history_count);

//     free(input_copy); 


// }

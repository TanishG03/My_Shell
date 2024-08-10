#include "headers.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 100

void execute_command(char* args[], int input_fd, int output_fd,char history[15][4096], int *history_count) {

       char cwd[1000];
    getcwd(cwd, sizeof(cwd));
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        pipe_execute(myHome, cwd, args, history, history_count);
        // execvp(args[0], args);
        // perror("execvp");
        // exit(EXIT_FAILURE);
    } else { // Parent process
        wait(NULL);
    }
}




int redirection(char *command, char *input,char history[15][4096], int *history_count) {


    // int input_fd = STDIN_FILENO;
    // int output_fd = STDOUT_FILENO;
    int append_fd = STDOUT_FILENO;

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }

    // Handle input/output redirection
    char* input_redirection = strstr(input, "<");
    char* output_redirection = strstr(input, ">");
    char *append_redirection = strstr(input, ">>");

    if (!input_redirection && !output_redirection && !append_redirection) {
        return 0;
    }  if (input_redirection) {
        *input_redirection = '\0'; // Null-terminate the command part
        char* input_file = input_redirection + 1; // Get the input file name
        char *input_token = strtok(input_file, " \t\n");
        if (input_token != NULL) {
            input_fd = open(input_token, O_RDONLY);
            if (input_fd == -1) {
                perror("open");
                input_fd = STDIN_FILENO; // Reset to stdin on error
                return 0;
            }
        }
    }  if (append_redirection) {
        *append_redirection = '\0';
        char* output_file = append_redirection + 2;

        char *output_token = strtok(output_file, " \t\n");
        if (output_token != NULL) {
            append_fd = open(output_token, O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (append_fd == -1) {
                perror("open");
                output_fd = STDOUT_FILENO;
                return 0;
            }
        }
    }  else if (output_redirection) {
        *output_redirection = '\0';
        char* output_file = output_redirection + 1;

        char *output_token = strtok(output_file, " \t\n");
        if (output_token != NULL) {
            output_fd = open(output_token, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (output_fd == -1) {
                perror("open");
                output_fd = STDOUT_FILENO;
                return 0;
            }
        }
    }

        char* token = strtok(input, " \t\n");
        int arg_count = 0;
        char* args[4096];
        while (token != NULL) {
            args[arg_count++] = token;
            token = strtok(NULL, " \t\n");
        }
        args[arg_count] = NULL;

    if (arg_count > 0) {
        if (append_fd != STDOUT_FILENO) {
            execute_command(args, input_fd, append_fd,history,history_count);
        } else {
            execute_command(args, input_fd, output_fd,  history,history_count);
        }
    }
        //     if (arg_count > 0) {
        //     execute_command(args, input_fd, output_fd);
        // }
    // input_fd = STDIN_FILENO;
    // output_fd = STDOUT_FILENO;
    // append_fd = STDOUT_FILENO;

    return 1;
}
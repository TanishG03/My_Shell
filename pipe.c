#include "headers.h"
#include "pipe.h"
#include "pipe_execute.h"
#include <fcntl.h>

#include <stdbool.h>


int piping(char *segment_token, char history[HISTORY_MAX_SIZE][4096], int *history_count, char *og_home, char *cwd) {

    char * input=segment_token;

    int input_fd = STDIN_FILENO;
        int output_fd = STDOUT_FILENO;

        // char *token = strtok(input, " \t\n");
        // char * commands[4096];

        

        int arg_count = 0;
        int append_output = 0;
        // char token[1000];
        // strcpy(token,input);

        char *args[MAX_ARGS];
        int argCount = 0;
    // int insideQuotes = 0; 
    char * token= strtok(input, " \t\n");
    args[argCount] = token;
    argCount++;
    int tokenCount = 0;

    int readIdx=0;
        bool insideQuotes = false;

  char **tokens = NULL;

    while (input[readIdx] != '\0') {
        if (input[readIdx] == '"' || input[readIdx] == '\'') {
            insideQuotes = !insideQuotes;
        }

        if ((input[readIdx] == ' ' && !insideQuotes) || input[readIdx + 1] == '\0') {
            // End of a token, add it to the array of tokens.
            if (input[readIdx + 1] == '\0') {
                readIdx++; // Include the last character in the token
            }
            input[readIdx] = '\0';
            if (tokenCount == 0) {
                tokens = (char **)malloc(sizeof(char *));
            } else {
                tokens = (char **)realloc(tokens, (tokenCount + 1) * sizeof(char *));
            }
            // tokens[tokenCount] = &input[readIdx + 1];
            
            // tokenCount++;

            char *tokenStart = &input[readIdx + 1];
            if (tokenStart[0] == '"' && tokenStart[strlen(tokenStart) - 1] == '"' || tokenStart[0] == '\'' && tokenStart[strlen(tokenStart) - 1] == '\'') {
                tokenStart[strlen(tokenStart) - 1] = '\0';
                tokenStart++; // Move t
        }
                    tokens[tokenCount] = tokenStart;
            tokenCount++;
        }
        readIdx++;
    }

    for (int i = 0; i < tokenCount; i++) {
    char *token = tokens[i];

    // Check if the token has double quotes at the beginning and end, and remove them.
    if (token[0] == '"' && token[strlen(token) - 1] == '"' || token[0] == '\'' && token[strlen(token) - 1] == '\'') {
        token[strlen(token) - 1] = '\0';
        args[argCount] = &token[1]; // Move the pointer to exclude the first double quote.
    } else {
        args[argCount] = token; // No quotes, just copy the token.
    }

    if (strcmp(args[argCount], " ") == 0) {
        continue;
    }

    argCount++;
}


     for (int i = 0; i < argCount-1; i++) {
        char *token = args[i];

        if (strcmp(token, "<") == 0) {
            i++;
            if (i < argCount) {
                input_fd = open(args[i], O_RDONLY);
                if (input_fd == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Missing input file after '<'\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(token, ">") == 0) {
            i++;
            if (i < argCount) {
                output_fd = open(args[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (output_fd == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Missing output file after '>'\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(token, ">>") == 0) {
            i++;
            if (i < argCount) {
                output_fd = open(args[i], O_WRONLY | O_CREAT | O_APPEND, 0666);
                if (output_fd == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                append_output = 1;
            } else {
                fprintf(stderr, "Missing output file after '>>'\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(token, "|") == 0) {
            args[arg_count] = NULL;
            int pipe_fd[2];
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        // input_fd = pipe_fd[0];

            output_fd = pipe_fd[1];
            execute_command(args, input_fd, output_fd,history,history_count);
            close(pipe_fd[1]);
            input_fd = pipe_fd[0];
            arg_count = 0;
        } else {
            args[arg_count] = token;
            arg_count++;
        }
    }
    args[arg_count] = NULL;

    if (arg_count > 0) {
        execute_command(args, input_fd, output_fd,history,history_count);
    }

    if (append_output) {
        close(output_fd);
    }

    if (input_fd != STDIN_FILENO) {
        close(input_fd);
    }



    return 0;
}

    // int count_pipe_commands = 0;
    // char *pipe_commands[MAX_SIZE];

    // char *tokenize = strtok(segment_token, "|");
    // while (tokenize != NULL) {
    //     pipe_commands[count_pipe_commands++] = tokenize;
    //     tokenize = strtok(NULL, "|");
    // }

    // // Check for invalid use of pipe
    // if (count_pipe_commands < 2) {
    //     printf("Invalid use of pipe\n");
    //     return -1;
    // }

    // int prev_read_end = -1; // Initialize the previous read end

    // for (int i = 0; i < count_pipe_commands; i++) {
    //     int fd[2]; // File descriptors for the pipe

    //     if (pipe(fd) == -1) {
    //         perror("pipe");
    //         exit(EXIT_FAILURE);
    //     }

    //     int input_fd = prev_read_end; // Set input_fd to previous read end
    //     int output_fd = fd[1]; // Set output_fd to the write end of the pipe

    //     if (redirection(pipe_commands[i], pipe_commands[i]) == 1) {
    //         continue; // Handle redirection within the command
    //     }

    //     pid_t pid = fork();
    //     if (pid == -1) {
    //         perror("fork");
    //         exit(EXIT_FAILURE);
    //     }

    //     if (pid == 0) { // Child process
    //         close(fd[0]); // Close the read end of the pipe

    //         // Set up input redirection
    //         if (input_fd != -1) {
    //             dup2(input_fd, STDIN_FILENO);
    //             close(input_fd);
    //         }

    //         // Set up output redirection
    //         if (i < count_pipe_commands - 1) {
    //             dup2(fd[1], STDOUT_FILENO);
    //         }

    //         // Execute the command
    //         pipe_execute(pipe_commands[i], history, history_count, og_home, cwd);

    //         exit(EXIT_SUCCESS);
    //     } else { // Parent process
    //         close(fd[1]); // Close the write end of the pipe

    //         // Close the previous read end
    //         if (prev_read_end != -1) {
    //             close(prev_read_end);
    //         }

    //         prev_read_end = fd[0]; // Update the previous read end

    //         // Wait for the child process to complete
    //         int status;
    //         waitpid(pid, &status, 0);
    //     }
    // }
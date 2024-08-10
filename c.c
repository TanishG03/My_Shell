// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/wait.h>
// #include "headers.h"

// #define MAX_ARGS 256
// #define MAX_CMD_LEN 1024

// void execute_command(char *args[], int input_fd, int output_fd) {
//     pid_t pid = fork();
//     if (pid == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }

//     if (pid == 0) {
//         // Child process
//         if (input_fd != STDIN_FILENO) {
//             dup2(input_fd, STDIN_FILENO);
//             close(input_fd);
//         }
//         if (output_fd != STDOUT_FILENO) {
//             dup2(output_fd, STDOUT_FILENO);
//             close(output_fd);
//         }

//         execvp(args[0], args);
//         perror("execvp");
//         exit(EXIT_FAILURE);
//     } else {
//         // Parent process
//         wait(NULL);
//     }
// }

// int main() {
//     char input[MAX_CMD_LEN];
//     char *args[MAX_ARGS];

//     while (1) {
//         printf("Shell> ");
//         if (fgets(input, sizeof(input), stdin) == NULL) {
//             break;
//         }

//         int input_fd = STDIN_FILENO;
//         int output_fd = STDOUT_FILENO;

//         char *token = strtok(input, " \t\n");

//         int arg_count = 0;
//         int append_output = 0;

//         while (token != NULL) {
//             if (strcmp(token, "<") == 0) {
//                 token = strtok(NULL, " \t\n");
//                 input_fd = open(token, O_RDONLY);
//                 if (input_fd == -1) {
//                     perror("open");
//                     exit(EXIT_FAILURE);
//                 }
//             } else if (strcmp(token, ">") == 0) {
//                 token = strtok(NULL, " \t\n");
//                 output_fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0666);
//                 if (output_fd == -1) {
//                     perror("open");
//                     exit(EXIT_FAILURE);
//                 }
//             } else if (strcmp(token, ">>") == 0) {
//                 token = strtok(NULL, " \t\n");
//                 output_fd = open(token, O_WRONLY | O_CREAT | O_APPEND, 0666);
//                 if (output_fd == -1) {
//                     perror("open");
//                     exit(EXIT_FAILURE);
//                 }
//                 append_output = 1;
//             } else if (strcmp(token, "|") == 0) {
//                 args[arg_count] = NULL;
//                 int pipe_fd[2];
//                 if (pipe(pipe_fd) == -1) {
//                     perror("pipe");
//                     exit(EXIT_FAILURE);
//                 }
//                 output_fd = pipe_fd[1];
//                 execute_command(args, input_fd, output_fd);
//                 close(pipe_fd[1]);
//                 input_fd = pipe_fd[0];
//                 arg_count = 0;
//             } else {
//                 args[arg_count] = token;
//                 arg_count++;
//             }

//             token = strtok(NULL, " \t\n");
//         }

//         args[arg_count] = NULL;

//         if (arg_count > 0) {
//             execute_command(args, input_fd, output_fd);
//         }

//         if (append_output) {
//             close(output_fd);
//         }

//         if (input_fd != STDIN_FILENO) {
//             close(input_fd);
//         }
//     }

//     return 0;
// }



// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include <unistd.h>
// // #include <sys/types.h>
// // #include <sys/wait.h>
// // #include <fcntl.h>

// // void execute_command(char* args[], int input_fd, int output_fd) {
// //     pid_t pid = fork();
// //     if (pid == -1) {
// //         perror("fork");
// //         exit(EXIT_FAILURE);
// //     } else if (pid == 0) { // Child process
// //         if (input_fd != STDIN_FILENO) {
// //             dup2(input_fd, STDIN_FILENO);
// //             close(input_fd);
// //         }
// //         if (output_fd != STDOUT_FILENO) {
// //             dup2(output_fd, STDOUT_FILENO);
// //             close(output_fd);
// //         }
// //         execvp(args[0], args);
// //         perror("execvp");
// //         exit(EXIT_FAILURE);
// //     } else { // Parent process
// //         wait(NULL);
// //     }
// // }

// // int main() {
// //     char input[4096];

// //     printf("Enter a command: ");
// //     if (fgets(input, sizeof(input), stdin) == NULL) {
// //         exit(EXIT_FAILURE);
// //     }

// //     // Tokenize the input by pipe character
// //     char* pipe_token = strtok(input, "|");
// //     int input_fd = STDIN_FILENO;

// //     while (pipe_token != NULL) {
// //         int output_fd = STDOUT_FILENO;

// //         // Check for input/output redirection in the current command
// //         char* input_redirection = strstr(pipe_token, "<");
// //         char* output_redirection = strstr(pipe_token, ">");

// //         if (input_redirection) {
// //             *input_redirection = '\0';
// //             char* input_file = input_redirection + 1;
// //             char* input_token = strtok(input_file, " \t\n");
// //             if (input_token != NULL) {
// //                 input_fd = open(input_token, O_RDONLY);
// //                 if (input_fd == -1) {
// //                     perror("open");
// //                     input_fd = STDIN_FILENO;
// //                 }
// //             }
// //         }

// //         if (output_redirection) {
// //             *output_redirection = '\0';
// //             char* output_file = output_redirection + 1;
// //             char* output_token = strtok(output_file, " \t\n");
// //             if (output_token != NULL) {
// //                 output_fd = open(output_token, O_WRONLY | O_CREAT | O_TRUNC, 0666);
// //                 if (output_fd == -1) {
// //                     perror("open");
// //                     output_fd = STDOUT_FILENO;
// //                 }
// //             }
// //         }

// //         // Tokenize the current command
// //         char* token = strtok(pipe_token, " \t\n");
// //         int arg_count = 0;
// //         char* args[4096];
// //         while (token != NULL) {
// //             args[arg_count++] = token;
// //             token = strtok(NULL, " \t\n");
// //         }
// //         args[arg_count] = NULL;

// //         if (arg_count > 0) {
// //             execute_command(args, input_fd, output_fd);
// //         }

// //         // Restore input_fd to STDIN_FILENO for the next command
// //         input_fd = STDIN_FILENO;

// //         // Move to the next command in the pipeline
// //         pipe_token = strtok(NULL, "|");
// //     }

// //     return 0;
// // }



// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include <unistd.h>
// // #include <sys/types.h>
// // #include <sys/wait.h>
// // #include <fcntl.h>

// // void execute_command(char* args[], int input_fd, int output_fd) {
// //     pid_t pid = fork();
// //     if (pid == -1) {
// //         perror("fork");
// //         exit(EXIT_FAILURE);
// //     } else if (pid == 0) { // Child process
// //         if (input_fd != STDIN_FILENO) {
// //             dup2(input_fd, STDIN_FILENO);
// //             close(input_fd);
// //         }
// //         if (output_fd != STDOUT_FILENO) {
// //             dup2(output_fd, STDOUT_FILENO);
// //             close(output_fd);
// //         }
// //         execvp(args[0], args);
// //         perror("execvp");
// //         exit(EXIT_FAILURE);
// //     } else { // Parent process
// //         wait(NULL);
// //     }
// // }

// // int main() {
// //     char input[4096];


// //         printf("Enter a command: ");
// //         if (fgets(input, sizeof(input), stdin) == NULL) {
// //         }

// //         int input_fd = STDIN_FILENO;
// //         int output_fd = STDOUT_FILENO;

// //         // Check for input/output redirection
// //         char* input_redirection = strstr(input, "<");
// //         char* output_redirection = strstr(input, ">");
// //         char* append_redirection = strstr(input, ">>");

// //         if (input_redirection) {
// //             *input_redirection = '\0';
// //             char* input_file = input_redirection + 1;
// //             char* input_token = strtok(input_file, " \t\n");
// //             if (input_token != NULL) {
// //                 input_fd = open(input_token, O_RDONLY);
// //                 if (input_fd == -1) {
// //                     perror("open");
// //                     input_fd = STDIN_FILENO;
// //                 }
// //             }
// //         }

// //         if (output_redirection) {
// //             *output_redirection = '\0';
// //             char* output_file = output_redirection + 1;
// //             char* output_token = strtok(output_file, " \t\n");
// //             if (output_token != NULL) {
// //                 output_fd = open(output_token, O_WRONLY | O_CREAT | O_TRUNC, 0666);
// //                 if (output_fd == -1) {
// //                     perror("open");
// //                     output_fd = STDOUT_FILENO;
// //                 }
// //             }
// //         }

// //         if (append_redirection) {
// //             *append_redirection = '\0';
// //             char* append_file = append_redirection + 2;
// //             char* append_token = strtok(append_file, " \t\n");
// //             if (append_token != NULL) {
// //                 output_fd = open(append_token, O_WRONLY | O_CREAT | O_APPEND, 0666);
// //                 if (output_fd == -1) {
// //                     perror("open");
// //                     output_fd = STDOUT_FILENO;
// //                 }
// //             }
// //         }

// //         // Tokenize the input
// //         char* token = strtok(input, " \t\n");
// //         int arg_count = 0;
// //         char* args[4096];
// //         while (token != NULL) {
// //             args[arg_count++] = token;
// //             token = strtok(NULL, " \t\n");
// //         }
// //         args[arg_count] = NULL;

// //         if (arg_count > 0) {
// //             execute_command(args, input_fd, output_fd);
// //         }
    

// //     return 0;
// // }

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// int findLatestPID() {
//     int latestPID = 0;

//     // Iterate through all possible PIDs until we find the latest one
//     for (int pid = 1; pid <= 32767; pid++) { // Assuming a maximum PID of 32767
//         int status;
//         int result = waitpid(pid, &status, WNOHANG);
        
//         if (result == -1) {
//             // Error while checking the process, continue searching
//             continue;
//         }
        
//         if (result == 0) {
//             // Process with this PID exists
//             latestPID = pid;
//         }
//     }

//     return latestPID;
// }

// int main() {
//     int interval = 5; // Set the interval in seconds

//     printf("Starting process with PID: %d\n", getpid());
//     printf("Searching for the latest PID every %d seconds...\n", interval);

//     while (1) {
//         int latestPID = findLatestPID();
//         printf("Latest Process ID: %d\n", latestPID);
//         sleep(interval);
//     }

//     return 0;
// }


// #include <stdio.h>
// #include <dirent.h>
// #include <stdlib.h>
// #include <string.h>

// int findLatestPID() {
//     int latestPID = -1;
//     struct dirent *entry;
//     DIR *dir = opendir("/proc");

//     if (dir == NULL) {
//         perror("Failed to open /proc");
//         exit(1);
//     }

//     while ((entry = readdir(dir))) {
//         if (entry->d_type == DT_DIR) {
//             // Check if the entry name is a numeric string
//             int pid = atoi(entry->d_name);
//             if (pid > latestPID) {
//                 latestPID = pid;
//             }
//         }
//     }

//     closedir(dir);
//     return latestPID;
// }

// int main() {
//     int interval = 5; // Set the interval in seconds

//     printf("Starting process with PID: %d\n", getpid());
//     printf("Searching for the latest PID every %d seconds...\n", interval);

//     while (1) {
//         int latestPID = findLatestPID();
//         printf("Latest Process ID: %d\n", latestPID);
//         sleep(interval);
//     }

//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <termios.h>
// #include <unistd.h>
// #include <sys/types.h>

// void die(const char *s) {
//     perror(s);
//     exit(1);
// }

// struct termios orig_termios;

// void disableRawMode() {
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
//         die("tcsetattr");
// }

// void enableRawMode() {
//     if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
//     atexit(disableRawMode);
//     struct termios raw = orig_termios;
//     raw.c_lflag &= ~(ICANON | ECHO);
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
// }

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s <time_arg>\n", argv[0]);
//         return 1;
//     }
    
//     int time_arg = atoi(argv[1]);
//     if (time_arg <= 0) {
//         fprintf(stderr, "Invalid time_arg\n");
//         return 1;
//     }

//     enableRawMode();
    
//     int pid;
//     while (1) {
//         if ((pid = fork()) == -1) {
//             die("fork");
//         } else if (pid == 0) {
//             // Child process
//             printf("%d\n", getpid());
//             exit(0);
//         } else {
//             // Parent process
//             sleep(time_arg);
//         }

//         char c;
//         if (read(STDIN_FILENO, &c, 1) == 1) {
//             if (c == 'x' || c == 'X') {
//                 break; // Terminate the loop when 'x' or 'X' is pressed.
//             }
//         }
//     }
    
//     disableRawMode();
//     return 0;
// }


// #include <stdio.h>
// #include <stdlib.h>
// #include <termios.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/select.h>
// #include <sys/time.h>

// void die(const char *s) {
//     perror(s);
//     exit(1);
// }

// struct termios orig_termios;

// void disableRawMode() {
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
//         die("tcsetattr");
// }

// void enableRawMode() {
//     if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
//     atexit(disableRawMode);
//     struct termios raw = orig_termios;
//     raw.c_lflag &= ~(ICANON | ECHO);
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
// }

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s <time_arg>\n", argv[0]);
//         return 1;
//     }

//     int time_arg = atoi(argv[1]);
//     if (time_arg <= 0) {
//         fprintf(stderr, "Invalid time_arg\n");
//         return 1;
//     }

//     enableRawMode();

//     int pid;
//     while (1) {
//         if ((pid = fork()) == -1) {
//             die("fork");
//         } else if (pid == 0) {
//             // Child process
//             printf("%d\n", getpid());
//             exit(0);
//         } else {
//             // Parent process
//             struct timeval timeout;
//             fd_set fds;
//             timeout.tv_sec = time_arg;
//             timeout.tv_usec = 0;
//             FD_ZERO(&fds);
//             FD_SET(STDIN_FILENO, &fds);

//             int result = select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout);
//             if (result == -1) {
//                 die("select");
//             } else if (result == 0) {
//                 // Timeout, no input received
//                 // Continue with the loop
//             } else {
//                 char c;
//                 if (read(STDIN_FILENO, &c, 1) == 1) {
//                     if (c == 'x' || c == 'X') {
//                         break; // Terminate the loop when 'x' or 'X' is pressed.
//                     }
//                 }
//             }
//         }
//     }

//     disableRawMode();
//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <time_arg>\n", argv[0]);
        return 1;
    }

    int time_arg = atoi(argv[1]);
    if (time_arg <= 0) {
        fprintf(stderr, "Invalid time_arg\n");
        return 1;
    }

    enableRawMode();

    int pid;
    while (1) {
        if ((pid = fork()) == -1) {
            die("fork");
        } else if (pid == 0) {
            // Child process
            printf("%d\n", getpid());
            exit(0);
        } else {
            // Parent process
            struct timeval timeout;
            fd_set fds;
            timeout.tv_sec = time_arg;
            timeout.tv_usec = 0;
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);

            int result = select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout);
            if (result == -1) {
                die("select");
            } else if (result == 0) {
                // Timeout, no input received
                // Continue with the loop
            } else {
                char c;
                while (read(STDIN_FILENO, &c, 1) == 1) {
                    if (c == 'x' || c == 'X') {
                        disableRawMode(); // Disable raw mode before exiting
                        exit(0); // Terminate the loop when 'x' or 'X' is pressed.
                    } else {
                        // Ignore other characters
                    }
                }
            }
        }
    }

    return 0;
}

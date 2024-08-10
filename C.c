// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <unistd.h>
// // #include <fcntl.h>
// // #include <sys/wait.h>
// // #include <string.h>

// // #define MAX_ARGS 4096

// // void execute_command(char *args[], int input_fd, int output_fd) {
// //     pid_t pid = fork();
// //     if (pid == -1) {
// //         perror("fork");
// //         exit(EXIT_FAILURE);
// //     }

// //     if (pid == 0) {
// //         // Child process
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
// //     } else {
// //         // Parent process
// //         wait(NULL);
// //     }
// // }

// // int main() {
// //     char *input = "cat new.txt | wc | sed 's/ //g'";
// //     char *args[] = {"cat", "new.txt", "|", "wc", "|", "sed", "s/ //g"};
// //     int argCount = sizeof(args) / sizeof(args[0]);

// //     int input_fd = STDIN_FILENO;
// //     int output_fd = STDOUT_FILENO;

// //     int arg_count = 0;
// //     int append_output = 0;

// //     for (int i = 0; i < argCount; i++) {
// //         char *token = args[i];

// //         if (strcmp(token, "<") == 0) {
// //             i++;
// //             if (i < argCount) {
// //                 input_fd = open(args[i], O_RDONLY);
// //                 if (input_fd == -1) {
// //                     perror("open");
// //                     exit(EXIT_FAILURE);
// //                 }
// //             } else {
// //                 fprintf(stderr, "Missing input file after '<'\n");
// //                 exit(EXIT_FAILURE);
// //             }
// //         } else if (strcmp(token, ">") == 0) {
// //             i++;
// //             if (i < argCount) {
// //                 output_fd = open(args[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
// //                 if (output_fd == -1) {
// //                     perror("open");
// //                     exit(EXIT_FAILURE);
// //                 }
// //             } else {
// //                 fprintf(stderr, "Missing output file after '>'\n");
// //                 exit(EXIT_FAILURE);
// //             }
// //         } else if (strcmp(token, ">>") == 0) {
// //             i++;
// //             if (i < argCount) {
// //                 output_fd = open(args[i], O_WRONLY | O_CREAT | O_APPEND, 0666);
// //                 if (output_fd == -1) {
// //                     perror("open");
// //                     exit(EXIT_FAILURE);
// //                 }
// //                 append_output = 1;
// //             } else {
// //                 fprintf(stderr, "Missing output file after '>>'\n");
// //                 exit(EXIT_FAILURE);
// //             }
// //         } else if (strcmp(token, "|") == 0) {
// //             args[arg_count] = NULL;
// //             int pipe_fd[2];
// //             if (pipe(pipe_fd) == -1) {
// //                 perror("pipe");
// //                 exit(EXIT_FAILURE);
// //             }
// //             output_fd = pipe_fd[1];
// //             execute_command(args, input_fd, output_fd);
// //             close(pipe_fd[1]);
// //             input_fd = pipe_fd[0];
// //             arg_count = 0;
// //         } else {
// //             args[arg_count] = token;
// //             arg_count++;
// //         }
// //     }

// //     args[arg_count] = NULL;

// //     if (arg_count > 0) {
// //         execute_command(args, input_fd, output_fd);
// //     }

// //     if (append_output) {
// //         close(output_fd);
// //     }

// //     if (input_fd != STDIN_FILENO) {
// //         close(input_fd);
// //     }

// //     return 0;
// // }#include <stdio.h>
// // #include <stdlib.h>
// // #include <string.h>
// // #include "headers.h"

// // #define MAX_ARGS 100

// // int main() {
// //     char token[] = "cat < new.txt | wc | sed 's/ //g'";
// //     char *args[MAX_ARGS];
// //     int argCount = 0;

// //     int insideQuotes = 0; 
    
// //             int doubleQuoteCount = 0;
// //         int singleQuoteCount = 0;
// //         for (int i = 0; i < strlen(token); i++) {
// //             if (token[i] == '\"') {
// //                 doubleQuoteCount++;
// //             } else if (token[i] == '\'') {
// //                 singleQuoteCount++;
// //             }
// //         }

// //         if(doubleQuoteCount==2||singleQuoteCount==2)
// //         {
// //                 char* arg = strtok(token, "\"'\n");
// //             while (arg != NULL && argCount < MAX_ARGS - 1) {
// //                 // printf("%d\n",strlen(arg));

// //             if(arg[strlen(arg)-1]==' ')
// //             {
// //                 // printf("YES\n");
// //                 arg[strlen(arg)-1]='\0';
// //             }

// //             if(strlen(arg)>0)
// //             {
// //                             args[argCount] = arg;
        
// //                     argCount++;
// //             }


// //         arg = strtok(NULL, "\"'\n");
// //     }
// //     // printf("%d\n",argCount);
// //         }
        
// //         else{
// //                     char *arg = strtok(token, " ,\n\t");
// //         while (arg != NULL && argCount < MAX_ARGS - 1) {
 
// //             args[argCount] = arg;
// //             argCount++;
// //             arg = strtok(NULL, " ,\n\t");
// //         }
   
// //         }


// //         for(int i=0;i<argCount;i++)
// //         {
// //             printf("%s %d\n",args[i],strlen(args[i]));
// //         }
// //         argCount++;
// //         args[argCount] = NULL; 

// //     return 0;
// // // }
// // #include <stdio.h>
// // #include <string.h>

// // #define MAX_ARGS 100

// // int main() {
// //     char token[] = "cat < new.txt | wc | sed 's/ //g'";
// //     char *args[MAX_ARGS];
// //     int argCount = 0;

// //     int insideQuotes = 0;

// //     int doubleQuoteCount = 0;
// //     int singleQuoteCount = 0;
// //     for (int i = 0; i < strlen(token); i++) {
// //         if (token[i] == '\"') {
// //             doubleQuoteCount++;
// //         } else if (token[i] == '\'') {
// //             singleQuoteCount++;
// //         }
// //     }

// //     if (doubleQuoteCount == 2 || singleQuoteCount == 2) {
// //         char *arg = strtok(token, "\"'\n");
// //         while (arg != NULL && argCount < MAX_ARGS - 1) {
// //             // Remove leading and trailing spaces from the argument
// //             int start = 0;
// //             int end = strlen(arg) - 1;

// //             while (start <= end && arg[start] == ' ') {
// //                 start++;
// //             }

// //             while (end >= start && arg[end] == ' ') {
// //                 end--;
// //             }

// //             if (start <= end) {
// //                 arg[start + (end - start) + 1] = '\0'; // Null-terminate at the end of the trimmed string
// //                 args[argCount] = arg + start; // Set the argument pointer to the start of the trimmed string
// //                 argCount++;
// //             }

// //             arg = strtok(NULL, "\"'\n");
// //         }
// //     } else {
// //         char *arg = strtok(token, " ,\n\t");
// //         while (arg != NULL && argCount < MAX_ARGS - 1) {
// //             args[argCount] = arg;
// //             argCount++;
// //             arg = strtok(NULL, " ,\n\t");
// //         }
// //     }

// //     // Split space-separated tokens inside each element of the args array
// //     for (int i = 0; i < argCount; i++) {
// //         if (strchr(args[i], ' ') != NULL && args[i][0] != '\"' && args[i][0] != '\'') {
// //             char *innerToken = strtok(args[i], " ");
// //             while (innerToken != NULL && argCount < MAX_ARGS - 1) {
// //                 args[argCount] = innerToken;
// //                 argCount++;
// //                 innerToken = strtok(NULL, " ");
// //             }
// //         }
// //     }

// //     // Print the parsed arguments
// //     for (int i = 0; i < argCount; i++) {
// //         printf("%s %d\n", args[i], strlen(args[i]));
// //     }

// //     // Null-terminate the args array
// //     args[argCount] = NULL;

// //     return 0;
// // }


// // #include <stdio.h>
// // #include <stdbool.h>
// // #include <string.h>

// // void removeSpacesAndPreserveQuotes(char *input) {
// //     int readIdx = 0;
// //     int writeIdx = 0;
// //     bool insideQuotes = false;

// //     while (input[readIdx] != '\0') {
// //         if (input[readIdx] == '"') {
// //             insideQuotes = !insideQuotes;
// //         }

// //         if (insideQuotes || input[readIdx] != ' ') {
// //             input[writeIdx] = input[readIdx];
// //             writeIdx++;
// //         }

// //         readIdx++;
// //     }

// //     input[writeIdx] = '\0';
// // }

// // int main() {
// //     char input[] = "This is a \"sample string\" with spaces.";
// //     printf("Original: %s\n", input);
// //     removeSpacesAndPreserveQuotes(input);
// //     printf("Modified: %s\n", input);
// //     return 0;
// // }
// #include <stdio.h>
// #include <stdbool.h>
// #include <string.h>
// #include <stdlib.h>

// int main() {
//     char input[] = "This is a \"sample string\" with spaces.";
//     printf("Original: %s\n", input);

//     int readIdx = 0;
//     int tokenCount = 0;
//     bool insideQuotes = false;

//     // Initialize an array to store pointers to tokens (strings).
//     char **tokens = NULL;

//     while (input[readIdx] != '\0') {
//         if (input[readIdx] == '"') {
//             insideQuotes = !insideQuotes;
//         }

//         if ((input[readIdx] == ' ' && !insideQuotes) || input[readIdx + 1] == '\0') {
//             // End of a token, add it to the array of tokens.
//             if (input[readIdx + 1] == '\0') {
//                 readIdx++; // Include the last character in the token
//             }
//             input[readIdx] = '\0';
//             if (tokenCount == 0) {
//                 tokens = (char **)malloc(sizeof(char *));
//             } else {
//                 tokens = (char **)realloc(tokens, (tokenCount + 1) * sizeof(char *));
//             }
//             tokens[tokenCount] = &input[readIdx + 1];
//             tokenCount++;
//         }

//         readIdx++;
//     }

//     printf("Split strings:\n");
//     for (int i = 0; i < tokenCount; i++) {
//         printf("%s\n", tokens[i]);
//     }

//     // Clean up allocated memory for tokens.
//     free(tokens);

//     return 0;
// }


// #include <stdio.h>
// #include <termios.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <ctype.h>
// #include <string.h>

// void die(const char *s) {
//     perror(s);
//     exit(1);
// }

// struct termios orig_termios;

// void disableRawMode() {
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
//         die("tcsetattr");
// }

// /**
//  * Enable row mode for the terminal
//  * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
//  * Terminal attributes can be read into a termios struct by tcgetattr().
//  * After modifying them, you can then apply them to the terminal using tcsetattr().
//  * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
//  * The c_lflag field is for “local flags”
// */
// void enableRawMode() {
//     if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
//     atexit(disableRawMode);
//     struct termios raw = orig_termios;
//     raw.c_lflag &= ~(ICANON | ECHO);
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
// }

// /**
//  * stdout and stdin are buffered we disable buffering on that
//  * After entering in raw mode we read characters one by one
//  * Up arrow keys and down arrow keys are represented by 3 byte escape codes
//  * starting with ascii number 27 i.e. ESC key
//  * This way we interpret arrow keys
//  * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
//  * Backspace move the cursor one control character to the left
//  * @return
// */
// int main() {
//     char *inp = malloc(sizeof(char) * 100);
//     char c;
//     while (1) {
//         setbuf(stdout, NULL);
//         enableRawMode();
//         printf("Prompt>");
//         memset(inp, '\0', 100);
//         int pt = 0;
//         while (read(STDIN_FILENO, &c, 1) == 1) {
//             if (iscntrl(c)) {
//                 if (c == 10) break;
//                 else if (c == 27) {
//                     char buf[3];
//                     buf[2] = 0;
//                     if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
//                         printf("\rarrow key: %s", buf);
//                     }
//                 } else if (c == 127) { // backspace
//                     if (pt > 0) {
//                         if (inp[pt-1] == 9) {
//                             for (int i = 0; i < 7; i++) {
//                                 printf("\b");
//                             }
//                         }
//                         inp[--pt] = '\0';
//                         printf("\b \b");
//                     }
//                 } else if (c == 9) { // TAB character
//                     inp[pt++] = c;
//                     for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
//                         printf(" ");
//                     }
//                 } else if (c == 4) {
//                     exit(0);
//                 } else {
//                     printf("%d\n", c);
//                 }
//             } else {
//                 inp[pt++] = c;
//                 printf("%c", c);
//             }
//         }
//         disableRawMode();

//         printf("\nInput Read: [%s]\n", inp);
//     }
//     return 0;
// }

// #include <stdio.h>
// #include <termios.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <ctype.h>
// #include <string.h>
// #include <signal.h>

// void die(const char *s) {
//     perror(s);
//     exit(1);
// }

// struct termios orig_termios;

// void disableRawMode() {
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
//         die("tcsetattr");
// }

// void handleCtrlC(int sig) {
//     // Handle Ctrl+C (SIGINT) here if needed
//     printf("\nCtrl+C (SIGINT) pressed. Handling it gracefully...\n");
// }

// void enableRawMode() {
//     if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
//     atexit(disableRawMode);

//     struct termios raw = orig_termios;
//     raw.c_lflag &= ~(ICANON | ECHO);
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");

//     // Set up a signal handler for Ctrl+C (SIGINT)
//     signal(SIGINT, handleCtrlC);
// }

// int main() {
//     char *inp = malloc(sizeof(char) * 100);
//     char c;

//     while (1) {
//         setbuf(stdout, NULL);
//         enableRawMode();
//         printf("Prompt>");
//         memset(inp, '\0', 100);
//         int pt = 0;

//         while (read(STDIN_FILENO, &c, 1) == 1) {
//             if (iscntrl(c)) {
//                 if (c == 10) break;
//                 else if (c == 27) {
//                     char buf[3];
//                     buf[2] = 0;
//                     if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
//                         printf("\rarrow key: %s", buf);
//                     }
//                 } else if (c == 127) { // backspace
//                     if (pt > 0) {
//                         if (inp[pt-1] == 9) {
//                             for (int i = 0; i < 7; i++) {
//                                 printf("\b");
//                             }
//                         }
//                         inp[--pt] = '\0';
//                         printf("\b \b");
//                     }
//                 } else if (c == 9) { // TAB character
//                     inp[pt++] = c;
//                     for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
//                         printf(" ");
//                     }
//                 } else if (c == 4) {
//                     printf("\nCtrl+D (SIGTSTP) pressed. Suspending the process...\n");

//                     exit(0);
//                 } else if (c == 24) { // Ctrl+X
//                     printf("\nCtrl+X pressed. Custom action here...\n");
//                     // Perform custom action for Ctrl+X
//                 } else {
//                     printf("%d\n", c);
//                 }
//             } else {
//                 inp[pt++] = c;
//                 printf("%c", c);
//             }
//         }

//         disableRawMode();

//         printf("\nInput Read: [%s]\n", inp);
//     }

//     return 0;
// }


// #include <stdio.h>
// #include <termios.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <ctype.h>
// #include <string.h>
// #include <signal.h>
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

// void handleCtrlC(int sig) {
//     // Handle Ctrl+C (SIGINT) here if needed
//     printf("\nCtrl+C (SIGINT) pressed. Handling it gracefully...\n");
// }


// void enableRawMode() {
//     if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
//     atexit(disableRawMode);

//     struct termios raw = orig_termios;
//     raw.c_lflag &= ~(ICANON | ECHO);
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");

//     // Set up a signal handler for Ctrl+C (SIGINT)
 
// }

// void handleCtrlZ(int sig) {
//     // Handle Ctrl+Z (SIGTSTP) here
//     printf("\nCtrl+Z (SIGTSTP) pressed. Suspending the process...\n");

//     // Restore the terminal's original settings
//     disableRawMode();

//     // Suspend the process
//     raise(SIGSTOP);

//     // Regain control of the terminal
//     enableRawMode();
//     printf("Resumed. Prompt>\n");
// }





// int main() {
//     char *inp = malloc(sizeof(char) * 100);
//     char c;
//        signal(SIGINT, handleCtrlC);

//     signal(SIGTSTP, handleCtrlZ);
//     while (1) {
//         setbuf(stdout, NULL);
//         enableRawMode();
//         printf("Prompt>");
//         memset(inp, '\0', 100);
//         int pt = 0;

//         while (read(STDIN_FILENO, &c, 1) == 1) {
//             if (iscntrl(c)) {
//                 if (c == 10) break;
//                 else if (c == 27) {
//                     char buf[3];
//                     buf[2] = 0;
//                     if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
//                         printf("\rarrow key: %s", buf);
//                     }
//                 } else if (c == 127) { // backspace
//                     if (pt > 0) {
//                         if (inp[pt-1] == 9) {
//                             for (int i = 0; i < 7; i++) {
//                                 printf("\b");
//                             }
//                         }
//                         inp[--pt] = '\0';
//                         printf("\b \b");
//                     }
//                 } else if (c == 9) { // TAB character
//                     inp[pt++] = c;
//                     for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
//                         printf(" ");
//                     }
//                 } else if (c == 4) {
//                     printf("\nCtrl+D (EOF) pressed. Exiting...\n");
//                     exit(0);
//                 } else if (c == 24) { // Ctrl+X
//                     printf("\nCtrl+X pressed. Custom action here...\n");
//                     // Perform custom action for Ctrl+X
//                 } else {
//                     printf("%d\n", c);
//                 }
//             } else {
//                 inp[pt++] = c;
//                 printf("%c", c);
//             }
//         }

//         disableRawMode();

//         printf("\nInput Read: [%s]\n", inp);
//     }

//     return 0;
// // }
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <signal.h>
// #include <string.h>
// #include <termios.h>
// #include <ctype.h>
// #include <dirent.h>
// #include <sys/types.h>
// #include <wait.h>
// #define HISTORY_MAX_SIZE 100

// int neonatePID = 0;
// int exit_requested = 0;

// struct termios orig_termios;

// void sendTerminationSignal() {
//     if (neonatePID > 0) {
//         kill(neonatePID, SIGTERM);
//     }
// }

// void signal_handler(int signum) {
//     if (signum == SIGTERM) {
//         printf("Received SIGTERM. Exiting neonate.c (PID: %d)\n", getpid());
//         exit_requested = 1;
//     }
// }

// void disableRawMode() {
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
//         perror("tcsetattr");
//         exit(1);
//     }
// }

// void enableRawMode() {
//     if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
//         perror("tcgetattr");
//         exit(1);
//     }
//     atexit(disableRawMode);

//     struct termios raw = orig_termios;
//     raw.c_lflag &= ~(ICANON | ECHO);
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
//         perror("tcsetattr");
//         exit(1);
//     }
// }

// char* input_cmd()
// {
//     char *inp = malloc(sizeof(char) * 4096);
//     char c;
//     while (1) {
//         setbuf(stdout, NULL);
//         enableRawMode();
//         // printf("\033[0;32m");
//         // printf("%s>",prompt(og_home));
//         // prompt(og_home);
//         // printf("\033[0m");
//         // prompt_flag = 0;

//         memset(inp, '\0', 4096);
//         int pt = 0;
//         while (read(STDIN_FILENO, &c, 1) == 1) {
//             if (iscntrl(c)) {
//                 if (c == 10) break;
//                 else if (c == 127) { // backspace
//                     if (pt > 0) {
//                         if (inp[pt-1] == 9) {
//                             for (int i = 0; i < 7; i++) {
//                                 printf("\b");
//                             }
//                         }
//                         inp[--pt] = '\0';
//                         printf("\b \b");
//                     }
//                 } else if (c == 9) { // TAB character
//                     char tmp[1024];
//                     strcpy(tmp, inp);
//                     // strcpy(inp, tab(tmp));
//                     pt = strlen(inp);
//                     // printf("\033[0;32m");
//                     // printf("\r%s>",prompt());
//                     // printf("\033[0m");
//                     printf("%s", inp);
//                 } else if (c == 4) { // ctrl D
//                     printf("Logging Out of Shell\n");

//                     // addToHistory(history, history_count, "exit");
//                     // saveHistoryToFile(history, *history_count);
//                     exit(0);
//                 } else {
//                     printf("%d\n", c);
//                 }
//             } else {
//                 inp[pt++] = c;
//                 if(c == 'x') {
//                     printf("%d\n", neonatePID);
//                     sendTerminationSignal();
//                     exit_requested = 1;
//                 }
//                 printf("%c", c);
//             }
//         }
//         disableRawMode();
//         exit_requested = 0;
//         printf("\n");
//         return inp;
//     }
// }

// void neonate(int interval) {
//     neonatePID = getpid();
//     signal(SIGTERM, signal_handler);

//     printf("Starting process with PID: %d\n", neonatePID);
//     printf("Searching for the latest PID every %d seconds...\n", interval);

//     while (1) {
//         // int latestPID = findLatestPID();
//         // printf("Latest Process ID: %d\n", latestPID);
//         sleep(interval);
//     }

//     printf("neonate.c is exiting.\n");
// }

// int main() {
//     int interval = 5; // Set the interval in seconds

//     printf("Starting process with PID: %d\n", getpid());
//     printf("Searching for the latest PID every %d seconds...\n", interval);

//     // Create a signal handler for exit request
//     signal(SIGTERM, signal_handler);

//     // Fork a new process for neonate.c
//     pid_t child_pid = fork();

//     if (child_pid == -1) {
//         perror("Failed to fork");
//         exit(1);
//     }

//     if (child_pid == 0) {
//         // This is the child process (neonate.c)
//         neonate(interval);
//     } else {
//         // This is the parent process (main.c)
//         // while (!exit_requested) {
//         //     // Add your code to handle other tasks or checks here
//         //     sleep(interval);
//         // }
//         int status;
//         waitpid(child_pid, &status, 0);
//         while (1)
//         {
//             char *cmd = input_cmd();
//             printf("%s\n",cmd);
//         }


//         // Send a SIGTERM signal to the child process to request termination
//         // sendTerminationSignal();

//         // Wait for the child process to finish
//         // int status;
//         // waitpid(child_pid, &status, 0);

//         printf("main.c is exiting.\n");
//     }

//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <signal.h>
// #include <string.h>
// #include <termios.h>
// #include <ctype.h>
// #include <sys/types.h>
// #include <wait.h>
// #define HISTORY_MAX_SIZE 100

// int neonatePID = 0;
// int exit_requested = 0;

// struct termios orig_termios;

// void sendTerminationSignal() {
//     if (neonatePID > 0) {
//         kill(neonatePID, SIGTERM);
//     }
// }

// void signal_handler(int signum) {
//     if (signum == SIGTERM) {
//         printf("Received SIGTERM. Exiting neonate.c (PID: %d)\n", getpid());
//         exit_requested = 1;
//     }
// }

// void disableRawMode() {
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
//         perror("tcsetattr");
//         exit(1);
//     }
// }

// void enableRawMode() {
//     if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
//         perror("tcgetattr");
//         exit(1);
//     }
//     atexit(disableRawMode);

//     struct termios raw = orig_termios;
//     raw.c_lflag &= ~(ICANON | ECHO);
//     if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
//         perror("tcsetattr");
//         exit(1);
//     }
// }

// char* input_cmd() {
//     char *inp = malloc(sizeof(char) * 4096);
//     char c;
//     while (1) {
//         setbuf(stdout, NULL);
//         enableRawMode();

//         memset(inp, '\0', 4096);
//         int pt = 0;
//         while (read(STDIN_FILENO, &c, 1) == 1) {
//             if (iscntrl(c)) {
//                 if (c == 10) break;
//                 else if (c == 127) { // backspace
//                     if (pt > 0) {
//                         inp[--pt] = '\0';
//                         printf("\b \b");
//                     }
//                 } else if (c == 4) { // ctrl D
//                     printf("Logging Out of Shell\n");
//                     exit(0);
//                 } else if (c == 'x') {
//                     printf("Terminating neonate.c\n");
//                     sendTerminationSignal();
//                     exit_requested = 1;
//                     return inp; // Return from the function when 'x' is pressed
//                 }
//             } else {
//                 inp[pt++] = c;
//                 printf("%c", c);
//             }
//         }
//         disableRawMode();
//         exit_requested = 0;
//         printf("\n");
//         return inp;
//     }
// }

// void neonate(int interval) {
//     neonatePID = getpid();
//     signal(SIGTERM, signal_handler);

//     printf("Starting process with PID: %d\n", neonatePID);
//     printf("Searching for the latest PID every %d seconds...\n", interval);

//     while (!exit_requested) {
//         // In your actual code, you should find the latest PID here.
//         int latestPID = 123; // Replace with your logic to find the latest PID
//         printf("Latest Process ID: %d\n", latestPID);
//         sleep(interval);
//     }

//     printf("neonate.c is exiting.\n");
// }

// int main() {
//     int interval = 5; // Set the interval in seconds

//     printf("Starting process with PID: %d\n", getpid());
//     printf("Searching for the latest PID every %d seconds...\n", interval);

//     // Create a signal handler for exit request
//     signal(SIGTERM, signal_handler);

//     // Fork a new process for neonate.c
//     pid_t child_pid = fork();

//     if (child_pid == -1) {
//         perror("Failed to fork");
//         exit(1);
//     }

//     if (child_pid == 0) {
//         // This is the child process (neonate.c)
//         neonate(interval);
//     } else {
//         // This is the parent process (main.c)
//         while (!exit_requested) {
//             char *cmd = input_cmd();
//             if (cmd != NULL) {
//                 printf("%s\n", cmd);
//                 if(cmd[0]=='x')
//                 {
//                 sendTerminationSignal();

//                 }
//                 free(cmd); // Free the allocated memory
//             }
//         }

//         // Send a SIGTERM signal to the child process to request termination
//         sendTerminationSignal();

//         // Wait for the child process to finish
//         int status;
//         waitpid(child_pid, &status, 0);

//         printf("main.c is exiting.\n");
//     }

//     return 0;
// }


#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable raw mode for the terminal
 */
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/**
 * Function to run the infinite loop in a child process
 */
void runInfiniteLoop() {
    while (1) {
        // Your infinite loop code here
        printf("Child process running...\n");
        sleep(1); // Simulate some work in the loop
    }
}

int main() {
    char c;
    int child_pid = -1;

    setbuf(stdout, NULL);
    enableRawMode();
    
    while (1) {
        printf("Press 'x' to start/stop the child process or 'q' to exit: ");
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 'x') {
                if (child_pid == -1) {
                    // Fork a child process to run the infinite loop
                    child_pid = fork();
                    if (child_pid == -1) {
                        die("fork");
                    } else if (child_pid == 0) {
                        // This is the child process
                        runInfiniteLoop();
                        exit(0);
                    }
                } else {
                    // Terminate the child process if it's running
                    kill(child_pid, SIGTERM); // Send a termination signal to the child process
                    waitpid(child_pid, NULL, 0); // Wait for the child process to exit
                    child_pid = -1;
                    return 0;
                }
            } else if (c == 'q') {
                // Exit the program
                break;
            }
        }
    }

    // Terminate the child process if it's running
    if (child_pid > 0) {
        kill(child_pid, SIGTERM); // Send a termination signal to the child process
        waitpid(child_pid, NULL, 0); // Wait for the child process to exit
        return 0;
    }

    disableRawMode();
    return 0;
}

// int main() {
//     char c;
//     int child_pid = fork();

//     if (child_pid == -1) {
//         die("fork");
//     } else if (child_pid == 0) {
//         // This is the child process
//         enableRawMode();
//         runInfiniteLoop();
//         disableRawMode();
//         exit(0);
//     }

//     while (1) {
//         printf("Press 'x' to stop the child process or 'q' to exit: ");
//         if (read(STDIN_FILENO, &c, 1) == 1) {
//             if (c == 'x') {
//                 // Terminate the child process if it's running
//                 kill(child_pid, SIGTERM); // Send a termination signal to the child process
//                 waitpid(child_pid, NULL, 0); // Wait for the child process to exit
//                 child_pid = -1;
//                 printf("Child process stopped.\n");
//             } else if (c == 'q') {
//                 // Exit the program
//                 break;
//             }
//         }
//     }

//     disableRawMode();
//     return 0;
// }


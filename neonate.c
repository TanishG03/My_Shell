#include "headers.h"
#include <dirent.h>


volatile int neonatePID = 0;  // Initialize with 0
struct termios orig_termios1;
void die1(const char *s) {
    perror(s);
    exit(1);
}
void disableRawMode1() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios1) == -1)
        die1("tcsetattr");
}


void enableRawMode1() {
    if (tcgetattr(STDIN_FILENO, &orig_termios1) == -1) die1("tcgetattr");
    atexit(disableRawMode1);

    struct termios raw = orig_termios1;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die1("tcsetattr");

}

char* input_cmd_2()
{
  char *inp = malloc(sizeof(char) * 4096);
    char c;
    while (1) {
        setbuf(stdout, NULL);
        enableRawMode1();
        // printf("\033[0;32m");
        // printf("%s>",prompt(og_home));
        // printf("\033[0m");
        // prompt_flag = 0;

        memset(inp, '\0', 4096);
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (iscntrl(c)) {
                if (c == 10) break;
                else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                } 
            } else {
                inp[pt++] = c;
                if(c=='x')
                {
                    // printf("%d\n",neonatePID);
                    exit_requested=1;

                    // printf("hi\n");
                    // sendExitSignalToNeonate();
                }
                printf("%c", c);
            }
        }
        disableRawMode1();
        exit_requested=0;
        printf("\n");
        return inp;
      }
}

// void sendExitSignalToNeonate(int sig) {
//     if (neonatePID != 0) {
//         printf("killing neonate\n");
//         kill(neonatePID, SIGTERM); // Send a SIGTERM signal to the neonate process
//     }
// }
int findLatestPID() {
    int latestPID = -1;
    struct dirent *entry;
    DIR *dir = opendir("/proc");

    if (dir == NULL) {
        perror("Failed to open /proc");
        exit(1);
    }

    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR) {
            // Check if the entry name is a numeric string
            int pid = atoi(entry->d_name);
            if (pid > latestPID) {
                latestPID = pid;
            }
        }
    }

    closedir(dir);
    return latestPID;
}

// int neonate() {

//     if(arguementCount != 3) {
//         printf("Invalid number of arguments\n");
//         return 0;
//     }
//     int interval = atoi(arguments[2]); 
//     if(interval <= 0) {
//         printf("Invalid interval\n");
//         return 0;
//     }
//     neonatePID=getpid();
//     // printf("Starting process with PID: %d\n", getpid());
//     // printf("Searching for the latest PID every %d seconds...\n", interval);
//         signal(SIGTERM,sendExitSignalToNeonate);

//     while (1) {
//         int latestPID = findLatestPID();
//         printf("Latest Process ID: %d \n", latestPID);
//         // char *user_input = input_cmd_2();

//         // // If 'x' is pressed, exit the neonate function
//         // if (user_input[0] == 'x' ) {
//         //     printf("Terminating Neonate\n");
//         //     free(user_input);
//         //     return 1;
//         // }
//         sleep(interval);

//     }

// }

void runInfiniteLoop() {
    while (1) {
        // Your infinite loop code here
        printf("%d\n",findLatestPID());
        sleep(1); // Simulate some work in the loop
    }
}

// void kill_proc(pid_t pid)
// {
//     kill(pid, SIGTERM);
// }
// int kbhit(void)
// {
//     struct termios oldt, newt;
//     int ch;
//     int oldf;

//     // Get the current terminal I/O settings
//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;

//     // Set the new terminal I/O settings
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);

//     // Set stdin to non-blocking mode
//     oldf = fcntl(STDIN_FILENO, F_GETFL);
//     fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

//     // Check for keyboard input
//     ch = getchar();

//     // Restore the old terminal I/O settings and stdin blocking mode
//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     fcntl(STDIN_FILENO, F_SETFL, oldf);

//     if (ch != EOF)
//     {
//         ungetc(ch, stdin);
//         return 1;
//     }

//     return 0;
// }

int neonate() {

    if(arguementCount != 3) {
        printf("Invalid number of arguments\n");
        return 0;
    }
    int interval = atoi(arguments[2]); 
    if(interval <= 0) {
        printf("Invalid interval\n");
        return 0;
    }
    neonatePID=getpid();


    char c;
    int child_pid = -1;

    setbuf(stdout, NULL);
    enableRawMode1();
    
    while (1) {
        printf("Press 'x' to start/stop the neonate process ");
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 'x') {
                if (child_pid == -1) {
                    // Fork a child process to run the infinite loop
                    child_pid = fork();
                    if (child_pid == -1) {
                        die1("fork");
                    } else if (child_pid == 0) {
                        // This is the child process
                        runInfiniteLoop();
                        exit(0);
                    }
                } else {
                    // Terminate the child process if it's running
                    printf("Terminating neonate\n");
                    kill(child_pid, SIGTERM); // Send a termination signal to the child process
                    waitpid(child_pid, NULL, 0); // Wait for the child process to exit
                    child_pid = -1;
                    return 1;
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
        return 1;
    }

    //  while (1)
    // {
    //     printf("%d\n", findLatestPID());
    //     fflush(stdout);  // Flush the output buffer to ensure immediate printing
    //     sleep(interval); // Pause the program for 'time_arg' seconds
    //     if (kbhit())     // Check if a key is pressed
    //     {
    //         c = getchar(); // Read a character
    //         if (c == 'x')  // If 'x' is pressed, exit the loop
    //         {
    //             printf("terminated\n");
    //             break;
    //         }
    //     }
    // }
    disableRawMode1();
    return 1;
}



// #include "headers.h"
// #include "neonate.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <termios.h>
// #include <dirent.h>
// #include <ctype.h>
// #include <signal.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <stdio.h>
// #include <termios.h>
// #include <unistd.h>
// #include <fcntl.h>

// int kbhit(void)
// {
//     struct termios oldt, newt;
//     int ch;
//     int oldf;

//     // Get the current terminal I/O settings
//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;

//     // Set the new terminal I/O settings
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);

//     // Set stdin to non-blocking mode
//     oldf = fcntl(STDIN_FILENO, F_GETFL);
//     fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

//     // Check for keyboard input
//     ch = getchar();

//     // Restore the old terminal I/O settings and stdin blocking mode
//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     fcntl(STDIN_FILENO, F_SETFL, oldf);

//     if (ch != EOF)
//     {
//         ungetc(ch, stdin);
//         return 1;
//     }

//     return 0;
// }

// void setRawMode()
// {
//     struct termios term;
//     tcgetattr(STDIN_FILENO, &term);
//     term.c_lflag &= ~(ECHO | ICANON);
//     tcsetattr(STDIN_FILENO, TCSANOW, &term);
// }



// int isNumber(const char *str)
// {
//     for (int i = 0; str[i] != '\0'; i++)
//     {
//         if (!isdigit(str[i]))
//         {
//             return 0;
//         }
//     }
//     return 1;
// }
// int largest()
// {
//     DIR *proc_dir = opendir("/proc");
//     if (!proc_dir)
//     {
//         perror("Error opening /proc");
//         return 0;
//     }

//     int largest_pid = -1;

//     struct dirent *entry;
//     while ((entry = readdir(proc_dir)) != NULL)
//     {
//         if (isNumber(entry->d_name))
//         {
//             int pid = atoi(entry->d_name);
//             if (pid > largest_pid)
//             {
//                 largest_pid = pid;
//             }
//         }
//     }

//     closedir(proc_dir);
//     return largest_pid;
// }
// void neonate(char *str)
// {
//     char *cmd = strtok(str, " ");
//     // store tokens in array
//     char *args[5];
//     int i = 0;
//     while (cmd != NULL)
//     {
//         args[i] = cmd;
//         i++;
//         cmd = strtok(NULL, " ");
//     }
//     int time_arg = atoi(args[2]);
//     pid_t pid_ = largest();

//     setRawMode();
//     char c;
//     while (1)
//     {
//         printf("%d\n", pid_);
//         fflush(stdout);  // Flush the output buffer to ensure immediate printing
//         sleep(time_arg); // Pause the program for 'time_arg' seconds
//         if (kbhit())     // Check if a key is pressed
//         {
//             c = getchar(); // Read a character
//             if (c == 'x')  // If 'x' is pressed, exit the loop
//             {
//                 printf("terminated\n");
//                 break;
//             }
//         }
//     }
// }
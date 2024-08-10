#include "headers.h"
#include "warp.h"
#include "prompt.h"
#include "peek.h"
#include"history.h"
#include"execute.h"
#include<ctype.h>
#include <termios.h>
#include"globals.h"
int bg_proc_count;
volatile int exit_requested = 0;

void handleSignal(int sig) {
    if (sig == SIGINT) {
        // Handle Ctrl+C if needed
        // For now, let's ignore it
    } else if (sig == SIGTERM) {
        exit_requested = 1;
    }
}

void sendTerminationSignal() {
    if (neonatePID != -1) {
        if (kill(neonatePID, SIGTERM) == 0) {
            printf("Sent SIGTERM to neonate.c (PID: %d)\n", neonatePID);
        } else {
            perror("Failed to send SIGTERM");
        }
    }
}
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


void ctrl_c_handler(int sig)
{
    // printf("hi\n");
  return;
}
void ctrl_z_handler(int sig)
{


  return;
}

char* input_cmd(char * og_home,char history[HISTORY_MAX_SIZE][4096], int history_count)
{
  char *inp = malloc(sizeof(char) * 4096);
    char c;
    while (1) {
        setbuf(stdout, NULL);
        enableRawMode();
        // printf("\033[0;32m");
        // printf("%s>",prompt(og_home));
        prompt(og_home);
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
                } else if (c == 9) { // TAB character
                    char tmp[1024];
                    strcpy(tmp,inp);
                    // strcpy(inp,tab(tmp));
                    pt = strlen(inp);
                    // printf("\033[0;32m");
                    // printf("\r%s>",prompt());
                    // printf("\033[0m");
                    printf("%s",inp);
                } else if (c == 4) { // ctrl D
                    printf("Logging Out of Shell\n");

                    addToHistory(history, &history_count, "exit");
                    saveHistoryToFile(history, history_count);
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                inp[pt++] = c;
                // if(c=='x')
                // {
                //     printf("%d\n",neonatePID);
                //     sendTerminationSignal();
                //     exit_requested=1;

                // }
                printf("%c", c);
            }
        }
        disableRawMode();
        exit_requested=0;
        printf("\n");
        return inp;
      }
}

void BG_HANDLER()
{
    if (bg_proc_count == 0)
    {
        return;
    }

    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);

    if (pid > 0)
    {
        node itr = bg_proc_list_head;
        while (itr->pid != pid)
        {
            itr = itr->next;
        }

        if (!WIFSTOPPED(status))
        {
            if (WIFEXITED(status))
            {
                printf("\n");
                fflush(stdout);
                printf("%s exited with pid: %d Normally\n", itr->command, itr->pid);
            }
            else
            {
                printf("\n");
                fflush(stdout);
                printf("%s exited with pid = %d Abnormally\n", itr->command, itr->pid);
            }

            remove_node(pid);
        }
    }

    return;
}


    char history[HISTORY_MAX_SIZE][4096];
    int history_count = 0;

void sigtstp_handler(int signo) {
    // Add "exit" to history
    add_node(getpid());
    addToHistory(history, &history_count, "exit");
    saveHistoryToFile(history, history_count);

    // Exit the program
}
int restart_loop=0;
int main()
{   

    // signal(SIGINT, SIG_IGN);
    // signal(SIGTSTP, SIG_IGN);
    // signal(SIGTTIN, SIG_IGN);
    // signal(SIGTTOU, SIG_IGN);
        signal(SIGCHLD, BG_HANDLER);

        // signal(SIGTERM, sigtstp_handler);
        signal(SIGINT, ctrl_c_handler);
        signal(SIGTSTP,ctrl_z_handler);
    bg_proc_list_head=init_list();
    // Keep accepting commands
    char og_home[4096];  // Adjust the array size as needed

    // char *og_home = getenv("PWD");
        if (getcwd(og_home, sizeof(og_home)) == NULL) {
        perror("getcwd");
        return 1;
    }
        strcpy(myHome,og_home);

    char *prev_dir=og_home;
    // char history[HISTORY_MAX_SIZE][4096];
    // int history_count = 0;


    shell_pid=getpid();
    // printf("Shell pid: %d\n",shell_pid);
    // signal(SIGCHLD, BG_HANDLER);

    //     signal(SIGTSTP, sigtstp_handler);


loadHistoryFromFile(history, &history_count);
    while (1)
    {
        arguementCount=0;
        exec_success=0;
        if(restart_loop==1)
        {
            restart_loop=0;
            continue;
        }
        // Print appropriate prompt with username, systemname and directory before accepting input
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        // prompt(og_home);
        char input[4096];
        // fgets(input, 4096, stdin);
        strcpy(input,input_cmd(og_home,history,history_count));
        if(strcmp(input,"x"))
        {
            // sendTerminationSignal();
        }





        execute(history, &history_count, input, og_home, cwd);

    saveHistoryToFile(history, history_count);



    }
}

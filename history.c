#include "peek.h"
#include "warp.h"
#include "headers.h"
#include "history.h"
#include"execute.h"
#define HISTORY_MAX_SIZE 15


void removeNewline(char *str) {
    char *newlinePos = strchr(str, '\n');
    if (newlinePos != NULL) {
        *newlinePos = '\0'; // Replace newline with null terminator
    }
}

void saveHistoryToFile(const char history[HISTORY_MAX_SIZE][4096], int history_count) {
    FILE *file = fopen("history.txt", "w");
    if (file == NULL) {
        perror("Error opening history file");
        return;
    }
    

    for (int i = 0; i < history_count; i++) {
        char temp[4096];
        strcpy(temp, history[i]); // Create a temporary copy of the history item
        removeNewline(temp); // Remove newline from the temporary copy
        fprintf(file, "%s\n", temp);
    }
    //     for (int i = 0; i < history_count; i++) {
    //     if (strcmp(history[i], "\n") != 0) {
    //         fprintf(file, "%s\n", history[i]);
    //     }
    // }
    
    fclose(file);
}

void loadHistoryFromFile(char history[HISTORY_MAX_SIZE][4096], int *history_count) {
    FILE *file = fopen("history.txt", "r");
    if (file == NULL) {
        perror("Error opening history file");
        return;
    }
    
    // *history_count = 0;
    // while (*history_count < HISTORY_MAX_SIZE && fgets(history[*history_count], 4096, file)) {
    //     (*history_count)++;
    // }
    

        *history_count = 0;
    while (*history_count < HISTORY_MAX_SIZE && fgets(history[*history_count], 4096, file)) {
        // Remove the newline character if present
        size_t len = strlen(history[*history_count]);
        if (len > 0 && history[*history_count][len - 1] == '\n') {
            history[*history_count][len - 1] = '\0';
        }
        (*history_count)++;
    }

        fclose(file);

    
}

void addToHistory(char history[HISTORY_MAX_SIZE][4096], int *history_count, const char *command) {
   if(strcmp(command,"\n")==0)
    {
         return;
    }

    if (*history_count > 0 && strcmp(command, history[*history_count - 1]) == 0) {
        return; // Don't add if it's the same as the latest entry
    }
   
    if (*history_count < HISTORY_MAX_SIZE) {
        strcpy(history[*history_count], command);
        (*history_count)++;
    } else {
        for (int j = 0; j < HISTORY_MAX_SIZE - 1; j++) {
            strcpy(history[j], history[j + 1]);
        }
        strcpy(history[HISTORY_MAX_SIZE - 1], command);
    }
}

// Function to print history
void printHistory(const char history[HISTORY_MAX_SIZE][4096], int history_count) {
    for (int i = 0; i < history_count; i++) {
        printf("%s\n", history[i]);
    }
}

void init_history(char *token,char history[HISTORY_MAX_SIZE][4096], int *history_count, char *og_home, char *cwd) {

    char *args[10];
    int argCount = 0;

    while ((token = strtok(NULL, " \t\n")) != NULL) {
                    args[argCount] = token;
                    argCount++;
    }

    if (argCount == 0) {
        printHistory(history, *history_count);
    } else if (strcmp(args[0],"purge")==0){
    *history_count = 0;
    printf("History purged.\n");

    } else if (argCount==2 && strcmp(args[0],"execute")==0) {
        // int index= args[1];

        int index = atoi(args[1]); // Convert the string to an integer

        if (index > 0 && index <= *history_count) {
            printf("History at index %d:\n%s\n", index, history[*history_count - index]);
            execute(history, history_count, history[*history_count - index], og_home, cwd);
        } else {
            printf("Invalid index or index out of range.\n");
        }
    }

    else  {
        printf("Invalid arguments\n");
    }

}

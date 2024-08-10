#include "headers.h"
#include "warp.h"
#include "prompt.h"
#include "peek.h"
#include"history.h"
#include"execute.h"
#include<ctype.h>

int main()
{
    // Keep accepting commands

    char *og_home = getenv("PWD");
    char *prev_dir=og_home;

    char history[HISTORY_MAX_SIZE][4096];
    int history_count = 0;

loadHistoryFromFile(history, &history_count);
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        prompt(og_home);
        char input[4096];
        fgets(input, 4096, stdin);

        execute(history, &history_count, input, og_home, cwd);
    //     const char semicolon_delim[] = ";\n";
    //     char *segment_tokens[100]; // Array to store segment tokens
    //     int num_segments = 0;

    // //     char *segment = strtok(input, semicolon_delim);

    // //     while (segment != NULL && num_segments < 100) {
    // //     segment_tokens[num_segments] = segment;

    // //     num_segments++;

    // //     segment = strtok(NULL, semicolon_delim); // Move to the next segment
    // // }

    //     char *input_copy = strdup(input);  // Create a copy of the input to avoid modifying it

    //     char *saveptr1;  // Saveptr for strtok_r

    //     char *segment = strtok_r(input_copy, semicolon_delim, &saveptr1);

    //     while (segment != NULL && num_segments < 100) {
    //         // Trim leading whitespaces
    //         while (isspace(*segment)) {
    //             segment++;
    //         }

    //         // Trim trailing whitespaces
    //         char *end = segment + strlen(segment) - 1;
    //         while (end > segment && isspace(*end)) {
    //             end--;
    //         }
    //         *(end + 1) = '\0';

    //         segment_tokens[num_segments] = segment;
    //         num_segments++;

    //         segment = strtok_r(NULL, semicolon_delim, &saveptr1); // Move to the next segment
    //     }


    //     for (int i = 0; i < num_segments; i++) {
    //     char history_instance[10000];
    //     strcpy(history_instance,segment_tokens[i]);
    //     const char word_delim[] = " ,\n"; // Delimiters for tokenizing each segment
    //     char *token = strtok(segment_tokens[i], word_delim);

    //     if(token)
    //     {
    //         if(strcmp(token,"warp")==0)
    //         {
    //             getcwd(cwd, sizeof(cwd));
    //             // printf("%s\n",cwd);
    //             if(warp(token,cwd,og_home))
    //             {
    //                 addToHistory(history, &history_count, history_instance);
    //                 printf("added to history\n");
    //             }
    //             else{
    //                 printf("not added to history\n");}
    //         }

    //         if(strcmp(token,"ls")==0)
    //         {
                
    //             if(peek(token,cwd,og_home))
    //             {
    //                 addToHistory(history, &history_count, history_instance);
    //                 printf("added to history\n");
    //             }
    //             else{
    //                 printf("not added to history\n");}

    //         }

    //         if(strcmp(token,"pastevents")==0)
    //         {

    //             printHistory(history, history_count);
    //         }
    //     }
    // }

    // free(input_copy); // Free the copy of the input


    saveHistoryToFile(history, history_count);



    }
}

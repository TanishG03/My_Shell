#include "headers.h"


void pipe_execute(char *og_home, char *cwd , char *args[],char history[HISTORY_MAX_SIZE][4096], int *history_count)
{
//     for(int i=0;i<argCount-1;i++)
// {
//     if(strcmp(args[i],"peek")==0)
//     {
//         args[i]="ls";
//     }

//      if(strcmp(args[i],"warp")==0)
//     {
//         args[i]="cd";
//     }

// }
        // signal(SIGCHLD, sigchld_handler);
    char pipe_command[4096] = "";
    for (int i = 0; args[i] != NULL; i++) {
        // printf("%s\n",args[i]);

    //         if(strcmp(args[i],"peek")==0)
    // {
    //     args[i]="ls";
    // }

        strcat(pipe_command, args[i]);
        strcat(pipe_command, " ");
    }
        // printf("%s\n",pipe_command);
        char history_instance[10000];
        strcpy(history_instance,pipe_command);
        const char word_delim[] = " ,\n\t"; // Delimiters for tokenizing each segment
        char *token = strtok(pipe_command, word_delim);

        if(token)
        {
            if(strcmp(token,"warp")==0)
            {
                getcwd(cwd, sizeof(cwd));
                // printf("%s\n",cwd);
                if(warp(token,cwd,og_home))
                {
                    // addToHistory(history, history_count, badi_copy);
                      exec_success=1;
                      exit(0);
                }
              
                else{
                    exec_success=-1;}
            }

            
            else if(strcmp(token,"peek")==0)
            {
                
                if(peek(token,cwd,og_home))
                {
                      exec_success=1;
                      exit(0);
                    // addToHistory(history, history_count, badi_copy);
                }
                else{
                    exec_success=-1;}

            }

            else if(strcmp(token,"pastevents")==0)
            {
                init_history(token,history, history_count,og_home,cwd);
                exit (0);
            }

            else if(strcmp(token,"proclore")==0)
            {
          

                 if(proclore(token,og_home,cwd))
                {
                      exec_success=1;
                      exit(0);
                }
                else{
                    exec_success=-1;}


            }

            else if(strcmp(token,"seek")==0)
            {
                 exec_success=1;
   
                seek(token,og_home,cwd);
                exit(0);


            }
             else if(strcmp(token,"activities")==0)
            {
                if(activities(token))
                {
                    exec_success=1;
                    exit(0);
                }

                    else{
                    exec_success=-1;}
            }
            else if(strcmp(token,"bg")==0)
            {
                if(bg())
                {
                    exec_success=1;
                    exit(0);
                }

                    else{
                    exec_success=-1;}

            }

                        else if(strcmp(token,"fg")==0)
            {


                if(fg())
                {
                    exec_success=1;
                    exit(0);
                }

                    else{
                    exec_success=-1;}
            }

            else if (strcmp(token,"ping")==0)
            {
                if(ping())
                {
                    exec_success=1;
                    exit(0);
                }

                 else{
                    exec_success=-1;}
            }

            else if (strcmp(token,"neonate")==0)
            {
                if(neonate(og_home,history,history_count)==1)
                {
                    
                    exit(0);
                }
                    exec_success=1;

            }
            else if(strcmp(token,"iMan")==0)
            {
                if(man(token))
                {
                    exec_success=1;
                    exit(0);
                }
                 else{
                    exec_success=-1;}
                
            }

            else{
                        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
            }
        }
    }






// void pipe_execute(char * pipe_command,char history[HISTORY_MAX_SIZE][4096], int *history_count, char *og_home, char *cwd)
// {
//         // signal(SIGCHLD, sigchld_handler);

        
//         char history_instance[10000];
//         strcpy(history_instance,pipe_command);
//         const char word_delim[] = " ,\n\t"; // Delimiters for tokenizing each segment
//         char *token = strtok(pipe_command, word_delim);

//         if(token)
//         {
//             if(strcmp(token,"warp")==0)
//             {
//                 getcwd(cwd, sizeof(cwd));
//                 // printf("%s\n",cwd);
//                 if(warp(token,cwd,og_home))
//                 {
//                     // addToHistory(history, history_count, badi_copy);
//                       exec_success=1;
//                 }
              
//                 else{
//                     exec_success=-1;}
//             }

            
//             else if(strcmp(token,"peek")==0)
//             {
                
//                 if(peek(token,cwd,og_home))
//                 {
//                       exec_success=1;
//                     // addToHistory(history, history_count, badi_copy);
//                 }
//                 else{
//                     exec_success=-1;}

//             }

//             else if(strcmp(token,"pastevents")==0)
//             {
//                 init_history(token,history, history_count,og_home,cwd);
//             }

//             else if(strcmp(token,"proclore")==0)
//             {
          

//                  if(proclore(token,og_home,cwd))
//                 {
//                       exec_success=1;
//                 }
//                 else{
//                     exec_success=-1;}

//             }

//             else if(strcmp(token,"seek")==0)
//             {
//                  exec_success=1;
   
//                 seek(token,og_home,cwd);


//             }

//             else{
//                 execute_segments(history_instance);
//             }
//         }
//     }





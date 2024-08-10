#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "headers.h"

        int match_found = 0; // Flag to track if any match is found
        static int count=0;
        // int dir_only=0;
        // int file_only=0;
        char temp[1000];




void list_files(const char *path, const char *search,int dir_only,int file_only) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }


    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1000];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat file_stat;
        if (stat(full_path, &file_stat) == -1) {
            perror("Error getting file status");
            continue;
        }

        int is_dir = S_ISDIR(file_stat.st_mode);
        int is_file = S_ISREG(file_stat.st_mode);
        // printf("%d\n",is_dir);
        // printf("%d\n",is_file);
        if ((!dir_only && !file_only) ||
            (dir_only && is_dir) ||
            (file_only && is_file)) {
            if (strstr(entry->d_name, search) != NULL) {
                printf("%s/%s\n", path, entry->d_name);
                snprintf(temp, sizeof(temp), "%s/%s", path, entry->d_name);
                match_found = 1; // Set the flag to true if a match is found
                count++;
            }

        

            if (is_dir) {
                list_files(full_path, search,dir_only,file_only);
            }
        }
    }


    closedir(dir);

}

int seek(char *token, char *og_dir, char *cwd) {
    int argc = 0; // Number of arguments
    char *argv[4096]; // Array of argument strings
    // printf("hi\n");
    while ((token = strtok(NULL, " \t\n")) != NULL) {
        argv[argc] = token;
        argc++;
    }

    // for(int i=0;i<argc;i++)
    // {
    //     printf("%s\n",argv[i]);
    // }
    if (argc == 0) {
        // Case: No arguments provided
        fprintf(stderr, "seek: missing arguments\n");
        return 0;
    }

    char *search = NULL;
    char *target_directory = "."; // Default target directory
    int dir_only = 0;
    int file_only = 0;
    count =0;

    if(strcmp(argv[0],"-fd")==0||strcmp(argv[0],"-df")==0)
    {
        printf("Invalid flags\n");
        return 0;
    }
    int execute_flag = 0;
   for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            execute_flag = 1;
            break;
        }
    }

    // printf("%d\n",execute_flag);
   for (int i = 0; i < argc-1; i++) {
        if (((strcmp(argv[i], "-d") == 0)&& strcmp(argv[i+1], "-f") == 0)||((strcmp(argv[i], "-f") == 0)&& strcmp(argv[i+1], "-d") == 0)) {
            printf("Invalid flags\n");
            return 0;
        }
    }

    if (argc == 0) {
        // Case: No arguments provided
        fprintf(stderr, "seek: missing arguments\n");
        return 0;
    }

    else if (argc == 2 && argv[0][0] == '-') {
        // Case: A flag is provided as the first argument

         if((strcmp(argv[0],"-f")==0 && strcmp(argv[1],"-d")==0)|| (strcmp(argv[1],"-f")==0 && strcmp(argv[0],"-d")==0)) {
            fprintf(stderr, "Invalid flags\n");
            return 0;
        }
       else if (strcmp(argv[0], "-d") == 0) {
            dir_only = 1;
        } else if (strcmp(argv[0], "-f") == 0) {
            file_only = 1;
        } 
        else if(strcmp(argv[0],"-e")==0)
        {
            execute_flag=1;
        }

        else {
            fprintf(stderr, "Invalid flag: %s\n", argv[0]);
            return 0 ;
        }
        search = argv[1];

    } 
    else if (argc == 2) {
        // Case: No flag, first argument is search, and second argument is target directory
        search = argv[0];
        target_directory = argv[1];
    }

    else if(argc ==3)
    {

        if(argv[0][0]=='-' && argv[1][0]=='-')
        {

                    if ((strcmp(argv[0], "-d") == 0) || strcmp(argv[1], "-d") == 0) {
            dir_only = 1;
        }  if ((strcmp(argv[0], "-f") == 0) || strcmp(argv[1], "-f") == 0) {
            file_only = 1;
        } 

         if ((strcmp(argv[0], "-e") == 0) || strcmp(argv[1], "-e") == 0) {
            execute_flag = 1;
        } 

            else
     {
            fprintf(stderr, "Invalid flag: %s\n", argv[0]);
            
            return 0;
        }
        search = argv[2];
        // target_directory = argv[2];

        }
        else{
                                if ((strcmp(argv[0], "-d") == 0) ) {
            dir_only = 1;
        }  if ((strcmp(argv[0], "-f") == 0) ) {
            file_only = 1;
        } 

         if ((strcmp(argv[0], "-e") == 0) ) {
            execute_flag = 1;
        } 

            else
     {
            fprintf(stderr, "Invalid flag: %s\n", argv[0]);
            
            return 0;
        }
        search = argv[1];
        target_directory = argv[2];
        }
        // if ((strcmp(argv[0], "-d") == 0) || strcmp(argv[1], "-d") == 0) {
        //     dir_only = 1;
        // }  if ((strcmp(argv[0], "-f") == 0) || strcmp(argv[1], "-f") == 0) {
        //     file_only = 1;
        // } 

        //  if ((strcmp(argv[0], "-e") == 0) || strcmp(argv[1], "-e") == 0) {
        //     execute_flag = 1;
        // } 


        // else if (strcmp(argv[]))

    // else
    //  {
    //         fprintf(stderr, "Invalid flag: %s\n", argv[0]);
            
    //         return 0;
    //     }
    //     search = argv[1];
    //     target_directory = argv[2];


    //     else
    //  {
    //         fprintf(stderr, "Invalid flag: %s\n", argv[0]);
            
    //         return 0;
    //     }
    //     search = argv[1];
    //     target_directory = argv[2];
    }
     else if (argc == 1) {
        // Case: Only one argument (search), use current directory as target directory
        search = argv[0];
    }
    else if(argc ==4)
    {
             if (strcmp(argv[0], "-d") == 0) {
            dir_only = 1;
        } else if (strcmp(argv[0], "-f") == 0) {
            file_only = 1;
        } 
        search = argv[2];
        target_directory = argv[3];
    }
    else {
        fprintf(stderr, "Invalid arguments\n");
        return 0;
    }
    // printf("%d\n",dir_only);
    // printf("%d\n",file_only);
    // Call the list_files function with appropriate arguments
    list_files(target_directory, search,dir_only,file_only);
    if (!match_found) {
        printf("No match found \n");
    }
    // printf("%d\n",execute_flag);
    if(execute_flag)
    {
    
    if(count==1)
    {
        struct stat file_stat;
        if (stat(temp, &file_stat) == -1) {
            perror("Error getting file status");
            return 0;
        }
        // printf("%s\n",temp);
        int is_dir = S_ISDIR(file_stat.st_mode);
        int is_file = S_ISREG(file_stat.st_mode);

        
        
        if(is_file){
            if(access(temp,R_OK)==0)
            {            FILE *file = fopen(temp, "r");
                        if (file) {
                            char buffer[4096];
                            while (fgets(buffer, sizeof(buffer), file)) {
                                printf("%s", buffer);
                            }
                            fclose(file);}
                            else{
                                perror("Error opening file");}

                        } else {
                            perror("Missing Permissions for the Task\n");
                        }
        }
        else if(is_dir)
        {
    //             if (access(temp, X_OK) == 0) {
    //     chdir(temp);
    // } else {
    //     perror("Missing Permissions for the Task\n");
    // }
            chdir(temp);
        }

    
    }
    }

    return 1;
}


// void list_files(const char *path, const char *search) {
//     DIR *dir = opendir(path);
//     if (dir == NULL) {
//         perror("Error opening directory");
//         return;
//     }


//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
//             continue;
//         }

//         char full_path[1000];
//         snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

//         struct stat file_stat;
//         if (stat(full_path, &file_stat) == -1) {
//             perror("Error getting file status");
//             continue;
//         }

//         int is_dir = S_ISDIR(file_stat.st_mode);
//         int is_file = S_ISREG(file_stat.st_mode);
//         if ((!dir_only && !file_only) ||
//             (dir_only && is_dir) ||
//             (file_only && is_file)) {
//             if (strstr(entry->d_name, search) != NULL) {
//                 printf("%s/%s\n", path, entry->d_name);
//                 snprintf(temp, sizeof(temp), "%s/%s", path, entry->d_name);
//                 match_found = 1; // Set the flag to true if a match is found
//                 count++;
//             }

//             if (is_dir) {
//                 list_files(full_path, search);
//             }
//         }
//     }


//     closedir(dir);

// }

// void seek(char *token, char *og_dir, char *cwd) {
//     int argc = 0; // Number of arguments
//     char *argv[4096]; // Array of argument strings
//     // printf("hi\n");
//     while ((token = strtok(NULL, " \t\n")) != NULL) {
//         argv[argc] = token;
//         argc++;
//     }

//     // for(int i=0;i<argc;i++)
//     // {
//     //     printf("%s\n",argv[i]);
//     // }

//     char *search = NULL;
//     char *target_directory = "."; // Default target directory
//     int dir_only = 0;
//     int file_only = 0;

//     if(strcmp(argv[0],"-fd")==0||strcmp(argv[0],"-df")==0)
//     {
//         printf("Invalid flags\n");
//         return;
//     }

//     if (argc == 0) {
//         // Case: No arguments provided
//         fprintf(stderr, "seek: missing arguments\n");
//         return;
//     }
//     else if (argc == 2 && argv[0][0] == '-') {
//         // Case: A flag is provided as the first argument

//          if((strcmp(argv[0],"-f")==0 && strcmp(argv[1],"-d")==0)|| (strcmp(argv[1],"-f")==0 && strcmp(argv[0],"-d")==0)) {
//             fprintf(stderr, "Invalid flags\n");
//             return ;
//         }
//        else if (strcmp(argv[0], "-d") == 0) {
//             dir_only = 1;
//         } else if (strcmp(argv[0], "-f") == 0) {
//             file_only = 1;
//         } 
//         else {
//             fprintf(stderr, "Invalid flag: %s\n", argv[0]);
//             return ;
//         }
//         search = argv[1];

//     } else if (argc == 2) {
//         // Case: No flag, first argument is search, and second argument is target directory
//         search = argv[0];
//         target_directory = argv[1];
//     }
//     else if(argc ==3)
//     {
//         if((strcmp(argv[0],"-f")==0 && strcmp(argv[1],"-d")==0)|| (strcmp(argv[1],"-f")==0 && strcmp(argv[0],"-d")==0))
//         {
//             fprintf(stderr, "Invalid flag: %s\n", argv[0]);
//             return ;
//         }
//         if (strcmp(argv[0], "-d") == 0) {
//             dir_only = 1;
//         } else if (strcmp(argv[0], "-f") == 0) {
//             file_only = 1;
//         } 
//         // else if (strcmp(argv[]))

//     else {
//             fprintf(stderr, "Invalid flag: %s\n", argv[0]);
            
//             return ;
//         }
//         search = argv[1];
//         target_directory = argv[2];
//     }
//      else if (argc == 1) {
//         // Case: Only one argument (search), use current directory as target directory
//         search = argv[0];
//     }
//     else {
//         fprintf(stderr, "Invalid arguments\n");
//         return ;
//     }

//     // Call the list_files function with appropriate arguments
//     list_files(target_directory, search);
//             if (!match_found) {
//         printf("No match found \n");
//     }
//     if(count==1)
//     {
//         struct stat file_stat;
//         if (stat(temp, &file_stat) == -1) {
//             perror("Error getting file status");
//             return;
//         }

//         int is_dir = S_ISDIR(file_stat.st_mode);
//         int is_file = S_ISREG(file_stat.st_mode);

//         if(is_file){
//             if(access(temp,R_OK)==0)
//             {            FILE *file = fopen(temp, "r");
//                         if (file) {
//                             char buffer[4096];
//                             while (fgets(buffer, sizeof(buffer), file)) {
//                                 printf("%s", buffer);
//                             }
//                             fclose(file);}
//                             else{
//                                 perror("Error opening file");}

//                         } else {
//                             perror("Missing Permissions for the Task\n");
//                         }
//         }
//         else if(is_dir)
//         {
//                 if (access(temp, X_OK) == 0) {
//         chdir(temp);
//     } else {
//         perror("Missing Permissions for the Task\n");
//     }
//             // chdir(temp);
//         }

    
//     }

//     return ;
// }



// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <dirent.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include "headers.h"

//         int match_found = 0; // Flag to track if any match is found
//         static int count=0;

//         char temp[1000];

// void list_files(const char *path, const char *search) {
//     DIR *dir = opendir(path);
//     if (dir == NULL) {
//         perror("Error opening directory");
//         return;
//     }


//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
//             continue;
//         }

//         char full_path[1000];
//         snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

//         struct stat file_stat;
//         if (stat(full_path, &file_stat) == -1) {
//             perror("Error getting file status");
//             continue;
//         }

//         int is_dir = S_ISDIR(file_stat.st_mode);
//         int is_file = S_ISREG(file_stat.st_mode);
//         if ((!dir_only && !file_only) ||
//             (dir_only && is_dir) ||
//             (file_only && is_file)) {
//             if (strstr(entry->d_name, search) != NULL) {
//                 printf("%s/%s\n", path, entry->d_name);
//                 snprintf(temp, sizeof(temp), "%s/%s", path, entry->d_name);
//                 match_found = 1; // Set the flag to true if a match is found
//                 count++;
//             }

//             if (is_dir) {
//                 list_files(full_path, search);
//             }
//         }
//     }


//     closedir(dir);

// }

// void seek(char *token, char *og_dir, char *cwd) {
//     int argc = 0; // Number of arguments
//     char *argv[4096]; // Array of argument strings
//     // printf("hi\n");
//     while ((token = strtok(NULL, " \t\n")) != NULL) {
//         argv[argc] = token;
//         argc++;
//     }

//     // for(int i=0;i<argc;i++)
//     // {
//     //     printf("%s\n",argv[i]);
//     // }

//     char *search = NULL;
//     char *target_directory = "."; // Default target directory
//     int dir_only = 0;
//     int file_only = 0;

//     if(strcmp(argv[0],"-fd")==0||strcmp(argv[0],"-df")==0)
//     {
//         printf("Invalid flags\n");
//         return;
//     }

//     if (argc == 0) {
//         // Case: No arguments provided
//         fprintf(stderr, "seek: missing arguments\n");
//         return;
//     }
//     else if (argc == 2 && argv[0][0] == '-') {
//         // Case: A flag is provided as the first argument

//          if((strcmp(argv[0],"-f")==0 && strcmp(argv[1],"-d")==0)|| (strcmp(argv[1],"-f")==0 && strcmp(argv[0],"-d")==0)) {
//             fprintf(stderr, "Invalid flags\n");
//             return ;
//         }
//        else if (strcmp(argv[0], "-d") == 0) {
//             dir_only = 1;
//         } else if (strcmp(argv[0], "-f") == 0) {
//             file_only = 1;
//         } 
//         else {
//             fprintf(stderr, "Invalid flag: %s\n", argv[0]);
//             return ;
//         }
//         search = argv[1];

//     } else if (argc == 2) {
//         // Case: No flag, first argument is search, and second argument is target directory
//         search = argv[0];
//         target_directory = argv[1];
//     }
//     else if(argc ==3)
//     {
//         if((strcmp(argv[0],"-f")==0 && strcmp(argv[1],"-d")==0)|| (strcmp(argv[1],"-f")==0 && strcmp(argv[0],"-d")==0))
//         {
//             fprintf(stderr, "Invalid flag: %s\n", argv[0]);
//             return ;
//         }
//         if (strcmp(argv[0], "-d") == 0) {
//             dir_only = 1;
//         } else if (strcmp(argv[0], "-f") == 0) {
//             file_only = 1;
//         } 
        

//     else {
//             fprintf(stderr, "Invalid flag: %s\n", argv[0]);
            
//             return ;
//         }
//         search = argv[1];
//         target_directory = argv[2];
//     }
//      else if (argc == 1) {
//         // Case: Only one argument (search), use current directory as target directory
//         search = argv[0];
//     }
//     else {
//         fprintf(stderr, "Invalid arguments\n");
//         return ;
//     }

//     // Call the list_files function with appropriate arguments
//     list_files(target_directory, search);
//             if (!match_found) {
//         printf("No match found \n");
//     }
//     if(count==1)
//     {
//         struct stat file_stat;
//         if (stat(temp, &file_stat) == -1) {
//             perror("Error getting file status");
//             return;
//         }

//         int is_dir = S_ISDIR(file_stat.st_mode);
//         int is_file = S_ISREG(file_stat.st_mode);

//         if(is_file){
//             if(access(temp,R_OK)==0)
//             {            FILE *file = fopen(temp, "r");
//                         if (file) {
//                             char buffer[4096];
//                             while (fgets(buffer, sizeof(buffer), file)) {
//                                 printf("%s", buffer);
//                             }
//                             fclose(file);}
//                             else{
//                                 perror("Error opening file");}

//                         } else {
//                             perror("Missing Permissions for the Task\n");
//                         }
//         }
//         else if(is_dir)
//         {
//                 if (access(temp, X_OK) == 0) {
//         chdir(temp);
//     } else {
//         perror("Missing Permissions for the Task\n");
//     }
//             // chdir(temp);
//         }

    
//     }

//     return ;
// }


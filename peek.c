#include "peek.h"
#include "warp.h"
#include "headers.h"


int compare(const void *arg1, const void *arg2)
{
    return (strcmp(*(char **)arg1, *(char **)arg2));
}


char file_type(mode_t mode) {
    char c;
    if (S_ISREG(mode))
        c = '-';
    else if (S_ISDIR(mode))
        c = 'd';
    else if (S_ISBLK(mode))
        c = 'b';
    else if (S_ISCHR(mode))
        c = 'c';
    else if (S_ISFIFO(mode))
        c = 'p';
    else if (S_ISLNK(mode))
        c = 'l';
    else if (S_ISSOCK(mode))
        c = 's';
    else {
        c = '?';
    }
    return c;
}

int ls_print(char* file){
    struct stat per;

    if(stat(file,&per)!=0){
        perror("Error: ");
        return 0;
    }

    if(per.st_mode & S_IXUSR) printf("\033[0;32m");
    if (S_ISDIR(per.st_mode)) printf("\033[0;34m"); // Blue color for directory

    printf("%s\n",file);
    printf("\033[0m");
}

int ls_l_print(char* file){
    struct stat per;

    if(stat(file,&per)!=0){
        perror("Error: ");
        return 0;
    }

    printf("%c",file_type(per.st_mode));
    char date[100];
    printf( (per.st_mode & S_IRUSR) ? "r" : "-");
    printf( (per.st_mode & S_IWUSR) ? "w" : "-");
    printf( (per.st_mode & S_IXUSR) ? "x" : "-");
    printf( (per.st_mode & S_IRGRP) ? "r" : "-");
    printf( (per.st_mode & S_IWGRP) ? "w" : "-");
    printf( (per.st_mode & S_IXGRP) ? "x" : "-");
    printf( (per.st_mode & S_IROTH) ? "r" : "-");
    printf( (per.st_mode & S_IWOTH) ? "w" : "-");
    printf( (per.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
    printf("%ld ",per.st_nlink);
    printf("%s ", getpwuid(per.st_uid)->pw_name);
    printf("%s ", getgrgid(per.st_gid)->gr_name);
    printf("\t%ld\t ", per.st_size);
    if (time(0) - per.st_mtime < 15780000)
    strftime(date, 20, "%d %b %H:%M", localtime(&(per.st_mtime)));
    else
    strftime(date, 20, "%d %b  %Y", localtime(&(per.st_mtime)));
    printf("%s ",date);
    if(per.st_mode & S_IXUSR) printf("\033[0;32m");
    if (S_ISDIR(per.st_mode)) printf("\033[0;34m"); // Blue color for directory

    printf("%s\n",file);
    printf("\033[0m");
}

int peek(char * token , char * cwd, char *og_home)
{
    char *args[4096];
    int argCount = 0;
    int a_flag=0;
    int l_flag=0;
    int al_flag=0;
    while ((token = strtok(NULL, " \t\n")) != NULL) {
                    args[argCount] = token;
                    argCount++;
    }
    // printf("%s\n",args[0]);
    struct dirent *di;
 
     int has_l = 0;  // Flag for 'l' presence
    int has_a = 0;  // Flag for 'a' presence
    

     for (int i = 0; i < argCount; i++) {
        if (strcmp(args[i], "-a") == 0) {
            a_flag = 1;
        }
        if (strcmp(args[i], "-l") == 0) {
            l_flag = 1;
        }

        for (int j = 0; args[i][j] != '\0'; j++) {
            if(args[i][0]=='-')
            {

                if (args[i][j] == 'l') {
                has_l = 1;
            } else if (args[i][j] == 'a') {
                has_a = 1;
            } else if (args[i][j] != '-' && args[i][j] != '.') {
                // If a character is not 'l', 'a', '-', or '.', return
                printf("Invalid flag\n");
                return 0;
            }
            }
        
        }
    }
        if(has_a==1&&has_l==1)
        {
            al_flag=1;
            a_flag=1;
            l_flag=1;
        }







    if (argCount == 0 || strcmp(args[0],".")==0) { // No arguments provided

        DIR *d = opendir(".");
        char *array[1000];
        int i=0;
        while ((di = readdir(d)) != NULL)
        {
            if (di->d_name[0] == '.')
            {
                continue;
            }
            // printf("%s\n", di->d_name);
                            array[i] = (malloc(100 * sizeof(char)));
                strcpy(array[i], di->d_name);
            i++;
        }
        qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_print(array[j]);
                    }
                    closedir(d);

        return 1;

    }

    else if (strcmp(args[0],"..")==0)
    {
        DIR *d = opendir("..");
        // chdir(extract1);
        char * array[1000];
        int i=0;
        while ((di = readdir(d)) != NULL)
        {
            if (di->d_name[0] == '.')
            {
                continue;
            }
            // printf("%s\n", di->d_name);
                array[i] = (malloc(100 * sizeof(char)));
                strcpy(array[i], di->d_name);
                i++;
        }

        qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_print(array[j]);
                    }

        closedir(d);
        return 1;
    }

    else if (strcmp(args[0],"~")==0)
    {
        DIR *d = opendir(og_home);
        // chdir(extract1);
        while ((di = readdir(d)) != NULL)
        {
            if (di->d_name[0] == '.')
            {
                continue;
            }
            printf("%s\n", di->d_name);
        }
        closedir(d);
        return 1;
    }

else
    {

            DIR *d;
            if(a_flag==0 && l_flag==1)
            {   
                    if(argCount==1)
                    { d = opendir(".");

                    char *array[1000];
                    int i = 0;
                    while ((di = readdir(d)) != NULL)
                    {
                        if (di->d_name[0] == '.')
                        {
                            continue;
                        }
                        array[i] = (malloc(100 * sizeof(char)));
                        strcpy(array[i], di->d_name);
                        // array[i]=*strdup(di->d_name);
                        i += 1;
                    }

                    qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_l_print(array[j]);
                    }
                    closedir(d);
                    return 1;
                    }
                    else{
                        
                        char temp[100000];
                        char dir[100000];

                        if(args[1][0]=='~')
                        {
                            strcpy(temp, og_home);
                            getcwd(dir,100000);
                            strcat(temp, args[1] + 1);
                            // printf("%s\n",temp);
                            // printf("%s\n",args[1]);
                            // d=opendir(args[1]);
                        }
                        else{
                            strcpy(temp, args[1]);

                        }
                        getcwd(dir,100000);
                        printf("%s\n",temp);
                        d=opendir(temp);
                        if(d==NULL)
                        {
                            printf("No such directory exists\n");
                            return 0;
                        }
                    char *array[1000];
                        int i = 0;
                        while ((di = readdir(d)) != NULL)
                        {
                            if (di->d_name[0] == '.')
                            {
                                continue;
                            }
                            array[i] = (malloc(100 * sizeof(char)));
                            strcpy(array[i], di->d_name);
                            // array[i]=*strdup(di->d_name);
                            i += 1;
                        }
                        chdir(temp);

                    qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_l_print(array[j]);
                    }
                    chdir(dir);
                    return 1;
                    }

            }


            else if(a_flag==1 && l_flag==0)
            {   
                    if(argCount==1)
                    { d = opendir(".");

                    char *array[1000];
                    int i = 0;
                    while ((di = readdir(d)) != NULL)
                    {

                        array[i] = (malloc(100 * sizeof(char)));
                        strcpy(array[i], di->d_name);
                        // array[i]=*strdup(di->d_name);
                        i += 1;
                    }

                    qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_print(array[j]);
                    }

                    }
                    else{
                        
                        char temp[100000];
                        char dir[100000];

                        if(args[1][0]=='~')
                        {
                            strcpy(temp, og_home);
                            getcwd(dir,100000);
                            strcat(temp, args[1] + 1);

                        }
                        else{
                            strcpy(temp, args[1]);

                        }
                        getcwd(dir,100000);

                        d=opendir(temp);
                        if(d==NULL)
                        {
                            printf("No such directory exists\n");
                            return 0;
                        }

                    char *array[1000];
                        int i = 0;
                        while ((di = readdir(d)) != NULL)
                        {

                            array[i] = (malloc(100 * sizeof(char)));
                            strcpy(array[i], di->d_name);
                            i += 1;
                        }
                        chdir(temp);

                    qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_print(array[j]);
                    }
                    chdir(dir);
                    }
                    return 1;

            }

        else if(argCount==1 && al_flag==1)
        {
            d = opendir(".");

                    char *array[1000];
                    int i = 0;
                    while ((di = readdir(d)) != NULL)
                    {

                        array[i] = (malloc(100 * sizeof(char)));
                        strcpy(array[i], di->d_name);
                        // array[i]=*strdup(di->d_name);
                        i += 1;
                    }

                    qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_l_print(array[j]);
                    }
        }

        else if(argCount==2 && al_flag==1 && args[1][0]!='-')
        {
            char temp[100000];
                        char dir[100000];
                        // printf("%s\n",args[2]);
                        if(args[1][0]=='~')
                        {
                            strcpy(temp, og_home);
                            getcwd(dir,100000);
                            strcat(temp, args[2] + 1);

                        }
                        else{
                            strcpy(temp, args[1]);

                        }
                        getcwd(dir,100000);
                        // printf("%s\n",temp);
                        d=opendir(temp);
                        if(d==NULL)
                        {
                            printf("No such directory exists\n");
                            return 0;
                        }

                    char *array[1000];
                        int i = 0;
                        while ((di = readdir(d)) != NULL)
                        {
                            array[i] = (malloc(100 * sizeof(char)));
                            strcpy(array[i], di->d_name);
                            // array[i]=*strdup(di->d_name);
                            i += 1;
                        }
                        chdir(temp);

                    qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_l_print(array[j]);
                    }
                    chdir(dir);
                    return 1;
        }



            else if((a_flag==1 && l_flag==1) || (al_flag==1 && l_flag==1) || (al_flag==1 && a_flag==1) )
            {
                if(argCount==2)
                {
                    d = opendir(".");

                    char *array[1000];
                    int i = 0;
                    while ((di = readdir(d)) != NULL)
                    {

                        array[i] = (malloc(100 * sizeof(char)));
                        strcpy(array[i], di->d_name);
                        // array[i]=*strdup(di->d_name);
                        i += 1;
                    }

                    qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_l_print(array[j]);
                    }
                }
                else if(argCount==3)
                {
                    char temp[100000];
                        char dir[100000];
                        // printf("%s\n",args[2]);
                        if(args[2][0]=='~')
                        {
                            strcpy(temp, og_home);
                            getcwd(dir,100000);
                            strcat(temp, args[2] + 1);

                        }
                        else{
                            strcpy(temp, args[2]);

                        }
                        getcwd(dir,100000);
                        // printf("%s\n",temp);
                        d=opendir(temp);
                        if(d==NULL)
                        {
                            printf("No such directory exists\n");
                            return 0;
                        }

                    char *array[1000];
                        int i = 0;
                        while ((di = readdir(d)) != NULL)
                        {
                            array[i] = (malloc(100 * sizeof(char)));
                            strcpy(array[i], di->d_name);
                            // array[i]=*strdup(di->d_name);
                            i += 1;
                        }
                        chdir(temp);

                    qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_l_print(array[j]);
                    }
                    chdir(dir);
                    return 1;
                }
                }
            else if(al_flag )
            {   
                    if(argCount==1)
                    { d = opendir(".");

                    char *array[1000];
                    int i = 0;
                    while ((di = readdir(d)) != NULL)
                    {

                        array[i] = (malloc(100 * sizeof(char)));
                        strcpy(array[i], di->d_name);
                        // array[i]=*strdup(di->d_name);
                        i += 1;
                    }

                    qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_l_print(array[j]);
                    }

                    }
                    else{
                        
                        char temp[100000];
                        char dir[100000];

                        if(args[1][0]=='~')
                        {
                            strcpy(temp, og_home);
                            getcwd(dir,100000);
                            strcat(temp, args[1] + 1);

                        }
                        else{
                            strcpy(temp, args[1]);

                        }
                        getcwd(dir,100000);

                        d=opendir(temp);
                        if(d==NULL)
                        {
                            printf("No such directory exists\n");
                            return 0;
                        }

                    char *array[1000];
                        int i = 0;
                        while ((di = readdir(d)) != NULL)
                        {
                            array[i] = (malloc(100 * sizeof(char)));
                            strcpy(array[i], di->d_name);
                            // array[i]=*strdup(di->d_name);
                            i += 1;
                        }
                        chdir(temp);

                    qsort(array, i, sizeof(char *), compare);

                    for (int j = 0; j < i; ++j)
                    {
                        ls_l_print(array[j]);
                    }
                    chdir(dir);
                    return 1;
                    }

            }

            
         else {
        printf("Command Doesnt Exist\n");
        return 0;
    }




    }


    // else if(strcmp (args[0],"-a")==0 || strcmp (args[0],"-l")==0 || strcmp (args[0],"-al")==0|| strcmp (args[0],"-la")==0 )
    // {

    //         DIR *d;
    //         if(strcmp (args[0],"-l")==0)
    //         {   
    //                 if(argCount==1)
    //                 { d = opendir(".");

    //                 char *array[1000];
    //                 int i = 0;
    //                 while ((di = readdir(d)) != NULL)
    //                 {
    //                     if (di->d_name[0] == '.')
    //                     {
    //                         continue;
    //                     }
    //                     array[i] = (malloc(100 * sizeof(char)));
    //                     strcpy(array[i], di->d_name);
    //                     // array[i]=*strdup(di->d_name);
    //                     i += 1;
    //                 }

    //                 qsort(array, i, sizeof(char *), compare);

    //                 for (int j = 0; j < i; ++j)
    //                 {
    //                     ls_l_print(array[j]);
    //                 }
    //                 closedir(d);
    //                 return 1;
    //                 }
    //                 else{
                        
    //                     char temp[100000];
    //                     char dir[100000];

    //                     if("args[1][0]=='~'")
    //                     {
    //                         strcpy(temp, og_home);
    //                         getcwd(dir,100000);
    //                         strcat(temp, args[1] + 1);
    //                         // printf("%s\n",temp);
    //                         // printf("%s\n",args[1]);
    //                         // d=opendir(args[1]);
    //                     }
    //                     else{
    //                         strcpy(temp, args[1]);

    //                     }
    //                     getcwd(dir,100000);

    //                     d=opendir(temp);
    //                     if(d==NULL)
    //                     {
    //                         printf("No such directory exists\n");
    //                         return 0;
    //                     }
    //                 char *array[1000];
    //                     int i = 0;
    //                     while ((di = readdir(d)) != NULL)
    //                     {
    //                         if (di->d_name[0] == '.')
    //                         {
    //                             continue;
    //                         }
    //                         array[i] = (malloc(100 * sizeof(char)));
    //                         strcpy(array[i], di->d_name);
    //                         // array[i]=*strdup(di->d_name);
    //                         i += 1;
    //                     }
    //                     chdir(temp);

    //                 qsort(array, i, sizeof(char *), compare);

    //                 for (int j = 0; j < i; ++j)
    //                 {
    //                     ls_l_print(array[j]);
    //                 }
    //                 chdir(dir);
    //                 return 1;
    //                 }

    //         }


    //         else if(strcmp (args[0],"-a")==0)
    //         {   
    //                 if(argCount==1)
    //                 { d = opendir(".");

    //                 char *array[1000];
    //                 int i = 0;
    //                 while ((di = readdir(d)) != NULL)
    //                 {

    //                     array[i] = (malloc(100 * sizeof(char)));
    //                     strcpy(array[i], di->d_name);
    //                     // array[i]=*strdup(di->d_name);
    //                     i += 1;
    //                 }

    //                 qsort(array, i, sizeof(char *), compare);

    //                 for (int j = 0; j < i; ++j)
    //                 {
    //                     ls_print(array[j]);
    //                 }

    //                 }
    //                 else{
                        
    //                     char temp[100000];
    //                     char dir[100000];

    //                     if("args[1][0]=='~'")
    //                     {
    //                         strcpy(temp, og_home);
    //                         getcwd(dir,100000);
    //                         strcat(temp, args[1] + 1);

    //                     }
    //                     else{
    //                         strcpy(temp, args[1]);

    //                     }
    //                     getcwd(dir,100000);

    //                     d=opendir(temp);
    //                     if(d==NULL)
    //                     {
    //                         printf("No such directory exists\n");
    //                         return 0;
    //                     }

    //                 char *array[1000];
    //                     int i = 0;
    //                     while ((di = readdir(d)) != NULL)
    //                     {

    //                         array[i] = (malloc(100 * sizeof(char)));
    //                         strcpy(array[i], di->d_name);
    //                         i += 1;
    //                     }
    //                     chdir(temp);

    //                 qsort(array, i, sizeof(char *), compare);

    //                 for (int j = 0; j < i; ++j)
    //                 {
    //                     ls_print(array[j]);
    //                 }
    //                 chdir(dir);
    //                 }
    //                 return 1;

    //         }

    //         else if(strcmp (args[0],"-al")==0 || strcmp (args[0],"-la")==0)
    //         {   
    //                 if(argCount==1)
    //                 { d = opendir(".");

    //                 char *array[1000];
    //                 int i = 0;
    //                 while ((di = readdir(d)) != NULL)
    //                 {

    //                     array[i] = (malloc(100 * sizeof(char)));
    //                     strcpy(array[i], di->d_name);
    //                     // array[i]=*strdup(di->d_name);
    //                     i += 1;
    //                 }

    //                 qsort(array, i, sizeof(char *), compare);

    //                 for (int j = 0; j < i; ++j)
    //                 {
    //                     ls_l_print(array[j]);
    //                 }

    //                 }
    //                 else{
                        
    //                     char temp[100000];
    //                     char dir[100000];

    //                     if("args[1][0]=='~'")
    //                     {
    //                         strcpy(temp, og_home);
    //                         getcwd(dir,100000);
    //                         strcat(temp, args[1] + 1);

    //                     }
    //                     else{
    //                         strcpy(temp, args[1]);

    //                     }
    //                     getcwd(dir,100000);

    //                     d=opendir(temp);
    //                     if(d==NULL)
    //                     {
    //                         printf("No such directory exists\n");
    //                         return 0;
    //                     }

    //                 char *array[1000];
    //                     int i = 0;
    //                     while ((di = readdir(d)) != NULL)
    //                     {
    //                         array[i] = (malloc(100 * sizeof(char)));
    //                         strcpy(array[i], di->d_name);
    //                         // array[i]=*strdup(di->d_name);
    //                         i += 1;
    //                     }
    //                     chdir(temp);

    //                 qsort(array, i, sizeof(char *), compare);

    //                 for (int j = 0; j < i; ++j)
    //                 {
    //                     ls_l_print(array[j]);
    //                 }
    //                 chdir(dir);
    //                 return 1;
    //                 }

    //         }




    // }






}
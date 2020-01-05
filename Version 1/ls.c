#include "ls.h"
int compile_ls(char **parsed){
    pid_t pid;
        pid=fork();
        if(pid<0){
            return 0;
        }
        if(pid==0){
            if(parsed[1][0]=='-'){
                execl("/bin/ls","ls",parsed[1],(char*)0);

            }
            else if(strcmp(parsed[1],"\0")==0){
                execl("/bin/ls","ls",0,(char*)0);
            }
            else{
                printf("ls: invalid option -- '%c'\n",parsed[1][1]);
            }
        }
        else{
            wait(NULL);
            return 1;
        }
}
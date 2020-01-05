#include "mkdir.h"
int compile_mkdir(char** parsed){
    pid_t pid;
    pid=fork();
    if(pid<0){
        return 0;
    }
    if(pid==0){
        if(parsed[1][0]=='-'){
            execl("/bin/mkdir","mkdir",parsed[1],parsed[2],(char*)0);
        }
        else if(isalpha(parsed[1][0])){
            execl("/bin/mkdir","mkdir",parsed[1],(char*)0);
        }
        else if(strcmp(parsed[1],"\0")==0){
            printf("mkdir: missing operand\n");
        }
        else{
            printf("mkdir: invalid option -- '%c'\n",parsed[1][1]);
        }
    }
    else{
        wait(NULL);
        return 1;
    }
}
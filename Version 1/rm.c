#include "rm.h"
int compile_rm(char** parsed){
    pid_t pid;
    pid=fork();
    if(pid<0){
        return 0;
    }
    if(pid==0){
        if(parsed[1][0]=='-'){
            execl("/bin/rm","rm",parsed[1],parsed[2],(char*)0);
        }
        else if(isalpha(parsed[1][0])){
            execl("/bin/rm","rm",parsed[1],(char*)0);
        }
        else if(strcmp(parsed[1],"\0")==0){
            printf("rm: missing operand\n");
        }

        else{
            printf("rm: invalid option -- '%c'\n",parsed[1][1]);
        }
    }
    else{
        wait(NULL);
        return 1;
    }
}
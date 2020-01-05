#include "external.h"
int compile(char **parsed){
    pid_t pid;
    pid=fork();
    if(pid<0){
        return 0;
    }
    if(pid==0){
        execvp(parsed[0], parsed);
    }
    else{
        wait(NULL);
        return 1;
    }
}
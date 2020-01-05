#include "date.h"
int compile_date(char** parsed){
    pid_t pid;
    pid=fork();
        if(pid<0){
            return 0;
        }
        if(pid==0){
            if(strcmp(parsed[1],"-R")==0){
                execl("/bin/date","date","-R",(char*)0);

            }
            else if(strcmp(parsed[1],"-u")==0){
                execl("/bin/date","date","-u",(char*)0);
            }
            else if(strcmp(parsed[1],"\0")==0){
                execl("/bin/date","date",0,(char*)0);
            }

            else{
                printf("date: invalid option -- '%c'\n",parsed[1][1]);
            }
        }
        else{
            wait(NULL);
            return 1;
        }
}
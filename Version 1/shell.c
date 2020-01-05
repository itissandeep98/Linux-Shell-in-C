#include "shell.h"

FILE *history;
char pwd[100];

char **parse(char* input,int len){
    char** arr = (char**) malloc(sizeof(char*)*100);
    for(int i=0; i<100; ++i){
        arr[i]= (char*) malloc(sizeof(char)*100);
    }
    int i=0,j=0,k=0;
    while(*(input+i) !='\0'){
        if(*(input+i)!=' '){
            arr[j][k]=input[i];
            k+=1;
        }
        else if(*(input+i)==' ' && isspace(*(input+i+1))==0){
            j+=1;
            k=0;
        }
        i+=1;
    }
    return arr;
}

int execute(char** parsed){
    pid_t pid;
    getcwd(pwd,sizeof(pwd));
    if(strcmp(parsed[0],"exit")==0){
        return 0;
    }
    else if(strcmp(parsed[0],"pwd")==0){
        printf("%s\n",pwd);
        return 1;
    }
    else if(strcmp(parsed[0],"cd")==0){
        int r=0;
        if(strcmp(parsed[1],".")==0 ||strcmp(parsed[1],"..")==0){
            chdir(parsed[1]);
        }
        else{
            strcat(pwd,parsed[1]);
            r=chdir(pwd);
        }

        if(r==-1){
            printf("-bash: cd: %s: No such file or directory\n",parsed[1]);
        }
        return 1;
    }
    else if(strcmp(parsed[0],"history")==0){
        char ch;
        fseek(history,0,SEEK_SET);
        while((ch = fgetc(history)) != EOF)
            printf("%c", ch);

    }
    else if(strcmp(parsed[0],"echo")==0){
        char* arr= (char*) malloc(sizeof(char)*100);
        int i=0,j=0,k=0;
        while(*(parsed[2]+i) !='\0'){
            k=0;
            while(*(parsed[2]+i)=='\\'){
                k++;
                i++;
            }
            k=k/2;
            while(k>0){
                k--;
                arr[j]='\\';
                j++;
            }
            arr[j]=parsed[2][i];
            i++;
            j++;
        }

        if(strcmp(parsed[1],"-n")==0){
            printf("%s",arr);
        }
        else{
            if(strcmp(parsed[1],"-E")==0){
                printf("%s\n",arr);
            }
            else{
                char* arr= (char*) malloc(sizeof(char)*100);
                int i=0,j=0,k=0;
                while(*(parsed[1]+i) !='\0'){
                    k=0;
                    while(*(parsed[1]+i)=='\\'){
                        k++;
                        i++;
                    }
                    k=k/2;
                    while(k>0){
                        k--;
                        arr[j]='\\';
                        j++;
                    }
                    arr[j]=parsed[1][i];
                    i++;
                    j++;
                }
                printf("%s\n",arr);
            }
        }
    }
    else if(strcmp(parsed[0],"ls")==0){
        return compile_ls(parsed);
    }
    else if(strcmp(parsed[0],"date")==0){
        return compile_date(parsed);
    }
    else if(strcmp(parsed[0],"mkdir")==0){
        return compile_mkdir(parsed);
    }
    else if(strcmp(parsed[0],"rm")==0){
        return compile_rm(parsed);
    }
    else if(strcmp(parsed[0],"cat")==0){
        return compile_cat(parsed);
    }
    else{
        printf("%s: command not found\n",parsed[0]);
        return 1;
    }
}

void loop(void){
    int status;
    char input[100];
    char** parsed;

    do{
        getcwd(pwd,sizeof(pwd));
        status=0;
        printf("Sandeep@CAOS:%s$ ",pwd);
        fgets(input,100,stdin);
        input[strcspn(input,"\n")]=0;
        fseek(history,0,SEEK_END);
        fprintf(history,"%s\n",input);
        parsed=parse(input,sizeof(input)/sizeof(char));
        status=execute(parsed);
    }while(status);
}

int main(){
    history=fopen("history.txt","a+");
    if(history==NULL){
        printf("error");
        exit(0);
    }
    loop();
    fclose(history);
    return 0;
}

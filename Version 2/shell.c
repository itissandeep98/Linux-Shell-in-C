#include "shell.h"

FILE *history;
char history_loctaion[100];
char pwd[100];
int count;
int last_history_num=0;

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
    arr[j+1]=NULL;
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
        else if(strcmp(parsed[1],"\0")==0){
            chdir("/");
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
        if(strcmp(parsed[1],"-c")==0){
            history=fopen(history_loctaion,"w");
            fclose(history);
            last_history_num=0;
        }

        else{
            history=fopen(history_loctaion,"a+");
            fseek(history,0,SEEK_SET);
            while((ch = fgetc(history)) != EOF)
                printf("%c", ch);
            printf("\n");
            fclose(history);
        }
        return 1;
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
    else if(strcmp(parsed[0],"ls")==0 || strcmp(parsed[0],"date")==0|| strcmp(parsed[0],"mkdir")==0|| strcmp(parsed[0],"rm")==0||strcmp(parsed[0],"cat")==0){
        return compile(parsed);
    }
    else{
        printf("%s: command not found\n",parsed[0]);
        return 1;
    }
}

void loop(void){
    int status=1;
    char input[100];
    char** parsed;
    pid_t pid;

    do{
        getcwd(pwd,sizeof(pwd));
        printf("Sandeep@CAOS:%s$ ",pwd);
        fgets(input,100,stdin);
		input[strlen(input)]=' ';
        input[strcspn(input,"\n")]=0;
        //entering history
        history=fopen(history_loctaion,"a+");
        fseek(history,0,SEEK_END);
        last_history_num++;
        fprintf(history,"\n%d %s",last_history_num ,input);
        fclose(history);
        //splitting the input with delimiter as space
        parsed=parse(input,sizeof(input)/sizeof(input[0]));
        //execution of command
        status=execute(parsed);
    }while(status);
}

int main(){
    getcwd(history_loctaion,sizeof(history_loctaion));
    strcat(history_loctaion,"/history");
    //for finding the number of entries in file
	int count=0;
    history=fopen(history_loctaion,"a+");
	fseek(history,0,SEEK_SET);
	char ch=fgetc(history);

	while(ch !=EOF){
		if(ch=='\n'){
			count++;
		}
		ch=fgetc(history);
	}
	fclose(history);
	last_history_num=count;
    loop();
    return 0;
}

/*csd4497 stavros giakoumakis*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
enum redirection{NONE,LEFT='<',RIGHT='>',RIGHTRIGHT=1};
void term_print();
void multiples_check(char*string);
void simple_command_run(char*command,int flag,int);
void run_pipe();
/*Main function,gets input from user sanitizes it and calls multiples_check*/
int main(){
    while(1){
        int i=0;
        char command[1024];
        term_print();
        fgets(command,1024,stdin);
        command[strcspn(command, "\n")] = 0;
        multiples_check(command);
    }
}

void term_print(){
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("csd4497-hy345sh@%s:%s/$ ", getenv("USER"),cwd);
    //fflush(stdout);
}

/*splits user input into multple commands(if they exist).Sends the commands to simple_command_run with the according flag(0 if normal command.1 if piped,3 if redirected)*/
 void multiples_check(char*string){
    int functions_counter=0;
    char*end_string;
    char *token = strtok_r(string, ";", &end_string);
    while(token!=NULL){
        int pipecounter=0;
        enum redirection flag=NONE;
        for(int i=0;i<strlen(token);i++){
            if(token[i]=='|'){
            pipecounter++;
            }else if(token[i]==LEFT){
                flag=LEFT;
            }else if(token[i]==RIGHT){
                if((i+1)<strlen(token) && token[i+1]==RIGHT){
                    flag=RIGHTRIGHT;
                }else{
                    flag=RIGHT;
                }
            }
        }
        if(pipecounter>1){
            continue;
        }
        if(pipecounter==1){
            char*end_piped;
            char* piped=strtok_r(token,"|",&end_piped);
            int current_pipe=0;
            while(piped!=NULL){
                simple_command_run(piped,1,current_pipe);
                current_pipe=1;
                piped=strtok_r(NULL,"|",&end_piped);
            }
        }else{
            if(flag){
                simple_command_run(token,3,0);
            }else{
                simple_command_run(token,0,0);
            }
        }
        token = strtok_r(NULL, ";", &end_string);
    }
}  
/*bad pipe implementation,only  1 pipe*/
    char** pipe1;
    char** pipe2;
/*Most of the code,manually checks for quit/chdir,then,it takes 
each command and splits it into an array(including any arguments, e.g ls -Q -> ["ls","-Q",NULL] or ls -r > test.txt -> ["ls","-r",">","test.txt",NULL])
and depending on the flag it gets from multiple_checks acts accordingly.*/
  void simple_command_run(char*command,int flag,int current_pipe){
    char** carguments;
    int argcounter=1;
    for(int i=0;i<strlen(command);i++){
        if(command[i]==' '){
            argcounter++;
        }
    }
    carguments=malloc((1+argcounter)*sizeof(char*));
    char* test=strtok(command," ");
    if(!strcmp(test,"quit")){
        exit(0);
    }else{
        int i=0;
        while(test!=NULL){
            carguments[i]=test;
            test=strtok(NULL," ");
            i++;
        }
        if(!strcmp(carguments[0],"chdir")){
            chdir(carguments[1]);
            return;
        }
        carguments[argcounter]=NULL;
        if(flag==1){
                if(current_pipe==0){
                    pipe1=carguments;
                    return;
                }else{
                    pipe2=carguments;
                    run_pipe();
                    return;
                }
        }else if(flag==0){
                pid_t p = fork();
                if(p==0){
                    execvp(carguments[0],carguments);
                    exit(0);
                }else{
                    wait(NULL);
                }
        }else{
                int i=0;
                int symbol_pos=-1;
                while(carguments[i]!=NULL){
                    if(!strcmp(carguments[i],">")){
                        symbol_pos=i;
                        carguments[symbol_pos]=NULL;
                        int file=open(carguments[symbol_pos+1],O_WRONLY | O_CREAT,777);
                        pid_t p = fork();
                        if(p==0){
                            dup2(file,STDOUT_FILENO);
                            execvp(carguments[0],carguments);
                            exit(0);
                        }else{
                            wait(NULL);
                        }
                        break;
                    }else if(!strcmp(carguments[i],"<")){
                        symbol_pos=i;
                        carguments[symbol_pos]=NULL;
                        int file=open(carguments[symbol_pos+1],O_RDONLY | O_CREAT,777);
                        pid_t p = fork();
                        if(p==0){
                            dup2(file,STDIN_FILENO);
                            execvp(carguments[0],carguments);
                            exit(0);
                        }else{
                            wait(NULL);
                        }
                        break;
                    }else if(!strcmp(carguments[i],">>")){
                        symbol_pos=i;
                        carguments[symbol_pos]=NULL;
                        int file=open(carguments[symbol_pos+1], O_WRONLY | O_APPEND,777);
                        pid_t p = fork();
                        if(p==0){
                            dup2(file,STDOUT_FILENO);
                            execvp(carguments[0],carguments);
                            exit(0);
                        }else{
                            wait(NULL);
                        }
                        break;
                    }
                    i++;
                }
        }
    }
    
}  


/*my very bad pipe implementation */
void run_pipe(){
    int fd[2];
    pipe(fd);
    int pid=fork();
    if(pid==0){
        dup2(fd[1],STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(pipe1[0],pipe1);
    }
    int pid1=fork();
    if(pid1==0){
        dup2(fd[0],STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvp(pipe2[0],pipe2);
    }
    close(fd[1]);
    close(fd[0]);
    waitpid(pid,NULL,0);
    waitpid(pid1,NULL,0);
}
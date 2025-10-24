#include<stdio.h>
#include<time.h>
#include<stdlib.h>

int move(char *memory,char **maze,int v,int m,int a,int b,int countmemor);


int main(){
    int a,b,i,v,m,j,countmemor=0;
    char **maze;
    char *memory;
    srand(time(NULL));
    scanf("%d %d",&a,&b);
    memory=malloc(((a*b)/2)*(sizeof(char)));
    maze=malloc(a*sizeof(char*));
    for(i=0;i<a;i++){
        maze[i]=malloc(b*sizeof(char));
    }
    for(i=0;i<a;i++){
        for(j=0;j<b;j++){
            maze[i][j]='X';
        }
    } 
    maze[0][1]=' ';
    maze[1][1]=' '; 
    v=1;
    m=1;
    move(memory,maze,v,m,a,b,countmemor);
    free(memory);
    for(i=0;i<a;i++){
        free(maze[i]);
    }
    free(maze);
}


int move(char *memory,char **maze,int v,int m,int a,int b,int countmemor){
    int i,j,k,left,right,up,down,rc,counter;
    char dir[4];
        up=0;
        down=0;
        right=0;
        left=0;
        counter=0;
    if((v-2)>0){
        if(maze[v-2][m]=='X' && maze[v-1][m]=='X'){
      up=1;
    }
  }
   if((m-2)>0){                                                                                   
       if(maze[v][m-2]=='X' && maze[v][m-1]=='X'){
      left=1;
       }
  }
  if(m+2<b-1){
      if(maze[v][m+2]=='X' && maze[v][m+1]=='X'){                         
      right=1;
      }
  }
  if(v+2<a-1){
      if(maze[v+2][m]=='X' && maze[v+1][m]=='X'){
      down=1;
      }
  }
 if(left==1){
     dir[counter]='l';
     counter++;
 }
 if(right==1){
     dir[counter]='r';
     counter++;
 }

 if(up==1){                                                       
     dir[counter]='u';
     counter++;
 }
 if(down==1){
     dir[counter]='d';
     counter++;
 }
 if(counter!=0){
 switch(dir[rand()%counter]){
 case 'l':
     maze[v][m-2]=' ';
     maze[v][m-1]=' ';
     m=m-2;
     memory[countmemor]='l';
     countmemor++;
     break;
 case 'r':
     maze[v][m+1]=' ';
     maze[v][m+2]=' ';
     m=m+2;
     memory[countmemor]='r';
     countmemor++;
     break;
 case'u':
     maze[v-2][m]=' ';
     maze[v-1][m]=' ';
     v=v-2;
     memory[countmemor]='u';
     countmemor++;
     break;
 
 case'd':
   maze[v+2][m]=' ';
   maze[v+1][m]=' ';
   v=v+2;
   memory[countmemor]='d';
   countmemor++;
   break;
 }
 }
 if(counter==0){
   switch(memory[(countmemor-1)]){
   case 'l':
   k=countmemor-1;
   m=m+2;
   break;
   case'r':
   k=countmemor-1;
   m=m-2;
   break;
   case 'u':
   k=countmemor-1;
   v=v+2;
   break;
   case 'd':
   v=v-2;
   k=countmemor-1;
   break;
   
   }
   countmemor=k;
 }

    if(v==1 && m==1){
        for(i=0;i<a;i++){
        for(j=0;j<b;j++){
            printf("%c",maze[i][j]);
        }
        printf("\n");
    }
    return 0;
    }
    else{
        return move(memory,maze,v,m,a,b,countmemor);
    }
}
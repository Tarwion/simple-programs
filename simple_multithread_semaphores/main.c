/*csd4497 stavros giakoumakis*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include<unistd.h>
#include <semaphore.h>
#include <string.h>
sem_t semaphore;
pthread_mutex_t counter_mutex,reading_mutex,reading_mutex1;
int counter=0;
int reading[8]={0};
int waiting[40]={0};
int test=0;
int test1=0;
struct waiting_room{
    int am;
    int wait;
    struct waiting_room* next;
}*read_print=NULL,*wait_print=NULL;
typedef struct waiting_room waiting_room;

/*helping functs*/
void reading_print(int*arr){
    printf("reading room: ");
    for(int i=0;i<8;i++){
        printf("|%d",arr[i]);
    }
    printf("|\n");
}
void reading_remove(int val){
    for(int i=0;i<8;i++){
        if(reading[i]==val){
            reading[i]=0;
            return;
        }
    }
}

void waiting_print(int*arr){
    printf("waiting room: ");
    for(int i=0;i<40;i++){
        printf("|%d",arr[i]);
    }
    printf("|\n");
}
void waiting_remove(int val){
    for(int i=0;i<40;i++){
        if(waiting[i]==val){
            waiting[i]=0;
            return;
        }
    }
}


/*student logic*/
void* student(void*arg){
    waiting_room* student=(waiting_room*) arg;
    pthread_mutex_lock(&reading_mutex1);
    waiting[test1]=student->am;
    test1++;
    printf("student %d was born and entered waiting room\n",student->am);
    reading_print(reading);
    waiting_print(waiting);
    pthread_mutex_unlock(&reading_mutex1);
    sem_wait(&semaphore);
    pthread_mutex_lock(&reading_mutex);
    reading[test]=student->am;
    printf("student %d entered the reading room\n",student->am);
    waiting_remove(student->am);
    reading_print(reading);
    waiting_print(waiting);
    test++;
    pthread_mutex_unlock(&reading_mutex);
    sleep(student->wait);
    pthread_mutex_lock(&counter_mutex);
    printf("student %d left the reading room\n",student->am);
    reading_remove(student->am);
    reading_print(reading);
    waiting_print(waiting);

    if(counter<7){
        counter++;
    }else{
        counter=0;
        test=0;
        test1=0;
        for(int i=0;i<8;i++){
            sem_post(&semaphore);
        }
    }
    pthread_mutex_unlock(&counter_mutex);
}

/*gets the no of students and initializes stuff*/
int main(){
    waiting_room *head,*temp;
    pthread_t *pstudents;
    int students=0;
    srand(time(NULL));
    sem_init(&semaphore, 0, 8);
    pthread_mutex_init(&counter_mutex,NULL);
    pthread_mutex_init(&reading_mutex,NULL);
    pthread_mutex_init(&reading_mutex1,NULL);
    printf("how many students? \n");
    scanf("%d",&students);
    if(students<1){
        return 0;
    }
    pstudents=malloc(students*sizeof(pthread_t));
    head=malloc(sizeof(waiting_room));
    temp=head;
    for(int i=0;i<students;i++){
        temp->am=rand() % (6000 - 2000 + 1) + 2000;
        temp->wait=rand() % (15 - 5 + 1) + 5;
        if(i==students-1){
            temp->next=NULL;
        }else{
            temp->next=malloc(sizeof(waiting_room));
            temp=temp->next;
        }
    }
    for(int i=0;i<students;i++){
        waiting_room*temp1=malloc(sizeof(waiting_room));
        temp1->am=head->am;
        temp1->wait=head->wait;
        temp1->next=head->next;
        sleep(0.5);
        pthread_create(&pstudents[i], NULL, &student, temp1);
        head=head->next;
    }
    for (int i = 0; i < students; i++) {
        if (pthread_join(pstudents[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
}
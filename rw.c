#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

sem_t rw_mutex;
pthread_mutex_t mutex;
int blackboard = 1; // Blackboard originally has content of "1"
int read_count = 0; // Number of readers are reading blackboard simultaneously
int reader_count = 10;
int writer_count = 5;

void *writer(void *wno)
{   
    sem_wait(&rw_mutex);
    blackboard = blackboard*2;
    printf("Writer %d updated blackboard to %d\n",(*((int *)wno)),blackboard);
    sem_post(&rw_mutex);
}

void *reader(void *rno)
{   
    pthread_mutex_lock(&mutex);
    read_count++;
    if(read_count == 1) {
        sem_wait(&rw_mutex); 
    }
    pthread_mutex_unlock(&mutex);
    printf("Reader %d: reads blackboard as %d\n",*((int *)rno),blackboard);
    pthread_mutex_lock(&mutex);
    read_count--;
    if(read_count == 0) {
        sem_post(&rw_mutex);
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{
    printf("Blackboard originally has content of %d.\n", blackboard);
    printf("There come %d readers and %d writers...\n", reader_count, writer_count);
    pthread_t read[reader_count], write[writer_count];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&rw_mutex,0,1);
    
    int r[reader_count], w[writer_count];
    for(int j=0;j<reader_count;j++){
        r[j]=j+1;
    }
    for(int k=0;k<writer_count;k++){
        w[k]=k+1;
    }

    int i=0, j=0, k=0, n;
    srand(time(NULL));    
    while(i<reader_count+writer_count){
        n = rand();
        if (n%2==0 && j<reader_count) {
            pthread_create(&read[j], NULL, (void *)reader, (void *)&r[j]);
            j++; i++;
        } else if (k<writer_count) {
            pthread_create(&write[k], NULL, (void *)writer, (void *)&w[k]);
            k++; i++;
        }
    }
    
    for(int i = 0; i < reader_count; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < writer_count; i++) {
        pthread_join(write[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&rw_mutex);
    return 0;
}

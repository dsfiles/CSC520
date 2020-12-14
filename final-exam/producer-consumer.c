/*
 * This program simulates the producer-consumer problem using POSIX mutex and semaphore.
 * 
 * buffer size remains the same
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#define BUFFSIZE 5
sem_t empty; //semaphore empty
sem_t full;  //semaphore full
int buffer[BUFFSIZE];
pthread_mutex_t mutex; //mutex lock
int in = 0;
int out = 0;
int item = 0;

void* producer(void* p)
{
    for (int i = 0; i < 6; i++)
    {
        sem_wait(&empty); //wait operation on semaphore empty
        pthread_mutex_lock(&mutex); //lock mutex
        buffer[in] = item; //insert the item to the buffer
        item = item + 1;
        printf("producer %d: insert item %d at\tbuffer[%d]\n", *((int*)p), buffer[in], in);
        in = (in+1) % BUFFSIZE;
        pthread_mutex_unlock(&mutex); //unlock mutex
        sem_post(&full); //signal operation on emaphore full
    }
}

void* consumer(void* c)
{
    for (int i = 0; i < 4; i++)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item_removed = buffer[out];
        printf("consumer %d: remove item %d from buffer[%d]\n", *((int*)c), item_removed, out);
        out = (out+1) % BUFFSIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main()
{
    pthread_t p[4], c[6];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFSIZE);
    sem_init(&full, 0, 0);
    int id;
    int a[6] = { 1, 2, 3, 4, 5, 6 }; //Keep track of the producer and consumer
    for (int i = 0; i < 4; i++)
    { //create 4 producers
        pthread_create(&p[i], NULL, (void*)producer, (void*)&a[i]);
    }
    for (int i = 0; i < 6; i++)
    { //create 6 consumers
        pthread_create(&c[i], NULL, (void*)consumer, (void*)&a[i]);
    }
    for (int i = 0; i < 4; i++)
    {
        pthread_join(p[i], NULL);
    }
    for (int i = 0; i < 6; i++)
    {
        pthread_join(c[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}

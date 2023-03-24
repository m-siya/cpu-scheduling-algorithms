#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t empty, full;
pthread_mutex_t mutex;
buffer *b;

void *producer();
void *consumer() ;
/* the buffer */

buffer *create_buffer(int buffer_size)
{
    buffer *b = (buffer *)malloc(sizeof(buffer));
    b -> buffer_size = buffer_size;
    b -> count = 0;
    b -> out = 0;
    b -> in = 0;
    b -> items = (buffer_item *)calloc(buffer_size, sizeof(buffer_item));
    return b;
}

int insert_item(buffer *b, buffer_item item)
{
    if (b -> count == b -> buffer_size)
        return -1;
    else
    {
        b -> items[b -> in] = item;
        b -> in = (b -> in + 1) % b -> buffer_size;
        b -> count++;
        return 0;
    }
}

int remove_item(buffer *b, buffer_item item)
{
    if (b -> count == 0)
        return -1;
    else
    {
        b -> out = (b -> out + 1) % b -> buffer_size;
        b -> count--;
        return 0;
    }
}

int main(int argc, char *argv[])
{
    /* get command line arguments */
    int sleep_time = atoi(argv[1]);
    int producer_count = atoi(argv[2]);
    int consumer_count = atoi(argv[3]);

    /* initialize buffer */
    b = create_buffer(10);

    /* create semaphores */

    /* prototype of sem_init() :
    *  int	sem_init(sem_t *sem, int pshared, unsigned int value);
    *  if pshared is 0, semaphore cannot be shared between threads/processes. 
       else, if non-zero, can be shared between threads/processes
    */
    
    sem_init(&empty, 1, 10); 
    sem_init(&full, 1, 0);

    /* create mutex lock */
    
    pthread_mutex_init(&mutex, NULL);

    /* create producer threads */
    pthread_t *producers = malloc(sizeof(pthread_t) * producer_count);

    for (int i = 0; i < producer_count; i++)
    {
        pthread_create(&producers[i], NULL, producer, NULL);
    }


    /* create consumer threads */
    pthread_t *consumers = malloc(sizeof(pthread_t) * consumer_count);

    for (int i = 0; i < consumer_count; i++)
    {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

 


    sleep(sleep_time);

}

void *producer() 
{
    buffer_item item;

    while (1) 
    {
        /* sleep for a random period of time */
        int sleep_time = rand() % 5;
        sleep(sleep_time);


        /* generate a random number */
        item = rand();

        sem_wait(&empty);
        pthread_mutex_lock(&mutex); //acquire mutex lock

        /* critical section */
        if (insert_item(b, item) == -1)
            printf("Item was not inserted");
        else
            printf("Producer produced %d \n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer() 
{
    buffer_item item;

    while (1) 
    {
        /* sleep for a random period of time */
        int sleep_time = rand() % 5;
        sleep(sleep_time);


        /* generate a random number */
        item = rand();

        sem_wait(&full);
        pthread_mutex_lock(&mutex); //acquire mutex lock

        /* critical section */
        if (remove_item(b, item) == -1)
            printf("Item was not inserted");
        else
            printf("Producer produced %d \n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}
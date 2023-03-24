#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 10

typedef struct {

    int value;

} item;
item buffer[BUFFER_SIZE];

int in = 0;
int out = 0;
void *run_producer(void *production_limit);
void *run_consumer(void * production_limit);

int main(int argc, char *argv[]){
    pthread_t producer, consumer;
    int production_limit = 7;
    char *producer_prompt = "Producer starting up...";
    char *consumer_prompt = "Consumer starting up...";
 
    pthread_create(&producer, NULL, run_producer, (void *)&production_limit);
    pthread_create(&consumer, NULL, run_consumer, (void *)&production_limit);
    
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    exit(0);

}

void *run_producer(void* production_limit){
    int * limit = (int *)production_limit;
    item next_produced = {11};
    int count = 0;

    while (count < *limit) {
        printf("Producer checks buffer\n");

        /* produce an item in next_produced */
        if (((in + 1) % BUFFER_SIZE) == out)
            printf("Buffer is full !\n"); 
            /* do nothing */
        else{
            printf("Producing item...\n");
            buffer[in] = next_produced;
            in = (in + 1) % BUFFER_SIZE;
        }
        count++;      
    }
    pthread_exit(0);

}

void *run_consumer(void* production_limit){
    int * limit = (int *)production_limit;
    item next_consumed = {11};
    int count = 0;
    while (count < *limit){
        printf("Consumer checks buffer\n");

        if (in == out)
            printf("Buffer is empty !\n"); 
            /* do nothing */
        else{
            /*consume the item in next_consumed*/
            printf("Consuming item...\n");
            next_consumed = buffer[out];
            out = (out + 1) % BUFFER_SIZE;
        }      
        count++;
    }    
    pthread_exit(0);
}

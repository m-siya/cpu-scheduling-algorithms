//Write a program to demonstrate peterson's solution for 2 processes

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

/* gloabl variables*/
int turn;
int flag[2];
buffer *b;

void *producer();
void *consumer();

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

int remove_item(buffer *b)
{
    if (b -> count == 0)
        return -1;
    else
    {
        buffer_item item = b -> items[b -> out];
        b -> out = (b -> out + 1) % b -> buffer_size;
        b -> count--;
        return item;
    }
}

int main(int argc, char *argv[])
{
    /* get command line arguments */
    int sleep_time = atoi(argv[1]);

    /* initialize buffer */
    b = create_buffer(10);

    /* initialize the shared variables*/
    turn = 0; //setting turn to producer first
    flag[0] = 0;
    flag[1] = 0; 

    /* create producer threads */
    pthread_t p;

    pthread_create(&p, NULL, producer, NULL);

    pthread_t c;
    pthread_create(&c, NULL, consumer, NULL);

    sleep(sleep_time);
    exit(0);

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
        item = rand() % 100;

        /*make flag[i] = true to signal it is ready to enter
        critical section */
        flag[0] = 1;
        
        /* set turn = other process's turn to let it enter
        CS if it wishes to do so first*/
        turn = 1;

        while (flag[1] && turn == 1) {
            ; //busy waiting
        }

        /* critical section */
        if (insert_item(b, item) == -1)
            printf("Item was not inserted");
        else
            printf("Producer produced %d \n", item);

        flag[0] = 0;
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

        /*make flag[i] = true to signal it is ready to enter
        critical section */
        flag[1] = 1;
        
        /* set turn = other process's turn to let it enter
        CS if it wishes to do so first*/
        turn = 0;

        while (flag[0] && turn == 0) {
            ; //busy waiting
        }

        /* critical section */
        buffer_item item_consumed = remove_item(b);
        if (item_consumed == -1)
            printf("Item was not removed");
        else
            printf("Consumer consumed %d \n", item_consumed);


        flag[1] = 0;
    }
}


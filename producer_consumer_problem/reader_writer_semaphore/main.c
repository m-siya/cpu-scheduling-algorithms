/* implement the semaphore solution to the first reader-writer's problem.
* If a reader is reading and 
*   - another reader comes, the new reader can access the data
*    - a writer comes, the writer must wait until no reader's are waiting to access
*    data
*
* If a writer is writing and
*    - a reader comes, the reader must wait until writer is finished
*    - another writer comes, must wait in queue
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include "buffer.h"
#include <unistd.h>

sem_t rw_mutex, mutex;
int read_count;
buffer *b;

void *reader();
void *writer();

buffer *create_buffer(int buffer_size) {
    b = (buffer *)malloc(sizeof(buffer));
    b -> buffer_size = buffer_size;
    b -> count = 0;
    b -> out = 0;
    b -> in = 0;
    b -> items = (buffer_item *)calloc(buffer_size, sizeof(buffer_item));
    return b;
}

int read_item(buffer *b)
{
    if (b -> count == 0) {
        return -1;
    }

    return 0;
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
    int reader_count = atoi(argv[2]);
    int writer_count = atoi(argv[3]);

    /* initialize buffer */
    b = create_buffer(10);

    /* create semaphores */

    /* prototype of sem_init() :
    *  int	sem_init(sem_t *sem, int pshared, unsigned int value);
    *  if pshared is 0, semaphore cannot be shared between threads/processes. 
       else, if non-zero, can be shared between threads/processes
    */
    
    sem_init(&rw_mutex, 1, 1); 
    sem_init(&mutex, 1, 1);

    /* create reader threads */
    pthread_t *readers = malloc(sizeof(pthread_t) * reader_count);

    for (int i = 0; i < reader_count; i++)
    {
        pthread_create(&readers[i], NULL, reader, NULL);
    }


    /* create writer threads */
    pthread_t *writers = malloc(sizeof(pthread_t) * writer_count);

    for (int i = 0; i < writer_count; i++)
    {
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    sleep(sleep_time);
    exit(0);

}

void *reader() 
{
    buffer_item item;

    while (1) 
    {
        /* sleep for a random period of time */
        int sleep_time = rand() % 5;
        sleep(sleep_time);
       
        sem_wait(&mutex);
        read_count++;
        
        if (read_count == 1)
            sem_wait(&rw_mutex);
        sem_post(&mutex);

        /* critical section -> reading */
        if (read_item(b) == -1)
            printf("Buffer empty, nothing to read\n");
        else
            printf("A Reader is reading....\n");

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) 
            sem_post(&rw_mutex);
        sem_post(&mutex);
    }
}

void *writer() 
{
    buffer_item item;

    while (1) 
    {
        /* sleep for a random period of time */
        int sleep_time = rand() % 5;
        sleep(sleep_time);

        /* decide to delete from or add to buffer
        if add == 1, then add to buffer
        if add == 0, then delete from buffer
        */
        int add = random() % 2;

        if (add == 1) {
            /* generate a random number */
            item = rand() % 100;
        }
 
        sem_wait(&rw_mutex);
       
        /* critical section -> writing*/
        if (add == 1) {
            if (insert_item(b, item) == -1)
                printf("A writer is writing...Item was not inserted\n");
            else
                printf("A writer is writing.... %d added \n", item);
        }
        else {
            buffer_item item_consumed = remove_item(b);
            if (item_consumed == -1)
                printf("A writer is writing...Item was not removed\n");
            else
                printf("A writer is writing...%d removed \n", item_consumed); 
        }
        sem_post(&rw_mutex);
    }
}

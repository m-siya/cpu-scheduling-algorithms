#ifndef BUFFER
#define BUFFER

typedef int buffer_item;

typedef struct 
{
    int buffer_size;
    int count;
    int out;
    int in;
    int *items;
} buffer;

#endif
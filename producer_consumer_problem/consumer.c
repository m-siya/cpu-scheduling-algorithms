#include <stdio.h>
#include "buffer.c"

item next_consumed;

int main()
{
    while (true)
    {
        while (in == out)\
            ; /* do nothing */

        next_consumed = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        /*consume the item in next_consumed*/
        printf("Consuming item");
    }

    return 0;
}
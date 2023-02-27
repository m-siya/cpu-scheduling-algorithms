#include <stdio.h>
#include <stdlib.h>
#include "buffer.c"

int main() {
    printf("Producer checks buffer");

    item next_produced;

    while (true) {
        /* produce an item in next_produced */

        while (((int + 1) % BUFFER_SIZE) == out)
            ; /* do nothing */

        printf("Write to buffer");
        buffer[in] = next_produced;
        in = (in + 1) % BUFFER_SIZE;
    }

    return 0;
}

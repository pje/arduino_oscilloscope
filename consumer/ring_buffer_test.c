#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "ring_buffer.h"

#define EPSILON 0.00000000001

int main(int argc, char *argv[]) {
    RingBuffer *buffer = ring_buffer_init(4);

    ring_buffer_push(buffer, 0.1);
    ring_buffer_push(buffer, 0.2);
    ring_buffer_push(buffer, 0.3);
    ring_buffer_push(buffer, 0.4);
    ring_buffer_push(buffer, 0.5);
    ring_buffer_push(buffer, 0.6);

    assert(fabs(ring_buffer_get(buffer, 0) - 0.6) < EPSILON);
    assert(fabs(ring_buffer_get(buffer, 1) - 0.5) < EPSILON);
    assert(fabs(ring_buffer_get(buffer, 2) - 0.4) < EPSILON);
    assert(fabs(ring_buffer_get(buffer, 3) - 0.3) < EPSILON);

    ring_buffer_free(buffer);

    RingBuffer *buffer2 = ring_buffer_init(6);

    ring_buffer_push(buffer2, 0.1);
    ring_buffer_push(buffer2, 0.2);
    ring_buffer_push(buffer2, 0.3);
    ring_buffer_push(buffer2, 0.4);
    ring_buffer_push(buffer2, 0.5);
    ring_buffer_push(buffer2, 0.6);
    ring_buffer_push(buffer2, 0.7);
    ring_buffer_push(buffer2, 0.8);

    TYPE *output_buffer = calloc(4, sizeof(TYPE));
    ring_buffer_get_n(buffer2, 4, output_buffer);
    TYPE *expected_buffer = calloc(4, sizeof(TYPE));
    expected_buffer[0] = 0.8;
    expected_buffer[1] = 0.7;
    expected_buffer[2] = 0.6;
    expected_buffer[3] = 0.5;

    for (int i = 0; i < 4; i++) {
        assert(output_buffer[i] == expected_buffer[i]);
    }
    free(output_buffer);

    ring_buffer_push(buffer2, 1.0);
    ring_buffer_push(buffer2, 2.0);
    ring_buffer_push(buffer2, 3.0);
    ring_buffer_push(buffer2, 4.0);
    ring_buffer_push(buffer2, 5.0);

    output_buffer = calloc(6, sizeof(TYPE));

    int return_value = ring_buffer_get_n(buffer2, 7, output_buffer);
    assert(return_value == -1);

    ring_buffer_get_n(buffer2, 6, output_buffer);
    expected_buffer = calloc(6, sizeof(TYPE));
    expected_buffer[0] = 5.0;
    expected_buffer[1] = 4.0;
    expected_buffer[2] = 3.0;
    expected_buffer[3] = 2.0;
    expected_buffer[4] = 1.0;
    expected_buffer[5] = 0.8;

    for (int i = 0; i < 6; i++) {
        assert(output_buffer[i] == expected_buffer[i]);
    }
    free(output_buffer);

    printf("success!\n");

    //
    // valgrind test: should exhibit constant memory usage (no leaks)
    //
    // while(1) {
    //   size_t size = 1000;
    //   TYPE *output_buffer = malloc(size * sizeof(TYPE));
    //   RingBuffer *buffer = ring_buffer_init(size);
    //   for (int i = 0; i < size; i++) {
    //     ring_buffer_push(buffer, (i / (TYPE)size));
    //   }
    //   for (int i = 0; i < size; i++) {
    //     ring_buffer_get_n(buffer, i, output_buffer);
    //   }
    //   free(output_buffer);
    //   ring_buffer_free(buffer);
    // }
    return(0);
}

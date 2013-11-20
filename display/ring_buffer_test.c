#include <stdio.h>
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

  assert(fabs(ring_buffer_pop(buffer) - 0.6) < EPSILON);
  assert(fabs(ring_buffer_pop(buffer) - 0.5) < EPSILON);
  assert(fabs(ring_buffer_pop(buffer) - 0.4) < EPSILON);
  assert(fabs(ring_buffer_pop(buffer) - 0.3) < EPSILON);

  ring_buffer_inspect(ring_buffer);

  printf("success!\n");

  // valgrind test: should exhibit constant memory usage (no leaks)
  //
  // while(1) {
  //   size_t size = 1000;
  //   RingBuffer *buffer = ring_buffer_init(size);
  //   for (int i = 0; i < size; i++) {
  //     ring_buffer_push(buffer, (i / (double)size));
  //   }
  //   for (int i = 0; i < size; i++) {
  //     ring_buffer_pop(buffer);
  //   }
  //   ring_buffer_free(buffer);
  // }
  return(0);
}

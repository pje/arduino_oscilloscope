#include <stdio.h>
#include <math.h>
#include "ring_buffer.h"
#include <assert.h>

#define EPSILON 0.00000000001

int main(int argc, char *argv[]) {
  RingBuffer *buffer = ring_buffer_init(4);

  ring_buffer_push(buffer, 0.1);
  ring_buffer_push(buffer, 0.2);
  ring_buffer_push(buffer, 0.3);
  ring_buffer_push(buffer, 0.4);

  assert(fabs(ring_buffer_pop(buffer) - 0.4) < EPSILON);
  assert(fabs(ring_buffer_pop(buffer) - 0.3) < EPSILON);
  assert(fabs(ring_buffer_pop(buffer) - 0.2) < EPSILON);
  assert(fabs(ring_buffer_pop(buffer) - 0.1) < EPSILON);

  return(0);
}

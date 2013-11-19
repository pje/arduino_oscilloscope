#include <stdio.h>
#include <math.h>
#include "ring_buffer.h"
#include <assert.h>

#define EPSILON 0.00000001

int main(int argc, char *argv[]) {
  size_t ring_size = 10;
  RingBuffer *buffer = ring_buffer_init(ring_size);

  for (int i = 0; i < ring_size; i++) {
    double e = i / 100.0;
    ring_buffer_push(buffer, e);
    double head_element = ring_buffer_get(buffer, 0);
    assert(fabs(head_element - e) < EPSILON);
  }

  return(0);
}

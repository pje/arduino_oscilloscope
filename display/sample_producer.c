#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include "sample_producer.h"
#include "ring_buffer.h"

#define PI 3.14159265358979323846264338327950288419716939937510582097

void *sample_producer_start(void *arg) {
  unsigned long time = 0;
  while(1) {
    RingBuffer *buffer = (RingBuffer*) arg;
    _fill_ring_buffer_with_sin_samples(buffer, time++);
    nanosleep((struct timespec[]){{0, 50000000}}, NULL);
  }
  return NULL;
}

void _fill_ring_buffer_with_sin_samples(RingBuffer *buffer, unsigned long cycle_offset) {
  for (size_t i = 0; i < buffer->size; i++) {
    double rads = ((((i + cycle_offset) % 360) * PI) / 180.0);
    double sample = ((sin(rads) + 1.0) / 2.0);
    ring_buffer_push(buffer, sample);
  }
}

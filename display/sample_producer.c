#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include "sample_producer.h"
#include "ring_buffer.h"

#define TAU 6.283185307179586

void *sample_producer_start(void *arg) {
  unsigned long time = 0;

  while(1) {
    RingBuffer *buffer = (RingBuffer*) arg;

    for (size_t i = 0; i < buffer->size; i++) {
      double rads = ((((i + time++) % 360) * (TAU / 2.0)) / 180.0);
      double sample = ((sin(rads) + 1.0) / 2.0);
      ring_buffer_push(buffer, sample);
    }

    nanosleep((struct timespec[]){{0, 500000000}}, NULL);
  }

  return NULL;
}

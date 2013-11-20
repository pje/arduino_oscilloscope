#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "ring_buffer.h"

RingBuffer *ring_buffer_init(size_t size) {
  TYPE *elements = malloc(size * sizeof(TYPE));
  RingBuffer *rb = malloc(sizeof(RingBuffer));
  rb->elements = elements;
  rb->head_index = size;
  rb->size = size;
  return(rb);
}

void ring_buffer_free(RingBuffer *buffer) {
  free(buffer->elements);
  free(buffer);
}

TYPE ring_buffer_pop(RingBuffer *buffer) {
  TYPE element = buffer->elements[buffer->head_index];
  buffer->head_index = buffer->head_index == (buffer->size - 1) ? 0 : buffer->head_index + 1;
  return element;
}

TYPE ring_buffer_get(const RingBuffer *buffer, size_t index) {
  return buffer->elements[(buffer->head_index + index) % buffer->size];
}

// void ring_buffer_get(const RingBuffer *buffer, size_t index, size_t amount, TYPE* output_buffer) {}

void ring_buffer_push(RingBuffer *buffer, TYPE element) {
  buffer->head_index = buffer->head_index == 0 ? buffer->size - 1 : buffer->head_index - 1;
  buffer->elements[buffer->head_index % buffer->size] = element;
}

void ring_buffer_inspect(const RingBuffer *buffer) {
  printf("{ ");
  for (size_t i = 0; i < buffer->size; i++) {
    printf("%f, ", buffer->elements[i]);
  }
  printf(" }\n");
}

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "ring_buffer.h"
#include <stdio.h>

RingBuffer *ring_buffer_init(const size_t size) {
  TYPE *elements = malloc(size);
  RingBuffer *rb = malloc(sizeof(RingBuffer));
  rb->elements = elements;
  rb->head_index = 0;
  rb->size = size;
  return(rb);
}

void ring_buffer_free(RingBuffer * const buffer) {
  free(buffer->elements);
  free(buffer);
}

TYPE ring_buffer_get(RingBuffer * const buffer, const long index) {
  return((buffer->elements[(buffer->head_index - index) % buffer->size]));
}

void ring_buffer_push(RingBuffer * const buffer, const TYPE element) {
  ring_buffer_set(buffer, element, 0);
}

void ring_buffer_set(RingBuffer * const buffer, const TYPE element, const long index) {
  TYPE* element_copy = _copy(element);
  buffer->elements[((buffer->head_index + 1) % buffer->size)] = *element_copy;
  buffer->head_index = ((buffer->head_index + 1) % buffer->size);
}

TYPE *_copy(const TYPE e) {
  TYPE *element_copy = malloc(sizeof(TYPE));
  memcpy(element_copy, &e, sizeof(TYPE));
  return(element_copy);
}

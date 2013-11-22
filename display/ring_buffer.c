#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "ring_buffer.h"

RingBuffer *ring_buffer_init(size_t size) {
  TYPE *elements = calloc(size, sizeof(TYPE));
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

int ring_buffer_get_n(const RingBuffer *buffer, size_t amount_requested, TYPE* output_buffer) {
  // { 1, 8, 4, 3, 6, 2, 7, 5 }
  //               ^----head
  //
  // buffer->size: 8
  // buffer->head_index: 4
  // amount_requested: 7
  //
  // section_a =              { 6, 2, 7, 5 }
  // section_b = { 1, 8, 4, 3 }
  // return(section_a + section_b)
  //
  //
  // section_a_num_elements = size - head_index;
  // section_b_num_elements = (amount_requested > section_a_num_elements) ? head_index : 0;
  //
  // section_a = (head_index..size);
  // section_b = (0..head_index);

  size_t section_a_num_elements = buffer->size - buffer->head_index;
  TYPE *section_a_start_address = buffer->elements + buffer->head_index;

  memcpy(output_buffer, section_a_start_address, sizeof(TYPE) * section_a_num_elements);

  if(amount_requested > section_a_num_elements) {
    size_t section_b_num_elements = buffer->head_index;
    TYPE *section_b_start_address = buffer->elements;

    memcpy(output_buffer, section_a_start_address, section_a_num_elements);
    memcpy(output_buffer + section_a_num_elements, section_b_start_address, sizeof(TYPE) * section_b_num_elements);
  }
  return 0; // todo: return success/failure
}

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

void _buffer_inspect(TYPE *buffer, size_t buffer_size) {
  printf("{ ");
  for (int i = 0; i < buffer_size; i++) {
    printf("%f, ", buffer[i]);
  }
  printf(" }\n");
}

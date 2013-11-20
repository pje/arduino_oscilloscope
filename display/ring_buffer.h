#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#define TYPE double

typedef struct RingBuffer {
  TYPE *elements;
  size_t size;
  size_t head_index;
} RingBuffer;

RingBuffer *ring_buffer_init(size_t size);
void ring_buffer_free(RingBuffer *buffer);
TYPE ring_buffer_pop(RingBuffer *buffer);
TYPE ring_buffer_get(const RingBuffer *buffer, size_t index);
// void ring_buffer_get(const RingBuffer *buffer, size_t index, size_t amount, TYPE* output_buffer);
void ring_buffer_push(RingBuffer *buffer, TYPE element);
void ring_buffer_inspect(const RingBuffer *buffer);

#endif

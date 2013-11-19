#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#define TYPE double

typedef struct RingBuffer {
  TYPE *elements;
  size_t size;
  int head_index;
} RingBuffer;

RingBuffer *ring_buffer_init(const size_t size);
void ring_buffer_free(RingBuffer *buffer);
TYPE ring_buffer_get(const RingBuffer *buffer, const int index);
void ring_buffer_set(RingBuffer *buffer, const TYPE element, const long index);
void ring_buffer_push(RingBuffer *buffer, const TYPE element);
TYPE *_copy(TYPE e);

#endif

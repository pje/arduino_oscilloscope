#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#define TYPE double

typedef struct RingBuffer {
  TYPE *elements;
  size_t size;
  long head_index;
} RingBuffer;

RingBuffer *ring_buffer_init(const size_t size);
void ring_buffer_free(RingBuffer * const buffer);
TYPE ring_buffer_get(RingBuffer * const buffer, const long index);
void ring_buffer_set(RingBuffer * const buffer, const TYPE element, const long index);
void ring_buffer_push(RingBuffer * const buffer, const TYPE element);
TYPE *_copy(const TYPE e);

#endif

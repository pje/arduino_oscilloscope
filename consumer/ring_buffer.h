#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <pthread.h>

#define TYPE double

typedef struct RingBuffer {
    TYPE *elements;
    size_t size;
    size_t head_index;
    pthread_mutex_t *elements_lock;
} RingBuffer;

RingBuffer *ring_buffer_init(size_t size);
void ring_buffer_free(RingBuffer *buffer);
TYPE ring_buffer_get(const RingBuffer *buffer, size_t index);
int ring_buffer_get_n(const RingBuffer *buffer, size_t amount_requested, TYPE* output_buffer);
void ring_buffer_push(RingBuffer *buffer, TYPE element);
void ring_buffer_inspect(const RingBuffer *buffer);
void _buffer_inspect(TYPE *buffer, size_t buffer_size);

#endif

#include <pthread.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "ring_buffer.h"

RingBuffer *ring_buffer_init(size_t size) {
    RingBuffer *rb = (RingBuffer*) malloc(sizeof(RingBuffer));
    if (rb == NULL) {
        fprintf(stderr, "unable to malloc() RingBuffer");
        exit(EXIT_FAILURE);
    }
    rb->elements = (TYPE*)calloc(size, sizeof(TYPE));
    if (rb->elements == NULL) {
        fprintf(stderr, "unable to malloc() RingBuffer elements");
        exit(EXIT_FAILURE);
    }
    rb->head_index = size;
    rb->size = size;
    rb->elements_lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    if (rb->elements_lock == NULL) {
        fprintf(stderr, "unable to malloc() RingBuffer pthread_mutex_t");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_init(rb->elements_lock, NULL);
    return(rb);
}

void ring_buffer_free(RingBuffer *buffer) {
    free(buffer->elements);
    pthread_mutex_destroy(buffer->elements_lock);
    free(buffer);
}

TYPE ring_buffer_pop(RingBuffer *buffer) {
    TYPE element = buffer->elements[buffer->head_index];
    buffer->head_index = buffer->head_index == (buffer->size - 1) ? 0 : buffer->head_index + 1;
    return element;
}

TYPE ring_buffer_get(const RingBuffer *buffer, size_t index) {
    TYPE element = buffer->elements[(buffer->head_index + index) % buffer->size];
    return element;
}

int ring_buffer_get_n(const RingBuffer *buffer, size_t amount_requested, TYPE* output_buffer) {
    size_t section_a_num_elements = buffer->size - buffer->head_index;
    TYPE *section_a_start_address = (buffer->elements) + (buffer->head_index);
    memcpy(output_buffer, section_a_start_address, sizeof(TYPE) * section_a_num_elements);
    if(amount_requested > section_a_num_elements) {
        size_t section_b_num_elements = (amount_requested - section_a_num_elements);
        TYPE *section_b_start_address = buffer->elements;
        memcpy(output_buffer + section_a_num_elements, section_b_start_address, sizeof(TYPE) * section_b_num_elements);
    }
    return 0; // todo: return success/failure on memcpy failure
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
    printf("  head_index: %zu\n", buffer->head_index);
}

void _buffer_inspect(TYPE *buffer, size_t buffer_size) {
    printf("{ ");
    for (size_t i = 0; i < buffer_size; i++) {
        printf("%f, ", buffer[i]);
    }
    printf(" }\n");
}

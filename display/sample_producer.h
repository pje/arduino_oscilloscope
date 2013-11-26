#ifndef SAMPLE_PRODUCER_H
#define SAMPLE_PRODUCER_H

struct RingBuffer;

void *sample_producer_start(void *ring_buffer);
void sample_producer_print_bytes_little_endian(size_t num_bytes, const void *ptr);
void sample_producer_print_bytes_big_endian(size_t num_bytes, const void *ptr);
void sample_producer_error(char* msg);

#endif

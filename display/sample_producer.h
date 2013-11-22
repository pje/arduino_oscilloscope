#ifndef SAMPLEPRODUCER_H
#define SAMPLEPRODUCER_H

struct RingBuffer;

void *sample_producer_start(void *arg);
void sample_producer_print_bits(size_t size, const void *ptr);
void sample_producer_error(char* msg);

#endif

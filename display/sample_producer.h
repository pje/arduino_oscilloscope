#ifndef SAMPLE_PRODUCER_H
#define SAMPLE_PRODUCER_H

struct RingBuffer;

void *sample_producer_start(void *ring_buffer);
void sample_producer_error(char* msg);

#endif

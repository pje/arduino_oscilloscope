#ifndef SAMPLEPRODUCER_H
#define SAMPLEPRODUCER_H

struct RingBuffer;

void *sample_producer_start(void *arg);
void _fill_ring_buffer_with_sin_samples(struct RingBuffer *buffer, unsigned long cycle_offset);

#endif

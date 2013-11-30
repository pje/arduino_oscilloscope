#ifndef SIGNAL_SOURCE_H
#define SIGNAL_SOURCE_H

struct RingBuffer;

void *signal_source_start(void *ring_buffer);
void signal_source_error(char* msg, int fd);

#endif

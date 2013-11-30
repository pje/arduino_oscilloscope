#ifndef SIGNAL_SOURCE_H
#define SIGNAL_SOURCE_H

void *signal_source_start(void *ring_buffer);
_Noreturn void signal_source_error(char* msg, int fd);
void signal_source_print_bytes(size_t size, void const * const ptr);

#endif

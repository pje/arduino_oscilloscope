#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "sample_producer.h"
#include "ring_buffer.h"
#include "arduino_serial_port.h"

void *sample_producer_start(void *ring_buffer) {
  RingBuffer *buffer = (RingBuffer*) ring_buffer;
  int fd = -1;
  const char *port = "/dev/tty.usbserial-A600afNY";
  const int baudrate = 9600;
  // const int max_sample_value = 1024;
  const int max_sample_value = 255;
  const int sleep_micros = 1000;
  fd = arduino_serial_port_init(port, baudrate);
  if (fd == -1) sample_producer_error("couldn't open port");
  const size_t protocol_sample_frame_size = 1;
  unsigned char raw_samples[protocol_sample_frame_size];

  while(1) {
    memset(raw_samples, 0, sizeof(unsigned char) * protocol_sample_frame_size);
    int read_result = arduino_serial_port_read(fd, raw_samples, protocol_sample_frame_size, sleep_micros);
    if(read_result < 0) sample_producer_error("read() returned negative value");
    if(read_result != protocol_sample_frame_size) sample_producer_error("read() read too many bytes?");

    // int byte2 = (int) raw_samples[1];
    // int byte1 = (int) raw_samples[2];
    // int voltage = (byte2 << 8) | byte1;
    unsigned int voltage = (unsigned int)raw_samples[0];
    TYPE sample = (TYPE)(voltage / (TYPE)max_sample_value);
    pthread_mutex_lock(buffer->elements_lock);
    ring_buffer_push(buffer, sample);
    pthread_mutex_unlock(buffer->elements_lock);
  }
}

void sample_producer_error(char* msg) {
  fprintf(stderr, "%s\n",msg);
  exit(EXIT_FAILURE);
}

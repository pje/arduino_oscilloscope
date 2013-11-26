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
  const char *serialport = "/dev/tty.usbserial-A600afNY";
  const int baudrate = 9600;

  // bits_per_producer_loop = 16
  // samples_per_second     = baudrate / bits_per_producer_loop
  // 600                    = 9600     / 16
  //
  // seconds_in_backlog     = 10
  // samples_in_backlog     = seconds_in_backlog * samples_per_second
  // 6000                   = 10                 * 600

  const int max_sample_value = 1024;
  const int sleep_micros = 1000; // 1 millisecond
  fd = arduino_serial_port_init(port, baudrate);
  if (fd == -1) sample_producer_error("couldn't open port");
  const size_t num_raw_samples = 2;
  unsigned char raw_samples[2];
  while(1) {
    memset(raw_samples, 0, sizeof(unsigned char) * protocol_sample_frame_size);
    int read_result = arduino_serial_port_read(fd, raw_samples, protocol_sample_frame_size, sleep_micros);
    if(read_result < 0) sample_producer_error("read() returned negative value");
    for (size_t i = 0; i < num_raw_samples; i++){
      if (i % 2 == 1) {
        int byte1 = (int) raw_samples[i - 1];
        int byte2 = (int) raw_samples[i];
        int voltage = (byte1 << 8) | byte2;
        TYPE sample = (TYPE)(voltage / (TYPE)max_sample_value);
        pthread_mutex_lock(buffer->elements_lock);
        ring_buffer_push(buffer, sample);
        pthread_mutex_unlock(buffer->elements_lock);
      }
    }
  }
}

void sample_producer_error(char* msg) {
  fprintf(stderr, "%s\n",msg);
  exit(EXIT_FAILURE);
}

void sample_producer_print_bytes_little_endian(size_t num_bytes, const void *ptr) {
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;
  for (i = 0; i <= num_bytes-1; i++) {
    for (j = 7; j >= 0; j--) {
      byte = b[i] & (1 << j);
      byte >>= j;
      printf("%u", byte);
    }
    printf(" ");
  }
  printf("\n");
}

void sample_producer_print_bytes_big_endian(size_t num_bytes, const void *ptr) {
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;
  for (i = num_bytes-1; i >= 0; i--) {
    for (j = 7; j >= 0; j--) {
      byte = b[i] & (1 << j);
      byte >>= j;
      printf("%u", byte);
    }
    printf(" ");
  }
  printf("\n");
}

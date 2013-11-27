#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "sample_producer.h"
#include "ring_buffer.h"
#include "arduino_serial_port.h"

//assumes little endian
void sample_producer_print_bytes(size_t size, void const * const ptr) {
  unsigned char *b = (unsigned char*) ptr;

  for (int i = size - 1; i >= 0; i--) {
    for (int j = 7; j >= 0; j--) {
      unsigned char byte = b[i] & (1 << j);
      byte >>= j;
      printf("%u", byte);
    }
    printf(" ");
  }
  printf("\n");
}

void *sample_producer_start(void *arg) {
  RingBuffer *buffer = (RingBuffer*)arg;

  int fd = -1;
  const char *port = "/dev/tty.usbserial-A600afNY";
  const int baudrate = 9600;
  const int max_sample_value = 1024;
  const int sleep_micros = 1000;
  fd = arduino_serial_port_init(port, baudrate);
  if (fd == -1) sample_producer_error("couldn't open port");
  const size_t protocol_sample_frame_size = 3;
  unsigned char raw_samples[protocol_sample_frame_size];

  while(1) {
    int read_result;
    unsigned char b[1];
    int broken_attempts = 4;
    while(b[0] != 0b11111111) { // just discard the first frame
      read_result = arduino_serial_port_read(fd, b, 1, sleep_micros);
      if (--broken_attempts <= 0) { sample_producer_error("couldn't align bits!"); }
    }
    memset(raw_samples, 0, sizeof(unsigned char) * protocol_sample_frame_size);
    read_result = arduino_serial_port_read(fd, raw_samples, protocol_sample_frame_size, sleep_micros);
    if(read_result < 0) sample_producer_error("read() returned negative value");
    if(read_result != protocol_sample_frame_size) sample_producer_error("read() read too many bytes?");

    unsigned short int sample_byte_1 = (unsigned short int) raw_samples[1];
    unsigned short int sample_byte_0 = (unsigned short int) raw_samples[0];
    unsigned short int sample_voltage = ((unsigned short int)(sample_byte_1 << 5)) | sample_byte_0;

    TYPE sample = (TYPE)(sample_voltage / (TYPE)max_sample_value);
    pthread_mutex_lock(buffer->elements_lock);
    ring_buffer_push(buffer, sample);
    pthread_mutex_unlock(buffer->elements_lock);
  }
}

void sample_producer_error(char* msg) {
  fprintf(stderr, "%s\n",msg);
  exit(EXIT_FAILURE);
}

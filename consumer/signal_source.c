#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "signal_source.h"
#include "ring_buffer.h"
#include "arduino_serial_port.h"

void signal_source_print_bytes(size_t size, void const * const ptr) {
  unsigned char *b = (unsigned char*) ptr;
  for (size_t i = size; i > 0; i--) {
    for (size_t j = 8; j > 0; j--) {
      unsigned char byte = b[i - 1] & (1 << (j - 1));
      byte >>= (j - 1);
      printf("%u", byte);
    }
    printf(" ");
  }
  printf("\n");
}

void *signal_source_start(void *arg) {
  RingBuffer *buffer = (RingBuffer*)arg;
  const size_t mutex_attempts = 100;
  int fd = -1;
  const char *port = "/dev/tty.usbserial-A600afNY";
  const int baudrate = 28800;
  const int max_sample_value = 1024;
  const int sleep_micros = 1000;
  fd = arduino_serial_port_init(port, baudrate);
  if (fd == -1) signal_source_error("couldn't open port", fd);
  const size_t protocol_sample_frame_size = 3;
  unsigned char *raw_samples = malloc(sizeof(unsigned char) * protocol_sample_frame_size);
  ssize_t read_result;
  unsigned char b[1];
  int alignment_attempts = 4;
  while(b[0] != 0xff) { // just discard the first frame
    read_result = arduino_serial_port_read(fd, b, 1, sleep_micros);
    if (--alignment_attempts <= 0) { signal_source_error("couldn't align bits!", fd); }
  }

  while(1) {
    alignment_attempts = 4;
    memset(raw_samples, 0, sizeof(unsigned char) * protocol_sample_frame_size);
    for (size_t i = 0; i < protocol_sample_frame_size; i++) {
      read_result = arduino_serial_port_read(fd, raw_samples + i, 1, sleep_micros);
      if(read_result < 0) signal_source_error("read() returned negative value", fd);
      if(read_result != 1) signal_source_error("read() read too many bytes?", fd);
    }

    while(raw_samples[2] != 0xff) {
      raw_samples[0] = raw_samples[1];
      raw_samples[1] = raw_samples[2];
      read_result = arduino_serial_port_read(fd, raw_samples + 2, 1, sleep_micros);
      if (--alignment_attempts <= 0) { signal_source_error("couldn't align bits!", fd); }
    }

    unsigned short int sample_byte_1 = (unsigned short int) raw_samples[0];
    unsigned short int sample_byte_0 = (unsigned short int) raw_samples[1];
    unsigned short int sample_voltage = ((unsigned short int)(sample_byte_1 << 5)) | sample_byte_0;

    TYPE sample = (TYPE)(sample_voltage / (TYPE)max_sample_value);

    for (size_t tries = 0; tries <= mutex_attempts; tries++) {
      int result = pthread_mutex_lock(buffer->elements_lock);
      if (result == 0) { break; }
      else { printf("error code: %d\n", result); }
      if (tries >= mutex_attempts) { signal_source_error("unable to obtain lock!", fd); }
    }
    ring_buffer_push(buffer, sample);
    pthread_mutex_unlock(buffer->elements_lock);
  }
}

_Noreturn void signal_source_error(char* msg, int fd) {
  fprintf(stderr, "%s\n",msg);
  if (fd > 0) { arduino_serial_port_close(fd); }
  exit(EXIT_FAILURE);
}

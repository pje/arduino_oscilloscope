#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "sample_producer.h"
#include "ring_buffer.h"
#include "arduino-serial-lib.h"

#define PI 3.14159265358979323846264338327950288419716939937510582097

void *sample_producer_start(void *arg) {
  RingBuffer *buffer = (RingBuffer*) arg;

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

  const samples_per_second = 1200;
  const int max_sample_value = 1024;
  size_t to_produce = 1024;

  fd = serialport_init(serialport, baudrate);
  if (fd == -1) sample_producer_error("couldn't open port");

  while(1) {
    unsigned char buf[to_produce];
    memset(buf, 0, buffer->size);
    serialport_read(fd, buf, buffer->size);
    for (size_t i = 0; i < buffer->size; i++){
      if (i % 2 == 1) {
        int byte1 = (int) buf[i - 1];
        int byte2 = (int) buf[i];
        int voltage = (byte1 << 8) | byte2;
        double sample = (double)(voltage / (double)max_sample_value);
        ring_buffer_push(buffer, sample);
      }
    }
  }
}

void sample_producer_error(char* msg) {
  fprintf(stderr, "%s\n",msg);
  exit(EXIT_FAILURE);
}

void sample_producer_print_bits(size_t size, const void *ptr) {
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;
  for (i = size-1; i >= 0; i--) {
    for (j = 7; j >= 0; j--) {
      byte = b[i] & (1 << j);
      byte >>= j;
      printf("%u", byte);
    }
  }
  printf("\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arduino-serial-lib.h"

void error(char* msg) {
  fprintf(stderr, "%s\n",msg);
  exit(EXIT_FAILURE);
}

void printBits(size_t size, const void *ptr) {
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

int main(int argc, char *argv[]) {
  const size_t buf_max = 1024;
  int fd = -1;
  const char *serialport = "/dev/tty.usbserial-A600afNY";
  const int baudrate = 9600; // max is 28800 for Duemilanove?
  const int timeout = 5000;

  fd = serialport_init(serialport, baudrate);
  if (fd == -1) error("couldn't open port");
  serialport_flush(fd);

  while(1) {
    unsigned char buf[buf_max];
    memset(buf, 0, buf_max);
    serialport_read(fd, buf, buf_max, timeout);
  }
  exit(EXIT_SUCCESS);
}

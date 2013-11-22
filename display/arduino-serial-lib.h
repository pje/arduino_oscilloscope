#ifndef __ARDUINO_SERIAL_LIB_H__
#define __ARDUINO_SERIAL_LIB_H__

#include <stdint.h>

int serialport_init(const char *serialport, int baud);
int serialport_close(int fd);
int serialport_flush(int fd);
int serialport_read(int fd, unsigned char *buf, size_t buf_max);

#endif

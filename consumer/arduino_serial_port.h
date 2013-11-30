#ifndef ARDUINO_SERIAL_PORT_H
#define ARDUINO_SERIAL_PORT_H

#include <stdint.h>

int arduino_serial_port_init(const char* serial_port, size_t baud_rate);
int arduino_serial_port_close(int fd);
int arduino_serial_port_flush(int fd);
ssize_t arduino_serial_port_read(int fd, unsigned char* buf, size_t buf_max, unsigned int sleep_micros);

#endif

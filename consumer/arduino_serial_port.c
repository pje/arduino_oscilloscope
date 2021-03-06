#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include "arduino_serial_port.h"

int arduino_serial_port_init(const char* serial_port, size_t baud_rate) {
    int fd = -1;
    struct termios toptions;
    fd = open(serial_port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)  {
        perror("arduino_serial_port_init: Unable to open port");
        perror(serial_port);
        return -1;
    }
    if (tcgetattr(fd, &toptions) < 0) {
        perror("arduino_serial_port_init: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud_rate;
    switch(baud_rate) {
    case 4800:
        brate=B4800;
        break;
    case 9600:
        brate=B9600;
        break;
    case 14400:
        brate=B14400;
        break;
    case 19200:
        brate=B19200;
        break;
    case 28800:
        brate=B28800;
        break;
    case 38400:
        brate=B38400;
        break;
    case 57600:
        brate=B57600;
        break;
    case 115200:
        brate=B115200;
        break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    toptions.c_cflag &= ~CRTSCTS;
    toptions.c_cflag |= CREAD | CLOCAL;
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    toptions.c_oflag &= ~OPOST;
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSANOW, &toptions);
    if (tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
        perror("arduino_serial_port_init: Couldn't set term attributes");
        return -1;
    }
    return fd;
}

int arduino_serial_port_close(int fd) {
    return close(fd);
}

ssize_t arduino_serial_port_read(int fd, unsigned char* buf, size_t buf_max, unsigned int sleep_micros) {
    unsigned char b[1];
    size_t i = 0;
    ssize_t bytes_read = 0;
    while (i < buf_max) {
        ssize_t n = read(fd, b, 1);
        switch(n) {
        case -1: {
            return(-1);
        }
        case 0: {
            usleep(sleep_micros);
            continue;
        }
        default: {
            break;
        }
        }
        bytes_read += n;
        buf[i] = b[0];
        i++;
    }
    return bytes_read;
}

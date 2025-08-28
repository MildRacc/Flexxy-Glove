#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>

int open_serial_port(const char* port_name) {
    int fd = open(port_name, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("Error opening serial port");
        return -1;
    }
    return fd;
}

void configure_serial_port(int fd) {
    struct termios options;
    tcgetattr(fd, &options);

    // Set baud rate (adjust as needed)
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    // Set other options (adjust as needed)
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CRTSCTS;
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 10;

    tcsetattr(fd, TCSANOW, &options);
}

int read_from_serial_port(int fd, char* buffer, int buffer_size) {
    int bytesRead = read(fd, buffer, buffer_size);
    if (bytesRead < 0) {
        perror("Error reading from serial port");
    }
    return bytesRead;
}

void close_serial_port(int fd) {
    close(fd);
}
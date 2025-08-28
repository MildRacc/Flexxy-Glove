#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

// Function to open the serial port
int open_serial_port(const char* port_name);

// Function to configure the serial port settings
void configure_serial_port(int fd);

// Function to read data from the serial port
int read_from_serial_port(int fd, char* buffer, int buffer_size);

// Function to close the serial port
void close_serial_port(int fd);

#endif // SERIAL_PORT_H
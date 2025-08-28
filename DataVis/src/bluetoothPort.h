#ifndef BLUETOOTH_PORT_H
#define BLUETOOTH_PORT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <string>

// Function to connect to a Bluetooth device
int connect_bluetooth(const char* bt_address);

// Function to read data from the Bluetooth connection
int read_from_bluetooth(int sock, char* buffer, int buffer_size);

// Function to close the Bluetooth connection
void close_bluetooth(int sock);

#endif // BLUETOOTH_PORT_H

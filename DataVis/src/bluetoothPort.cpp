#include "bluetoothPort.h"
#include <iostream>
#include <cstring>
#include <errno.h>

int connect_bluetooth(const char* bt_address) {
    struct sockaddr_rc addr = { 0 };
    int sock;
    
    // Allocate a socket
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (sock < 0) {
        perror("Error creating Bluetooth socket");
        return -1;
    }
    
    // Set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1; // RFCOMM channel 1
    str2ba(bt_address, &addr.rc_bdaddr);
    
    // Connect to server
    int status = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (status < 0) {
        perror("Error connecting to Bluetooth device");
        close(sock);
        return -1;
    }
    
    std::cout << "Connected to Bluetooth device: " << bt_address << std::endl;
    return sock;
}

int read_from_bluetooth(int sock, char* buffer, int buffer_size) {
    int bytes_read = read(sock, buffer, buffer_size - 1);
    if (bytes_read < 0) {
        perror("Error reading from Bluetooth socket");
    }
    return bytes_read;
}

void close_bluetooth(int sock) {
    close(sock);
    std::cout << "Bluetooth connection closed" << std::endl;
}

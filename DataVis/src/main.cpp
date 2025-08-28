#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "server.h"
#include "serialPort.h"
#include "bluetoothPort.h"
#include "websocket.h"
#include <cstring>

#define BUFFER_SIZE 256

void bluetooth_task(const char* bt_address) {
    char buffer[BUFFER_SIZE];

    int sock = connect_bluetooth(bt_address);
    if (sock < 0) {
        std::cerr << "Failed to connect to Bluetooth device: " << bt_address << std::endl;
        return;
    }

    // Wait for WebSocket client connection
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return client_connected; });
    }

    while (true) {
        int bytesRead = read_from_bluetooth(sock, buffer, BUFFER_SIZE);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "Received from Bluetooth: " << buffer << std::endl;
            send_message(buffer); // Send the data over WebSocket
            std::cout << "Sent over WebSocket: " << buffer << std::endl;
        } else if (bytesRead == 0) {
            std::cout << "No data read from Bluetooth" << std::endl;
        } else if (bytesRead < 0) {
            std::cerr << "Error reading from Bluetooth" << std::endl;
            break;
        }
    }

    close_bluetooth(sock);
}

void serial_task(const char* port) {
    char buffer[BUFFER_SIZE];

    int fd = open_serial_port(port);
    if (fd < 0) {
        std::cerr << "Failed to open serial port: " << port << std::endl;
        return;
    }

    configure_serial_port(fd);
    std::cout << "Serial port configured: " << port << std::endl;

    // Wait for WebSocket client connection
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return client_connected; });
    }

    while (true) {
        int bytesRead = read_from_serial_port(fd, buffer, BUFFER_SIZE);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "Received from serial: " << buffer << std::endl;
            send_message(buffer); // Send the data over WebSocket
            std::cout << "Sent over WebSocket: " << buffer << std::endl;
        } else if (bytesRead == 0) {
            std::cout << "No data read from serial port" << std::endl;
        } else if (bytesRead < 0) {
            std::cerr << "Error reading from serial port" << std::endl;
        }
    }

    close_serial_port(fd);
}

void server_task() {
    std::cout << "Starting WebSocket server..." << std::endl;
    socketMain();
    std::cout << "WebSocket server started." << std::endl;
}

void http_server_task() {
    std::cout << "Starting HTTP server..." << std::endl;
    start_server();
    std::cout << "HTTP server started." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <connection_type> <address>" << std::endl;
        std::cerr << "Connection types: serial, bluetooth" << std::endl;
        std::cerr << "Examples:" << std::endl;
        std::cerr << "  " << argv[0] << " serial /dev/ttyUSB0" << std::endl;
        std::cerr << "  " << argv[0] << " bluetooth 00:11:22:33:44:55" << std::endl;
        return 1;
    }

    const char* connection_type = argv[1];
    const char* address = argv[2];

    std::thread server_thread(server_task);
    std::thread http_thread(http_server_task);
    
    std::thread connection_thread;
    
    if (strcmp(connection_type, "serial") == 0) {
        connection_thread = std::thread(serial_task, address);
    } else if (strcmp(connection_type, "bluetooth") == 0) {
        connection_thread = std::thread(bluetooth_task, address);
    } else {
        std::cerr << "Invalid connection type: " << connection_type << std::endl;
        std::cerr << "Supported types: serial, bluetooth" << std::endl;
        return 1;
    }

    connection_thread.join();
    server_thread.join();
    http_thread.join();

    return 0;
}
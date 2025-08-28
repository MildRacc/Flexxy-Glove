// server.h
#ifndef SERVER_H
#define SERVER_H

#include </home/sashad/Arduino/HandController/DataVis/libs/httplib.h>
#include <string>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <set>
#include <mutex>

// Function declaration to initialize and run the server
void start_server();


#endif // SERVER_H

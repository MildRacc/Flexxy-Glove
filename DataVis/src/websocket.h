#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <mutex>
#include <condition_variable>
#include <string>

extern std::mutex mtx;
extern std::condition_variable cv;
extern bool client_connected;

int socketMain();
void send_message(const char* msg);
std::string process_finger_data(const char* msg);

#endif // WEBSOCKET_H
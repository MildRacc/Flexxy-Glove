#include "websocket.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <set>
#include <string>
#include <regex>
#include <sstream>

// Define a WebSocket server type using WebSocket++
typedef websocketpp::server<websocketpp::config::asio> server;

// A set to manage connected clients
std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> clients;

// Create a server endpoint as a global variable
server ws_server;

// Condition variable and mutex to synchronize connection
std::mutex mtx;
std::condition_variable cv;
bool client_connected = false;

// Buffer for incomplete messages
std::string message_buffer;

// Handle new connections
void on_open(server* s, websocketpp::connection_hdl hdl) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        clients.insert(hdl);
        client_connected = true;
    }
    cv.notify_all();
    std::cout << "Client connected" << std::endl;
}

// Handle closed connections
void on_close(server* s, websocketpp::connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(mtx);
    clients.erase(hdl);
    if (clients.empty()) {
        client_connected = false;
    }
    std::cout << "Client disconnected" << std::endl;
}

// Handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::cout << "Received: " << msg->get_payload() << std::endl;
}

// Process finger data to reverse the values
std::string process_finger_data(const char* msg) {
    std::string input(msg);
    std::string result;
    
    // Check if this is a complete message (should end with newline or contain 'G' followed by text)
    std::regex complete_pattern("a\\d+b\\d+c\\d+d\\d+e\\d+G[a-zA-Z ]+");
    
    if (!std::regex_search(input, complete_pattern)) {
        // Incomplete message, skip processing
        std::cerr << "Warning: Incomplete message received: " << input << std::endl;
        return "";
    }
    
    // Parse the message format: a1234b5678c9012d3456e7890GGesture
    std::regex pattern("([a-eG])(\\d+|[a-zA-Z ]+)");
    std::sregex_iterator begin(input.begin(), input.end(), pattern);
    std::sregex_iterator end;
    
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        char label = match[1].str()[0];
        std::string value = match[2].str();
        
        if (label >= 'a' && label <= 'e') {
            // Reverse finger values (4095 - value), but only if value is numeric
            try {
                int finger_value = std::stoi(value);
                int reversed_value = 4095 - finger_value;
                result += label + std::to_string(reversed_value);
            } catch (const std::invalid_argument& e) {
                // Skip invalid numeric values
                std::cerr << "Warning: Invalid numeric value '" << value << "' for finger " << label << std::endl;
                continue;
            }
        } else if (label == 'G') {
            // Keep gesture unchanged
            result += label + value;
        }
    }
    
    return result;
}

void send_message(const char* msg) {
    std::lock_guard<std::mutex> lock(mtx);
    
    // Add incoming data to buffer
    message_buffer += msg;
    
    // Process complete messages (messages ending with newline)
    std::string::size_type pos = 0;
    while ((pos = message_buffer.find('\n')) != std::string::npos) {
        std::string complete_message = message_buffer.substr(0, pos);
        message_buffer.erase(0, pos + 1);
        
        // Process the complete message
        std::string processed_msg = process_finger_data(complete_message.c_str());
        
        // Only send if we got a valid processed message
        if (!processed_msg.empty()) {
            for (const auto& hdl : clients) {
                try {
                    ws_server.send(hdl, processed_msg.c_str(), websocketpp::frame::opcode::text);
                } catch (const websocketpp::exception& e) {
                    std::cerr << "Broadcast failed: " << e.what() << std::endl;
                }
            }
        }
    }
    
    // Clean up buffer if it gets too long (prevent memory leaks from malformed data)
    if (message_buffer.length() > 1000) {
        message_buffer.clear();
        std::cerr << "Warning: Message buffer cleared due to excessive length" << std::endl;
    }
}

int socketMain() {
    try {
        // Set logging policies
        ws_server.set_access_channels(websocketpp::log::alevel::all);
        ws_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize the Asio transport policy
        ws_server.init_asio();

        // Bind handlers
        ws_server.set_open_handler(std::bind(&on_open, &ws_server, std::placeholders::_1));
        ws_server.set_close_handler(std::bind(&on_close, &ws_server, std::placeholders::_1));
        ws_server.set_message_handler(std::bind(&on_message, &ws_server, std::placeholders::_1, std::placeholders::_2));

        // Listen on port 9002
        ws_server.listen(9002);

        // Start the server accept loop
        ws_server.start_accept();

        // Start the ASIO io_service run loop
        ws_server.run();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (websocketpp::lib::error_code e) {
        std::cerr << "WebSocket++ Error: " << e.message() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
    }

    return 0;
}
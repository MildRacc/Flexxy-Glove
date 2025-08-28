cmake_minimum_required(VERSION 3.16)

project(MyWebServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(my_web_server src/main.cpp src/server.cpp src/serialPort.cpp src/bluetoothPort.cpp src/websocket.cpp)

# Include the path to httplib.h
include_directories(${CMAKE_SOURCE_DIR})

# Link Bluetooth library
target_link_libraries(my_web_server bluetooth)


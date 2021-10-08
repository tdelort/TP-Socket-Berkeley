cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 17)

project(USocket)

add_library(USocket STATIC
        src/USocket.cpp
        src/ConnectionTCP.cpp
        src/ConnectionUDP.cpp
        src/Connection.cpp
        src/Terminal.cpp
)

target_include_directories(USocket PUBLIC include)
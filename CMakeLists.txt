cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 17)

project(Socket)

add_executable(Server server.cpp)
add_executable(Client client.cpp)
include_directories(include)
add_subdirectory(src)

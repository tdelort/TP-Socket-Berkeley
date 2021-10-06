cmake_minimum_required(VERSION 3.13)

project(Socket)

add_executable(Server server.cpp )
add_executable(Client client.cpp)
include_directories(include)
add_subdirectory(src)

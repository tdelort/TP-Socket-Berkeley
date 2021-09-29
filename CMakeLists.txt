cmake_minimum_required(VERSION 3.10)

project(Socket)

add_executable(Server server.cpp "Connection.h" "ConnectionTCP.h" "ConnectionTCP.cpp")
add_executable(Client client.cpp "Connection.h" "ConnectionTCP.h" "ConnectionTCP.cpp")

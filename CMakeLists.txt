cmake_minimum_required(VERSION 3.10)

project(Socket)

add_executable(Server server.cpp "Connection.h" "ConnectionTCP.h" "ConnectionTCP.cpp" "USocket.cpp" "USocket.h" "Terminal.cpp" "Terminal.h" "Connection.cpp")
add_executable(Client client.cpp "Connection.h" "ConnectionTCP.h" "ConnectionTCP.cpp" "Connection.cpp" "USocket.cpp" "USocket.h" "Terminal.cpp" "Terminal.h")

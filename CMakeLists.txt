cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 17)

project(SocketChat)

add_executable(Server server.cpp)
add_executable(Client client.cpp)

add_subdirectory(USocket)

target_link_libraries(Server USocket)
target_link_libraries(Client USocket)

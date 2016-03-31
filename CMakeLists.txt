cmake_minimum_required(VERSION 3.4)

project(epoll_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES epoll_server.cpp)
add_executable(epoll_server ${SOURCE_FILES})

set(SOURCE_FILES block_io_server.cpp)
add_executable(block_io_server ${SOURCE_FILES})

add_executable(master_woker_epoll master_woker_epoll.cpp)
add_executable(block_io_client block_io_client.cpp)
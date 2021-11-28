cmake_minimum_required(VERSION 3.20)
project(server_plus C)

set(CMAKE_C_STANDARD 11)

add_executable(server_plus main.c BufferQueue.c BufferQueue.h Communication.h Utils/Hash.c Utils/Hash.h Utils/Array.c Utils/Array.h Clients.c Clients.h Groups.c Groups.h fileServer.h)

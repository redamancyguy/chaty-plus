cmake_minimum_required(VERSION 3.20)
project(server_plus C)

set(CMAKE_C_STANDARD 11)

add_executable(server_plus main.c BufferQueue.c BufferQueue.h Communication.h fileServer.h
        Utils/Tree.h Utils/Tree.c Utils/Array.h Client.h)

cmake_minimum_required(VERSION 3.5)
project(lzg C)

set(CMAKE_C_COMPILER /usr/local/Cellar/gcc/7.3.0_1/bin/gcc-7)

set(CMAKE_C_STANDARD 99)

add_executable(lzg main.c childProcess.c fatherProcess.c messageQueue.c shareDataBase.c header.h)

target_link_libraries(lzg pthread rt)
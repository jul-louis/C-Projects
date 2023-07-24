cmake_minimum_required(VERSION 3.20)
project(CMPUT379A3_Beta C)

set(CMAKE_C_STANDARD 99)

add_executable(CMPUT379A3_SERVER server.c server.h support.h tands.c)
add_executable(CMPUT379A3_CLIENT client.c client.h support.h tands.c)


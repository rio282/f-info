cmake_minimum_required(VERSION 4.1)
project(f_info C)

set(CMAKE_C_STANDARD 11)

add_executable(finfo main.c
        commands.c
        commands.h
        file_utils.h)

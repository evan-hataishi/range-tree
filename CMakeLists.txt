cmake_minimum_required(VERSION 3.13)
project(range_tree C)

set(CMAKE_C_STANDARD 11)

add_executable(range_tree main.c range_tree.c)

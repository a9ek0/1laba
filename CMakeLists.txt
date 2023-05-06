cmake_minimum_required(VERSION 3.24)
project(1laba)

set(CMAKE_C_STANDARD 99)

add_executable(1laba 1laba/main.c 1laba/lyb/check.c 1laba/lyb/memory.c 1laba/lyb/menu.c 1laba/lyb/parce.c 1laba/lyb/sort.c 1laba/lyb/structure.c ) 

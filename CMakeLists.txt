cmake_minimum_required(VERSION 3.24)
project(1laba)

set(CMAKE_C_STANDARD 99)

add_executable(1laba 1laba/main.c 1laba/mylib/check.c 1laba/mylib/memory.c 1laba/mylib/menu.c 1laba/mylib/parce.c 1laba/mylib/sort.c 1laba/mylib/structure.c ) 

cmake_minimum_required(VERSION 4.2)
project(GameOf31 C)

set(CMAKE_C_STANDARD 11)

add_executable(GameOf31 main.c
        card_utils.h)
if(WIN32)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
endif()

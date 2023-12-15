cmake_minimum_required(VERSION 3.10)

project(Exercise2)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ Standard")

FetchContent_Declare(systemc
    GIT_REPOSITORY https://github.com/accellera-official/systemc.git
    GIT_TAG 2.3.4
)

FetchContent_MakeAvailable(systemc)

add_executable(nand
    nand_main.cpp
)

target_link_libraries(nand
    SystemC::systemc
)

add_executable(xor
    exor_main.cpp
)

target_link_libraries(xor
    SystemC::systemc
)

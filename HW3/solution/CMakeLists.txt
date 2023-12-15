cmake_minimum_required(VERSION 3.10)

project(Exercise2)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ Standard")

FetchContent_Declare(systemc
    GIT_REPOSITORY https://github.com/accellera-official/systemc.git
    GIT_TAG 2.3.4
)

FetchContent_MakeAvailable(systemc)

add_executable(genomeParser
    main.cpp
    state_machine.cpp
    stimuli.cpp
)

target_link_libraries(genomeParser
    SystemC::systemc
)

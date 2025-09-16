# Uncomment this for nand exercise:
add_executable(nand
    nand_main.cpp
)

target_link_libraries(nand
    SystemC::systemc
)

# Uncomment this for xor exercise:
add_executable(xor
   exor_main.cpp
)

target_link_libraries(xor
   SystemC::systemc
)

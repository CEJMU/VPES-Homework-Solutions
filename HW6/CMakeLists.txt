add_executable(exercise6
    main.cpp
    processor.cpp
)

configure_file(stimuli1.txt stimuli1.txt COPYONLY)
configure_file(stimuli2.txt stimuli2.txt COPYONLY)

target_link_libraries(exercise6
    SystemC::systemc
)

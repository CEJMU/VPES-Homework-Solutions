#include <iostream>

#include "state_machine.h"

stateMachine::stateMachine(const sc_module_name &name)
    : sc_module(name), input("input")
{
    SC_METHOD(process);
    sensitive << clk.pos();
    dont_initialize();
    state.write(Start);
    counter = 0;
}

void stateMachine::process()
{
    switch (state)
    {
    case Start:
       if(input.read() == 'G')
            state.write(G);
       else
           state.write(Start);
       break;
    case G:
       if(input.read() == 'A')
            state.write(GA);
       else if(input.read() == 'G')
            state.write(G);
       else
           state.write(Start);
       break;
    case GA:
       if(input.read() == 'A')
            state.write(GAA);
       else if(input.read() == 'G')
            state.write(G);
       else
           state.write(Start);
       break;
    case GAA:
       if(input.read() == 'G')
            state.write(GAAG);
       else
           state.write(Start);
       break;
    case GAAG:
       std::cout << "Found: " << counter << std::endl;
       if(input.read() == 'G')
            state.write(G);
       else
           state.write(Start);
       break;
    }

    counter++;
}

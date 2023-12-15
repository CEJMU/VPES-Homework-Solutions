#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <systemc.h>

SC_MODULE(stateMachine)
{
public:
    sc_in<char> input;
    sc_in<bool> clk;

    SC_HAS_PROCESS(stateMachine);
    stateMachine(const sc_module_name &name);

private:
    void process();
    enum base {Start, G, GA, GAA, GAAG};
    sc_signal<base> state;

    unsigned int counter;
};

#endif // STATE_MACHINE_H



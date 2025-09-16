#pragma once

#include <systemc>

SC_MODULE(nand) // declare nand sc_module
{
public:
    sc_core::sc_in<bool> A, B; // input signal ports
    sc_core::sc_out<bool> Z;  // output signal ports

    SC_CTOR(nand) : A("A"), B("B"), Z("Z") // constructor for nand
    {
        SC_METHOD(do_nand);   // register do_nand with kernel
        sensitive << A << B;  // sensitivity list
    }

private:
    void do_nand() // a C++ function
    {
        Z.write(!(A.read() && B.read()));
    }
};

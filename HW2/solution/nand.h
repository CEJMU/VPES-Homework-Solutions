#ifndef NAND_H
#define NAND_H

#include <systemc.h>

SC_MODULE(nand) // declare nand sc_module
{
public:
    sc_in<bool> A, B; // input signal ports
    sc_out<bool> Z;  // output signal ports

    SC_CTOR(nand) : A("A"), B("B"), Z("Z") // constructor for nand2
    {
        SC_METHOD(do_nand);   // register do_nand2 with kernel
        sensitive << A << B;  // sensitivity list
    }

private:
    void do_nand() // a C++ function
    {
        Z.write(!(A.read() && B.read()));
    }
};

#endif

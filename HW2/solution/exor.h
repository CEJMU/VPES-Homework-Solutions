#ifndef EXOR_H
#define EXOR_H

#include <systemc.h>

#include "nand.h"

SC_MODULE(exor)
{ 
private:
    nand a, b, c, d;

    sc_signal<bool> h1, h2, h3;

public:
    sc_in<bool> A;
    sc_in<bool> B;
    sc_out<bool> Z;

    SC_CTOR(exor) : a("a"), b("b"), c("c"), d("d"), A("A"), B("B"), Z("Z")
    {
        a.A(A);
        a.B(B);
        a.Z(h1);

        b.A(A);
        b.B(h1);
        b.Z(h2);

        c.A(h1);
        c.B(B);
        c.Z(h3);

        d.A(h2);
        d.B(h3);
        d.Z(Z);
    }
};

#endif // EXOR_H

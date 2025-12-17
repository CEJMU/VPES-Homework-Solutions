#pragma once

#include <systemc>

SC_MODULE(kpn)
{
private:
    sc_core::sc_fifo<unsigned int> a;
    sc_core::sc_fifo<unsigned int> b;
    sc_core::sc_fifo<unsigned int> c;
    sc_core::sc_fifo<unsigned int> d;

    void kpn_add();
    void kpn_delay();
    void kpn_split();

    unsigned int counter = 0;

public:
    SC_CTOR(kpn) : a(10), b(10), c(10), d(10)
    {
        SC_THREAD(kpn_add);
        SC_THREAD(kpn_delay);
        SC_THREAD(kpn_split);

        b.write(1);
        c.write(0);
    }
};

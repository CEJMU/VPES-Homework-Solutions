#ifndef KPN_H
#define KPN_H

#include <systemc.h>

SC_MODULE(kpn)
{
private:
    sc_fifo<unsigned int> a, b, c, d;

    void kpn_add();
    void kpn_delay();
    void kpn_split();

    unsigned int counter;

public:
    SC_CTOR(kpn) : a(10), b(10), c(10), d(10)
    {
       SC_THREAD(kpn_add);
       SC_THREAD(kpn_delay);
       SC_THREAD(kpn_split);

       // Initialization:
       b.write(1);
       c.write(0);

       counter = 0;
    }
};

#endif // KPN_H

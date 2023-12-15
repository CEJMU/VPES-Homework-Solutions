#include <unistd.h>
#include <systemc.h>

#include "kpn.h"

void kpn::kpn_add()
{
    while (true)
    {
        b.write(a.read() + c.read());
    }
}

void kpn::kpn_delay()
{
    while (true)
    {
        c.write(d.read());
    }
}

void kpn::kpn_split()
{
    while(true)
    {
        unsigned int value = b.read();
        a.write(value);
        d.write(value);
        std::cout << value << ", ";
        if (counter++ > 10)
        {
            sc_stop();
        }
    }
}


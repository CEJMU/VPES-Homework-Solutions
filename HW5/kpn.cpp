#include "kpn.h"

#include <iomanip>

void kpn::kpn_add()
{
    while (true)
    {
        auto a_value = a.read();
        auto c_value = c.read();

        sc_core::wait(3, sc_core::SC_NS);
        b.write(a_value + c_value);
    }
}

void kpn::kpn_delay()
{
    while (true)
    {
        auto d_value = d.read();

        sc_core::wait(10, sc_core::SC_NS);
        c.write(d_value);
    }
}

void kpn::kpn_split()
{
    while (true)
    {
        // Why now call b.read() two times?
        unsigned int b_value = b.read();

        sc_core::wait(1, sc_core::SC_NS);
        a.write(b_value);
        d.write(b_value);

        std::cout << "@" << std::setw(6) << sc_core::sc_time_stamp() << ": " << std::setw(3)
                  << b_value << "\n";

        if (counter++ > 10)
        {
            std::cout << "...\n";
            sc_core::sc_stop();
        }
    }
}

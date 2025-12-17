#include <systemc>

#include "kpn.h"

int sc_main(int argc, char** argv)
{
    kpn kahn("kpn");
    sc_core::sc_start();
    return 0;
}

#include <systemc.h>
#include <iostream>

#include "kpn.h"

int sc_main(int, char**)
{
    kpn kahn("kpn");
    sc_start();
    return 0;
}

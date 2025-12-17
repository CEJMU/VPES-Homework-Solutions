#include <systemc>

#include "memory.h"
#include "processor.h"
#include "bus.h"

int sc_main (int argc, char **argv)
{
    processor cpu0("cpu0", "stimuli1.txt", sc_core::sc_time(1, sc_core::SC_NS));
    processor cpu1("cpu1", "stimuli2.txt", sc_core::sc_time(1, sc_core::SC_NS));
    memory<512> memory0("memory0");
    memory<512> memory1("memory1");
    bus bus("bus");

    cpu0.iSocket.bind(bus.tSocket[0]);
    cpu1.iSocket.bind(bus.tSocket[1]);
    bus.iSocket[0].bind(memory0.tSocket);
    bus.iSocket[1].bind(memory1.tSocket);

    sc_core::sc_start();
    return 0;
}

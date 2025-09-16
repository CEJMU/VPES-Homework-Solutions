#include "exor.h"
#include "stim.h"
#include "mon.h"

#include <systemc>

using namespace sc_core;

int sc_main(int argc, char** argv)
{
    sc_signal<bool> sigA;
    sc_signal<bool> sigB;
    sc_signal<bool> sigZ;
    sc_clock clk("clk", sc_time(1, SC_NS));

    stim Stim1("Stimulus");
    Stim1.A(sigA);
    Stim1.B(sigB);
    Stim1.clk(clk);

    exor DUT("exor");
    DUT.A(sigA);
    DUT.B(sigB);
    DUT.Z(sigZ);

    Monitor mon("Monitor");
    mon.A(sigA);
    mon.B(sigB);
    mon.Z(sigZ);

    sc_trace_file* Tf = nullptr;
    Tf = sc_create_vcd_trace_file("traces");
    sc_trace(Tf, clk, "clk");
    sc_trace(Tf, sigA, "A");
    sc_trace(Tf, sigB, "B");
    sc_trace(Tf, sigZ, "Z");

    sc_start();
    return 0;
}

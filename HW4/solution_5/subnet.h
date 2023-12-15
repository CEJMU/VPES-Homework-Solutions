#ifndef SUBNET_H
#define SUBNET_H

#include <systemc.h>

#include "place.h"
#include "transition.h"

// Subnet:
SC_MODULE(subnet)
{
public:
    sc_port<placeInterface> in;
    sc_port<placeInterface> out;

    transition<1, 1, 1> ACT;
    transition<1, 1, 0> RD;
    transition<1, 1, 0> PRE;
    transition<1, 1, 0> WR;
    place<1, 1> ACTIVE;

    SC_CTOR(subnet) : ACT("ACT"), RD("RD"), PRE("PRE"), WR("WR"), ACTIVE(0)
    {
        ACT.in.bind(in);
        ACT.out.bind(ACTIVE);
        ACT.inhibitors.bind(ACTIVE);
        RD.in.bind(ACTIVE);
        RD.out.bind(ACTIVE);
        WR.in.bind(ACTIVE);
        WR.out.bind(ACTIVE);
        PRE.in.bind(ACTIVE);
        PRE.out.bind(out);
    }
};

#endif // SUBNET_H

#pragma once

#include "place.h"

#include <systemc>

// Transition:
SC_MODULE(transition)
{
public:
    SC_CTOR(transition) {}

    sc_core::sc_port<placeInterface> in; 
    sc_core::sc_port<placeInterface> out;

    void fire()
    {
        if (in->testTokens() >= 1)
        {
            std::cout << this->name() << ": Fired\n";
            in->removeTokens(1);
            out->addTokens(1);
        }
        else
        {
            std::cout << this->name() << ": NOT Fired\n";
        }
    }
};

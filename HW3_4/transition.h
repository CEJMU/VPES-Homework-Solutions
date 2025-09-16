#pragma once

#include <systemc>

#include "place.h"

// Transition:
template<unsigned int N = 1, unsigned int M = 1>
SC_MODULE(transition)
{
public:
    SC_CTOR(transition) {}

    sc_core::sc_port<placeInterface, N, sc_core::SC_ALL_BOUND> in;
    sc_core::sc_port<placeInterface, M, sc_core::SC_ALL_BOUND> out;

    void fire()
    {
        bool enoughTokens = true;

        for (unsigned int i = 0; i < N; i++)
        {
            if (!in[i]->testTokens())
            {
                enoughTokens = false;
                break;
            }
        }

        if (enoughTokens)
        {
            std::cout << this->name() << ": Fired\n";

            for (unsigned int i = 0; i < N; i++)
                in[i]->removeTokens();

            for (unsigned int i = 0; i < M; i++)
                out[i]->addTokens();
        }
        else
        {
            std::cout << this->name() << ": NOT Fired\n";
        }
    }
};

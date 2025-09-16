#pragma once

#include <systemc>

#include "place.h"

// Transition:
template<unsigned int N = 1, unsigned int M = 1, unsigned int L = 0>
SC_MODULE(transition)
{
public:
    SC_CTOR(transition) {}

    sc_core::sc_port<placeInterface, N, sc_core::SC_ALL_BOUND> in;
    sc_core::sc_port<placeInterface, M, sc_core::SC_ALL_BOUND> out;
    sc_core::sc_port<placeInterface, L, sc_core::SC_ZERO_OR_MORE_BOUND> inhibitors;

    void fire()
    {
        bool enoughTokens = true;
        bool noInhibitance = true;

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
            for (unsigned int i = 0; i < L; i++)
            {
                if (inhibitors[i]->testTokens())
                {
                    noInhibitance = false;
                    break;
                }
            }
        }

        if (enoughTokens && noInhibitance)
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

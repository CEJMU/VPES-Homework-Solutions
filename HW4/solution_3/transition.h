#ifndef TRANSITION_H
#define TRANSITION_H

#include <systemc.h>

#include "place.h"

// Transition:
template<unsigned int N = 1, unsigned int M = 1>
SC_MODULE(transition)
{
public:
    SC_CTOR(transition) {}

    sc_port<placeInterface, N, SC_ALL_BOUND> in;
    sc_port<placeInterface, M, SC_ALL_BOUND> out;

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
            std::cout << this->name() << ": Fired" << std::endl;

            for (unsigned int i = 0; i < N; i++)
                in[i]->removeTokens();

            for (unsigned int i = 0; i < M; i++)
                out[i]->addTokens();
        }
        else
        {
            std::cout << this->name() << ": NOT Fired" << std::endl;
        }
    }
};

#endif // TRANSITION_H

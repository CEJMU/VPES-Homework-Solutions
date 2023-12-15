#ifndef PLACE_H
#define PLACE_H

#include <systemc.h>

// Place Interface:
class placeInterface : public sc_interface
{
public:
    virtual void addTokens() = 0;
    virtual void removeTokens() = 0;
    virtual bool testTokens() = 0;
};

// Place Channel:
template<unsigned int Win = 1, unsigned int Wout = 1>
class place : public placeInterface
{
public:
    place(unsigned int tokens) : tokens(tokens) {}

    virtual void addTokens() override
    {
        tokens += Win;
    }

    virtual void removeTokens() override
    {
        tokens -= Wout;
    }

    virtual bool testTokens() override
    {
        return tokens >= Wout;
    }

private:
    unsigned int tokens;
};

#endif // PLACE_H

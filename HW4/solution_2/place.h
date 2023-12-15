#ifndef PLACE_H
#define PLACE_H

#include <systemc.h>

// Place Interface:
class placeInterface : public sc_interface
{
public:
    virtual void addTokens(unsigned int n) = 0;
    virtual void removeTokens(unsigned int n) = 0;
    virtual unsigned int testTokens() = 0;
};

// Place Channel:
class place : public placeInterface
{
public:
    place(unsigned int tokens) : tokens(tokens) {}

    virtual void addTokens(unsigned int n) override
    {
        tokens += n;
    }

    virtual void removeTokens(unsigned int n) override
    {
        tokens -= n;
    }

    virtual unsigned int testTokens() override
    {
        return tokens;
    }

private:
    unsigned int tokens;
};

#endif // PLACE_H

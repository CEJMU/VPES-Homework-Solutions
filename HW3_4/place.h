#pragma once

#include <systemc>

// Place Interface:
class placeInterface : public virtual sc_core::sc_interface
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

    void addTokens() override
    {
        tokens += Win;
    }

    void removeTokens() override
    {
        tokens -= Wout;
    }

    bool testTokens() override
    {
        return tokens >= Wout;
    }

private:
    unsigned int tokens;
};

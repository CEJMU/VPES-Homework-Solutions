#pragma once

#include <systemc>

// Place Interface:
class placeInterface : public virtual sc_core::sc_interface
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

    void addTokens(unsigned int n) override
    {
        tokens += n;
    }

    void removeTokens(unsigned int n) override
    {
        tokens -= n;
    }

    unsigned int testTokens() override
    {
        return tokens;
    }

private:
    unsigned int tokens;
};

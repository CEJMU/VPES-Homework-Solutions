#pragma once

#include <format>
#include <iostream>
#include <queue>

#include <systemc>
#include <tlm>

// Convenience Sockets:
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

// PEQ:
#include <tlm_utils/peq_with_cb_and_phase.h>

// Internal Phase for transaction processing:
DECLARE_EXTENDED_PHASE(INTERNAL);

using namespace std;

SC_MODULE(Target)
{
public:
    tlm_utils::simple_target_socket<Target> tSocket;

private:
    static constexpr unsigned MEMORY_SIZE = 512;
    std::array<uint8_t, MEMORY_SIZE> mem{};

    static constexpr unsigned ACCEPT_DELAY_NS = 10;
    static constexpr unsigned INTERNAL_DELAY_NS = 40;

    unsigned int numberOfTransactions = 0;
    unsigned int bufferSize;

public:
    Target(const sc_core::sc_module_name& name, unsigned int bufferSize) :
        sc_module(name),
        tSocket("tSocket"),
        bufferSize(bufferSize)
    {
    }

    // Your code here...
};

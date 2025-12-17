#pragma once

#include <tlm>
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>

SC_MODULE(Interconnect)
{
public:
    tlm_utils::multi_passthrough_target_socket<Interconnect> tSocket;
    tlm_utils::multi_passthrough_initiator_socket<Interconnect> iSocket;

    SC_CTOR(Interconnect){}

    // Your code here...
};
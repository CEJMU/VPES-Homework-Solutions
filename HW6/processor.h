#pragma once

#include <string>

#include <systemc>
#include <tlm>
#include <tlm_utils/tlm_quantumkeeper.h>

class processor : public sc_core::sc_module, tlm::tlm_bw_transport_if<>
{
private:
    std::ifstream file;
    sc_core::sc_time cycleTime;

    tlm_utils::tlm_quantumkeeper quantumKeeper;

    // Method:
    void processTrace();
    void processRandom();

public:
    tlm::tlm_initiator_socket<> iSocket;

    processor(sc_core::sc_module_name, std::string pathToTrace, sc_core::sc_time cycleTime);

    // Dummy method:
    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range)
    {
        SC_REPORT_FATAL(this->name(), "invalidate_direct_mem_ptr not implement");
    }

    // Dummy method:
    tlm::tlm_sync_enum
    nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay)
    {
        SC_REPORT_FATAL(this->name(), "nb_transport_bw is not implemented");
        return tlm::TLM_ACCEPTED;
    }
};

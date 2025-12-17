#pragma once

#include <format>
#include <iostream>

#include <random>
#include <systemc>
#include <tlm>

// Convenience Sockets:
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

// PEQ:
#include <tlm_utils/peq_with_cb_and_phase.h>

// MM and tools:
#include "memory_manager.h"

using namespace std;

SC_MODULE(Initiator)
{
public:
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_initiator_socket<Initiator> iSocket;

private:
    static constexpr unsigned NUM_REQUESTS = 8192;
    static constexpr unsigned GENERATOR_PERIOD_NS = 10;
    static constexpr unsigned MAX_ADDR = 1024 - 1;
    std::uniform_int_distribution<unsigned> addressDistribution{0, MAX_ADDR};
    std::default_random_engine randomGenerator;

    static constexpr unsigned BUFFER_SIZE = 16;
    std::array<int, BUFFER_SIZE> data{};

    MemoryManager* memoryManager;
    tlm::tlm_generic_payload* requestInProgress{};
    sc_core::sc_event endRequest;
    tlm_utils::peq_with_cb_and_phase<Initiator> peq;

public:
    Initiator(const sc_core::sc_module_name& name, MemoryManager* memoryManager) :
        sc_core::sc_module(name),
        iSocket("iSocket"),
        memoryManager(memoryManager),
        peq(this, &Initiator::peqCallback)
    {
        iSocket.register_nb_transport_bw(this, &Initiator::nb_transport_bw);

        SC_THREAD(process);
    }

protected:
    void process()
    {
        for (int i = 0; i < NUM_REQUESTS; i++)
        {
            unsigned adr = addressDistribution(randomGenerator);
            tlm::tlm_command cmd = tlm::TLM_READ_COMMAND;

            // Grab a new transaction from the memory manager
            tlm::tlm_generic_payload* trans = memoryManager->allocate();
            trans->acquire();

            trans->set_command(cmd);
            trans->set_address(static_cast<uint64_t>(adr));
            trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data[i % BUFFER_SIZE]));
            trans->set_data_length(4);
            trans->set_streaming_width(4);
            trans->set_byte_enable_ptr(nullptr);
            trans->set_dmi_allowed(false);
            trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

            // Initiator must follow the BEGIN_REQ/END_REQ exclusion rule:
            if (requestInProgress != nullptr)
            {
                wait(endRequest);
            }
            requestInProgress = trans;
            tlm::tlm_phase phase = tlm::BEGIN_REQ;

            // Timing annot. models processing time of initiator prior to call
            sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

            std::cout << "\033[1;31m"
                      << std::format("({})@{:>12}: {} Addr = {:08d} Data = 0x{:08X}(nb_transport)",
                                     name(),
                                     sc_core::sc_time_stamp().to_string(),
                                     cmd == tlm::TLM_WRITE_COMMAND ? "Write to" : "Read from",
                                     adr,
                                     data[i % BUFFER_SIZE])
                      << "\033[0m\n";

            // Non-blocking transport call on the forward path
            // Call [1.0]:
            tlm::tlm_sync_enum status = iSocket->nb_transport_fw(*trans, phase, delay);

            // Check value returned from nb_transport_fw
            if (status == tlm::TLM_UPDATED) // [2.0] or [4.0]
            {
                // The timing annotation must be honored
                peq.notify(*trans, phase, delay);
            }
            else if (status == tlm::TLM_COMPLETED) // [3.0]
            {
                // The completion of the transaction
                // necessarily ends the BEGIN_REQ phase
                requestInProgress = nullptr;

                // The target has terminated the transaction
                checkTransaction(*trans);

                // Allow the memory manager to free the transaction object
                trans->release();
            }
            // In the case of TLM_ACCEPTED [1.1] we
            // will recv. a BW call in the future [1.2, 1.4]

            wait(GENERATOR_PERIOD_NS, sc_core::SC_NS);
        }
    }

    // [1.2, 1.4]
    virtual tlm::tlm_sync_enum nb_transport_bw(
        tlm::tlm_generic_payload & trans, tlm::tlm_phase & phase, sc_core::sc_time & delay)
    {
        // Queue the transaction into the peq until
        // the annotated time has elapsed
        peq.notify(trans, phase, delay);

        // HINT: a Return Path shortcut can be implemented here [2.1]

        return tlm::TLM_ACCEPTED; // [1.3, 1.5]
    }

    // Payload event queue callback
    void peqCallback(tlm::tlm_generic_payload & trans, const tlm::tlm_phase& phase)
    {
        if (phase == tlm::END_REQ // <-- [1.2, 2.0]
                                  // or [4.0] --V
            || (&trans == requestInProgress && phase == tlm::BEGIN_RESP))
        {
            // The end of the BEGIN_REQ phase
            requestInProgress = nullptr;
            endRequest.notify(); // wake up suspended main process
        }
        else if (phase == tlm::BEGIN_REQ || phase == tlm::END_RESP)
        {
            SC_REPORT_FATAL(name(), "Illegal transaction phase received");
        }

        if (phase == tlm::BEGIN_RESP) // [1.4]
        {
            checkTransaction(trans);

            // Send final phase transition to target
            tlm::tlm_phase fw_phase = tlm::END_RESP;
            sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
            // [1.6]
            iSocket->nb_transport_fw(trans, fw_phase, delay); // Ignore return

            // Allow the memory manager to free the transaction object
            trans.release();
        }
    }

    // Called on receiving BEGIN_RESP or TLM_COMPLETED
    void checkTransaction(tlm::tlm_generic_payload & trans)
    {
        if (trans.is_response_error())
        {
            SC_REPORT_ERROR(name(), "Transaction returned with error!");
        }

        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64 adr = trans.get_address();
        int* ptr = reinterpret_cast<int*>(trans.get_data_ptr());

        std::cout << "\033[1;31m"
                  << std::format("({})@{:>12}: Check {} Addr = {:08d} Data = 0x{:08X}",
                                 name(),
                                 sc_core::sc_time_stamp().to_string(),
                                 cmd == tlm::TLM_WRITE_COMMAND ? "Write" : "Read",
                                 adr,
                                 *ptr)
                  << "\033[0m\n";
    }
};

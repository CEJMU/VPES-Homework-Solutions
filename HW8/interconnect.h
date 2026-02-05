#pragma once

#include <format>
#include <iostream>
#include <tlm>
#include <tlm_utils/multi_passthrough_initiator_socket.h>
#include <tlm_utils/multi_passthrough_target_socket.h>

struct RoutingExtension : public tlm::tlm_extension<RoutingExtension>
{
    int inIdx{};
    int outIdx{};

    [[nodiscard]] tlm_extension_base* clone() const override { return new RoutingExtension(*this); }

    void copy_from(const tlm_extension_base& ext) override
    {
        const auto& cpyFrom = dynamic_cast<const RoutingExtension&>(ext);
        inIdx = cpyFrom.inIdx;
        outIdx = cpyFrom.outIdx;
    }
};

SC_MODULE(Interconnect)
{
public:
    tlm_utils::multi_passthrough_target_socket<Interconnect> tSocket;
    tlm_utils::multi_passthrough_initiator_socket<Interconnect> iSocket;

    Interconnect(const sc_core::sc_module_name& name) :
        sc_core::sc_module(name),
        tSocket("tSocket"),
        iSocket("iSocket")
    {
        tSocket.register_nb_transport_fw(this, &Interconnect::nb_transport_fw);
        iSocket.register_nb_transport_bw(this, &Interconnect::nb_transport_bw);
    }

private:
    static int routeFW(int inPort, tlm::tlm_generic_payload& trans)
    {
        int outPort = 0;

        // Memory map implementation:
        if (trans.get_address() < 512)
        {
            outPort = 0;
        }
        else if (trans.get_address() >= 512 && trans.get_address() < 1024)
        {
            // Correct Address:
            trans.set_address(trans.get_address() - 512);
            outPort = 1;
        }
        else
        {
            trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        }

        auto* ext = new RoutingExtension;
        ext->inIdx = inPort;
        ext->outIdx = outPort;
        trans.set_auto_extension(ext);

        std::cout << "\033[1;36m"
                  << std::format("(E) @{:>12}: Extension Created (inPort = {:>2} outPort = {:>2})",
                                 sc_core::sc_time_stamp().to_string(),
                                 inPort,
                                 outPort)
                  << "\033[0m\n";

        return outPort;
    }

    virtual tlm::tlm_sync_enum nb_transport_fw(
        int id, tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay)
    {
        int outPort = 0;

        if (phase == tlm::BEGIN_REQ)
        {
            // In the case of nb_transport_fw the address attribute is valid
            // immediately upon entering the function but only when the phase
            // is BEGIN_REQ. Following the return from any forward path TLM-2.0
            // interface method call, the address attribute will have the value
            // set by the interconnect component lying furthest downstream, and
            // so should be regarded as being undefined for the purposes of
            // transaction routing.
            trans.acquire();

            // Modify address accoring to memory map:
            outPort = routeFW(id, trans);
        }
        else if (phase == tlm::END_RESP)
        {
            // Adress was already modified in BEGIN_REQ phase:
            RoutingExtension* ext = nullptr;
            trans.get_extension(ext);
            outPort = ext->outIdx;
            trans.release();
        }
        else
        {
            SC_REPORT_FATAL(name(), "Illegal phase received by initiator");
        }

        std::cout << "\033[1;37m"
                  << std::format(
                         "({})@{:>12}: Addr = {:08d}  inPort = {:>2} outPort = {:>2} ptr = {}",
                         name(),
                         sc_core::sc_time_stamp().to_string(),
                         trans.get_address(),
                         id,
                         outPort,
                         static_cast<void*>(&trans))
                  << "\033[0m\n";

        return iSocket[outPort]->nb_transport_fw(trans, phase, delay);
    }

    virtual tlm::tlm_sync_enum nb_transport_bw(
        int id, tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& delay)
    {
        RoutingExtension* ext = nullptr;
        trans.get_extension(ext);
        int inPort = ext->inIdx;

        return tSocket[inPort]->nb_transport_bw(trans, phase, delay);
    }
};
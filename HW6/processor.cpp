#include "processor.h"

#include <iomanip>
#include <iostream>
#include <random>
#include <regex>

processor::processor(sc_core::sc_module_name, std::string pathToFile, sc_core::sc_time cycleTime) :
    file(pathToFile),
    cycleTime(cycleTime)
{
    // if (!file.is_open())
    //     SC_REPORT_FATAL(name(), "Could not open trace");

    // SC_THREAD(processTrace);
    SC_THREAD(processRandom);

    tlm_utils::tlm_quantumkeeper::set_global_quantum(sc_core::sc_time(100000, sc_core::SC_NS));
    quantumKeeper.reset();

    iSocket.bind(*this);
}

void processor::processTrace()
{
    wait(sc_core::SC_ZERO_TIME);
    std::string line;
    tlm::tlm_generic_payload trans;
    unsigned long long cycles = 0;
    unsigned long long address = 0;
    std::string dataStr;
    std::array<unsigned char, 4> data;
    bool read = true;

    while (std::getline(file, line))
    {
        std::regex reW(R"((\d+)\s*:\s*write\s+0x([\d\w]+)\s+0x([\d\w]+))");
        std::regex reR(R"((\d+)\s*:\s*read\s+0x([\d\w]+))");
        std::smatch matchW;
        std::smatch matchR;

        if (std::regex_search(line, matchW, reW) && matchW.size() > 1)
        {
            cycles = std::stoull(matchW.str(1), nullptr, 10);
            read = false;
            address = std::stoull(matchW.str(2), nullptr, 16);
            dataStr = matchW.str(3);
            for (int i = 0; i < 4; i++)
            {
                data[i] = (unsigned char)std::stoi(dataStr.substr(i * 2, 2), nullptr, 16);
            }
        }
        else if (std::regex_search(line, matchR, reR) && matchR.size() > 1)
        {
            cycles = std::stoull(matchR.str(1), nullptr, 10);
            read = true;
            address = std::stoull(matchR.str(2), nullptr, 16);
        }
        else
        {
            SC_REPORT_FATAL(name(), "Syntax error in trace");
        }

        sc_core::sc_time delay;

        if (sc_core::sc_time_stamp() <= cycles * cycleTime)
        {
            delay = cycles * cycleTime - sc_core::sc_time_stamp();
        }
        else
        {
            delay = sc_core::sc_time(0, sc_core::SC_NS);
        }

        trans.set_address(address);
        trans.set_data_length(4);
        trans.set_command(read ? tlm::TLM_READ_COMMAND : tlm::TLM_WRITE_COMMAND);
        trans.set_data_ptr(data.data());
        iSocket->b_transport(trans, delay);

        wait(delay);

        std::cout << std::setfill(' ') << std::setw(4) << name() << " " << std::setfill(' ')
                  << std::setw(10) << sc_core::sc_time_stamp() << " " << std::setfill(' ')
                  << std::setw(5) << (read ? "read" : "write") << " 0x" << std::setfill('0')
                  << std::setw(8) << address << " 0x" << std::hex << std::setfill('0')
                  << std::setw(2) << (unsigned int)data[0] << std::setfill('0') << std::setw(2)
                  << (unsigned int)data[1] << std::setfill('0') << std::setw(2)
                  << (unsigned int)data[2] << std::setfill('0') << std::setw(2)
                  << (unsigned int)data[3] << std::endl;
    }

    // End Simulation because there are no events.
}

void processor::processRandom()
{
    wait(sc_core::SC_ZERO_TIME);

    tlm::tlm_generic_payload trans;

    uint64_t cycles;
    uint64_t address;
    unsigned char data[4];

    std::default_random_engine randGenerator;
    std::uniform_int_distribution<uint64_t> distrCycle(0, 99);
    std::uniform_int_distribution<uint64_t> distrAddr(0, 1023);

    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    trans.set_data_length(4);
    trans.set_command(tlm::TLM_WRITE_COMMAND);
    trans.set_data_ptr(data);

    for (uint64_t transId = 0; transId < 100000000; transId++)
    {
        cycles = distrCycle(randGenerator);
        address = distrAddr(randGenerator);

        sc_core::sc_time delay = quantumKeeper.get_local_time();
        if (quantumKeeper.get_current_time() <= cycles * cycleTime)
        {
            delay += cycles * cycleTime - quantumKeeper.get_current_time();
        }
        trans.set_address(address);
        iSocket->b_transport(trans, delay);
        quantumKeeper.set(delay);
        if (quantumKeeper.need_sync())
            quantumKeeper.sync();
    }

    // End Simulation because there are no events.
}

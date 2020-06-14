/** @file DRAM.h
 *  @brief Function prototypes for the DRAM module.
 *
 *  This contains DRAM module
 *
 *  @Author: HarveyZeng (caslab), LanceHuang (caslab)
 *
 *  @bug No known bugs.
 */
#ifndef DRAM_H
#define DRAM_H

#include <fstream>
#include <string>
#include <sstream>
#include "ps_config.h"
#include "general_function.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

DECLARE_EXTENDED_PHASE(internal_ph);
using namespace std;

struct DRAM: sc_module {
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_target_socket<DRAM> socket;

    SC_CTOR(DRAM)
        : socket("socket")
        , n_trans(0)
        , response_in_progress(false)
        , next_response_pending(0)
        , end_req_pending(0)
        , m_peq(this, &DRAM::peq_cb)
    {
        // Register callbacks for incoming interface method calls
        socket.register_nb_transport_fw(this, &DRAM::nb_transport_fw);
        dram_id = DRAM4_ID;
        row_index = 0xffffffff; // Initial value
        for(int i=0; i<(MEM_SIZE/4); i++){
            mem[i] = 0;
        }
        
    }

    // TLM-2 non-blocking transport method

    virtual tlm::tlm_sync_enum nb_transport_fw( tlm::tlm_generic_payload& trans,
            tlm::tlm_phase& phase, sc_time& delay );
    void peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase);
    tlm::tlm_sync_enum send_end_req(tlm::tlm_generic_payload& trans);
    void send_response(tlm::tlm_generic_payload& trans);
    int get_id();
    uint32_t dram_id;
    int   n_trans;
    bool  response_in_progress;
    tlm::tlm_generic_payload*  next_response_pending;
    tlm::tlm_generic_payload*  end_req_pending;
    tlm_utils::peq_with_cb_and_phase<DRAM> m_peq;
    uint32_t mem[MEM_SIZE/4]; // 256MB(8B * 0x2000000)

    // DRAM timing
    int calculate_delay(int addr);
    uint32_t row_hit_miss_latency(int addr);
    int get_row_index(int);
    void access(bool,uint32_t,unsigned int ,uint32_t*);
    int row_index;

};

#endif

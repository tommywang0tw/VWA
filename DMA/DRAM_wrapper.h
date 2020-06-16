#ifndef DRAM_WRAPPER_H
#define DRAM_WRAPPER_H

#include "top.h"
#include "ps_config.h"
#include "general_function.h"
#include "mm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

struct DRAM_wrapper: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_initiator_socket<DRAM_wrapper> socket;

  SC_CTOR(DRAM_wrapper)
  : socket("socket")  // Construct and name socket
  , request_in_progress(0)
  , m_peq(this, &DRAM_wrapper::peq_cb)
  {
    // Register callbacks for incoming interface method calls
    socket.register_nb_transport_bw(this, &DRAM_wrapper::nb_transport_bw);
    SC_THREAD(thread_process);
  }

  void thread_process();
  void master_access(uint32_t addr,uint32_t & data,bool write, int length);

  // TLM-2 backward non-blocking transport method
  virtual tlm::tlm_sync_enum nb_transport_bw( tlm::tlm_generic_payload& trans,
                                              tlm::tlm_phase& phase, sc_time& delay );
  void peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase);

  mm   m_mm;
  uint32_t  data_read[1];
  uint32_t  data_write[1];


  tlm::tlm_generic_payload* trans;
  sc_event end_request_event;
  tlm_utils::peq_with_cb_and_phase<DRAM_wrapper> m_peq;

  sc_uint<32> temp_addr;
  sc_in<sc_uint<ADDR_LENGTH> > dram_addr_i;// DRAM address
  sc_in<sc_uint<DATA_LENGTH> > data;
  sc_in<sc_uint<1> > wr_enable;
  sc_out<sc_uint<DATA_LENGTH> > dram_dataout_i;

};
#endif
#include "DRAM_wrapper.h"

  void DRAM_wrapper::master_access(uint32_t addr,uint32_t& data,bool write,int length)
  {
    tlm::tlm_generic_payload* trans;
    tlm::tlm_phase phase;
    tlm::tlm_command cmd ;
    sc_time delay;
    if(write==1)
    {
      cmd = tlm::TLM_WRITE_COMMAND; 
    }
    else
    {
      cmd = tlm::TLM_READ_COMMAND; 
    }
    // Grab a new transaction from the memory manager
    trans = m_mm.allocate();
    trans->acquire();

    // Set all attributes except byte_enable_length and extensions (unused)
    trans->set_command( cmd );
    trans->set_address( addr );
    trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data) );
    trans->set_data_length( length ); //data length is 
    trans->set_streaming_width( length ); // = data_length to indicate no streaming
    trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
    trans->set_dmi_allowed( false ); // Mandatory initial value
    trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

    phase = tlm::BEGIN_REQ;

    // Timing annotation models processing time of initiator prior to call
    delay = sc_time(0, SC_PS);

    // Non-blocking transport call on the forward path
    tlm::tlm_sync_enum status;
    status = socket->nb_transport_fw( *trans, phase, delay );
  }


  void DRAM_wrapper::thread_process()
  {
    while(1)
    {
      temp_addr = dram_addr_i.read();
      if(wr_enable.read()){
        data_write[0] = data.read();
        master_access(temp_addr,*data_write,1,1);//write  length=1
      }
      else{
        master_access(temp_addr,*data_read,0,1);//read  length=1
        dram_dataout_i.write(data_read[0]);
      }
      wait( sc_time(CLK_CYCLE, SC_NS) );
    }
  }

  // TLM-2 backward non-blocking transport method

   tlm::tlm_sync_enum DRAM_wrapper::nb_transport_bw( tlm::tlm_generic_payload& trans,
                                              tlm::tlm_phase& phase, sc_time& delay )
  {
    // The timing annotation must be honored
    m_peq.notify( trans, phase, delay );
    return tlm::TLM_ACCEPTED;
  }

  // Payload event queue callback to handle transactions from target
  // Transaction could have arrived through return path or backward path

  void DRAM_wrapper::peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase)
  {

    if (phase == tlm::END_REQ || (&trans == request_in_progress && phase == tlm::BEGIN_RESP))
    {
      // The end of the BEGIN_REQ phase
    }
    else if (phase == tlm::BEGIN_REQ || phase == tlm::END_RESP)
      SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by initiator");

    if (phase == tlm::BEGIN_RESP)
    {
      trans.release();
      
      tlm::tlm_phase fw_phase = tlm::END_RESP;
      sc_time delay = sc_time(0, SC_PS);
      socket->nb_transport_fw( trans, fw_phase, delay );
    }
  }


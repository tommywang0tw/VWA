/** @file DRAM.cpp
 *  @brief Function prototypes for the DRAM module.
 *
 *  This contains DRAM module
 *
 *  @author HarveyZeng (caslab)
 *
 *  @bug No known bugs.
 */
#include "DRAM.h"

// TLM-2 non-blocking transport method

tlm::tlm_sync_enum DRAM::nb_transport_fw( tlm::tlm_generic_payload& trans,
        tlm::tlm_phase& phase, sc_time& delay )
{
    
    m_peq.notify( trans, phase, delay);
    return tlm::TLM_ACCEPTED;

}

void DRAM::peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase)
{
    tlm::tlm_sync_enum status;
    sc_time delay;
    delay = sc_time(0, SC_PS); // Accept delay
    switch (phase) {
        case tlm::BEGIN_REQ:
            // Increment the transaction reference count
            trans.acquire();
            // Put back-pressure on initiator by deferring END_REQ until pipeline is clear
 
            
            status = send_end_req(trans);
            if (status == tlm::TLM_COMPLETED) // It is questionable whether this is valid
                break;
            break;

        case tlm::END_RESP:
            // On receiving END_RESP, the target can release the transaction
            // and allow other pending transactions to proceed
            trans.release();
            n_trans--;

            // Target itself is now clear to issue the next BEGIN_RESP
            // response_in_progress = false;
            // if (next_response_pending)
            // {
            //   send_response( *next_response_pending );
            //   next_response_pending = 0;
            // }

            // // ... and to unblock the initiator by issuing END_REQ
            // if (end_req_pending)
            // {
            //   status = send_end_req( *end_req_pending );
            //   end_req_pending = 0;
            // }

            break;

        case tlm::END_REQ:
        case tlm::BEGIN_RESP:
            SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by target");
            break;

        default:
            if (phase == internal_ph) {
                // Execute the read or write commands
                
                //cout << " DRAM " << "dataIN:" << int(*trans.get_data_ptr()) << " | addrIN :"<< trans.get_address() << "\n";
                tlm::tlm_command cmd = trans.get_command();
                uint32_t    adr = trans.get_address();

                unsigned char*   ptr = trans.get_data_ptr();
                unsigned int     len = trans.get_data_length();
                unsigned char*   byt = trans.get_byte_enable_ptr();

                if ( cmd == tlm::TLM_READ_COMMAND ) {
                    memcpy(ptr, &mem[adr/4], len);
                    // cout << "adr: " << adr << endl;
                    // cout << "time: " << sc_time_stamp() << endl;
                    //cout << "mem[" << adr/4 << "] " << mem[adr/4] << endl;
                    // cout << "mem[18]" << mem[18] << endl;
                    
                    cout << "mem[" << adr/4 << "] *****" << mem[adr/4] << endl;
                    
                } else if ( cmd == tlm::TLM_WRITE_COMMAND ) {
                    if (byt) {
                        for (unsigned int i = 0; i < len/8; i++) {
                            uint32_t tmp=mem[adr/8+i];
                            bool byte_enable[8];
                            for(int j = 0; j < 8; j++) {
                                byte_enable[j]=(byt[i]&(1<<j))>>j;
                            }
                            for (unsigned int j = 0; j < 8; j++) {
                                if(byte_enable[j])
                                    tmp=((tmp&~((uint32_t)0x000000ff<<(8*j))) |(uint32_t)((uint32_t)ptr[i*8+j]<<(8*j)));
                            }
                            mem[adr/8+i] = tmp;
                            //cout<<"After Write :mem["<<(adr/8+i)<<"]="<<mem[adr/8+i]<<endl;
                        }

                    } else
                        memcpy(&mem[adr/4], ptr, len);

                }
                
                // Target must honor BEGIN_RESP/END_RESP exclusion rule
                // i.e. must not send BEGIN_RESP until receiving previous END_RESP or BEGIN_REQ
                // if (response_in_progress)
                // {
                //   // Target allows only two transactions in-flight
                //   if (next_response_pending)
                //     //cout<<"warning:Attempt to have two pending responses in target"<<endl;
                //     SC_REPORT_FATAL("TLM-2", "Attempt to have two pending responses in target");
                //   next_response_pending = &trans;
                // }
                // else
                trans.set_response_status( tlm::TLM_OK_RESPONSE );
                send_response(trans);
                
                break;
            }
    }
}

tlm::tlm_sync_enum DRAM::send_end_req(tlm::tlm_generic_payload& trans)
{
    tlm::tlm_sync_enum status;
    tlm::tlm_phase bw_phase;
    tlm::tlm_phase int_phase = internal_ph;
    sc_time delay;
    
    // Queue the acceptance and the response with the appropriate latency
    bw_phase = tlm::END_REQ;
    delay = sc_time(0, SC_PS); // Accept delay

    
    status = socket->nb_transport_bw( trans, bw_phase, delay );
    if (status == tlm::TLM_COMPLETED) {
        // Transaction aborted by the initiator
        // (TLM_UPDATED cannot occur at this point in the base protocol, so need not be checked)
        trans.release();
        return status;
    }

    // Queue internal event to mark beginning of response
    
    
    //cout << " DRAM " << "dataSR :" << int(*trans.get_data_ptr()) << " | addrSR :"<< trans.get_address() << "\n";
    m_peq.notify( trans, int_phase, delay );
    n_trans++;

    return status;
}

void DRAM::send_response(tlm::tlm_generic_payload& trans)
{
    tlm::tlm_sync_enum status;
    tlm::tlm_phase bw_phase;
    sc_time delay;
    int addr = trans.get_address();
    //response_in_progress = true;
    bw_phase = tlm::BEGIN_RESP;
    delay = SC_ZERO_TIME;
    
    int dram_delay=calculate_delay(addr);
    delay = delay + sc_time(dram_delay, SC_NS); // Latency
   
    status = socket->nb_transport_bw( trans, bw_phase, delay );
    //cout << "dataNBF :" << int(*trans.get_data_ptr()) << " | addrNBF :"<< trans.get_address() << "\n";
    if (status == tlm::TLM_UPDATED) {
        // The timing annotation must be honored
        m_peq.notify( trans, bw_phase, delay);
    } else if (status == tlm::TLM_COMPLETED) {
        // The initiator has terminated the transaction
        trans.release();
        n_trans--;
        //response_in_progress = false;
    }
}


int DRAM::get_id()
{
    return dram_id;
}

// TBD: THE tRAS Is not used .should  consider it
int DRAM::calculate_delay(int addr)// CACULATE DRAM DELAY
{
    return magic_delay*row_hit_miss_latency(addr)/CLK_CYCLE*CLK_CYCLE;
}

//Check DRAM Row hit or miss
uint32_t DRAM::row_hit_miss_latency(int addr)
{
    uint32_t dram_latency;

    if(row_index==0xffffffff) { // the first row open
        dram_latency = tRAS+tRCD+tCL;
    } else if(row_index==get_row_index(addr)) { //row hit
        dram_latency = tCL;
    } else { // row miss
        dram_latency = tRP+tRCD+tCL;
    }
    row_index=get_row_index(addr);

    return dram_latency;
}

int DRAM::get_row_index(int addr)
{
    return (addr>>(COLUMN_BIT+BANK_BIT+2));
}

// Driver will directly call this function to do memcpy
void DRAM::access(bool mode,uint32_t addr,unsigned int size,uint32_t* data_ptr)
{
    unsigned char *ptr=reinterpret_cast<unsigned char*>(&mem[0]);
    if(mode==DRIVER_READ) {
        memcpy(data_ptr, (ptr+addr), size);
    } else {
        memcpy((ptr+addr), data_ptr, size);
    }
}

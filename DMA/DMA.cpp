#include "DMA.h" 

void DMA::state_register(){
    while(1){
        if(rst){
            state = 5;
        }   
        else {
            switch (state.read()){
                case IDLE:
                    if (read_in){
                        state = READ_DRAM;
                    }
                    else if (write_back){
                        state = READ_OUTPUT;
                    }
                    else {
                        state = IDLE;
                    }
                break;

                case READ_DRAM :
                    if (dram_done){
                        state = WRITE_SRAM;
                    }
                    else {
                        state = READ_DRAM;
                    }
                break;

                case WRITE_SRAM :
                    if (isram_done | wsram_done){
                        state = WRITE_SRAM;
                    }
                    else {        
                        state = RESET;
                    }
                break;

                case READ_OUTPUT :
                    
                break;

                case WRITE_DRAM :
                    if (dram_done){
                        state = RESET;
                    }
                    else {
                        state = WRITE_DRAM;
                    }
                break;

                case RESET:
                    state = IDLE;
                break;
            }
        }
        wait();
    }
    
}

void DMA::dma_function(){
    switch(state.read()){
        case IDLE:
            
        break;

        case READ_DRAM :
            busy = 1;
            transport(0,source_address.read(), data_buffer , data_length.read() ); 
        break;

        case WRITE_SRAM :
            
                if (target_address.read() < WEIGHT_SRAM_BASE )
                {   
                    cout << target <<"= ="<<endl;
                    last_source = source;
                    input_SRAM_write = 1;
                    to_SRAM_mode = sram_mode;
                    to_SRAM_length = length;
                    for (int i = 0 ; i < 7; i++){
                        to_SRAM_data[i] = data_buffer[i];
                    }
                    if (isram_done | wsram_done){
                        done = 1;
                    }
                }
                else {
                    weight_SRAM_write = 1;
                    to_SRAM_mode = sram_mode;
                    to_SRAM_length = length;
                    for (int i = 0 ; i < 9; i++){
                        weight_data[i] = data_buffer[i];
                    }
                    
                    if (isram_done | wsram_done){
                        done = 1;
                    }
                }

        break;

        case READ_OUTPUT :
            // busy = 1;
            // SRAM_read = 1;
            // SRAM_write = 0;
            
            // if (data_vaild){
            //     data_buffer[length-len_counter] = SRAM_data_in;
            //     len_counter --;
            // }
            // else if (sram_done){

            // }
            // else {
            //     to_SRAM_addresss = source;
            //     rw_length = length;
            //     len_counter = lnength;
            // }
        break;

        case WRITE_DRAM :
            transport(1,target_address.read(), data_buffer , data_length.read() );
            if(dram_done){
                done = 1;
            }
        break;

        case RESET:
            busy = 0;
            input_SRAM_write = 0;
            weight_SRAM_write = 0;
            done = 0;
        break;
    }
    
}

void DMA::transport(bool rw, uint32_t addr, uint32_t* data, uint32_t length){
    tlm::tlm_phase phase;
    tlm::tlm_generic_payload* trans;
    tlm::tlm_command cmd;
    sc_time delay;
    if(rw==1){
        cmd = tlm::TLM_WRITE_COMMAND; 
    }
    else{
        cmd = tlm::TLM_READ_COMMAND; 
    }

    trans = m_mm.allocate();
    trans->acquire();

    trans->set_command( cmd );
    trans->set_address( addr );
    trans->set_data_ptr( reinterpret_cast<unsigned char*> (data) );
    trans->set_data_length( length ); 
    trans->set_streaming_width( length ); 
    trans->set_byte_enable_ptr( 0 ); 
    trans->set_dmi_allowed( false ); 
    trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );

    if (request_in_progress){
        wait(end_request_event);          
    }
    request_in_progress = trans;

    phase = tlm::BEGIN_REQ;
    delay = sc_time(0, SC_PS);
    //cout << "SEND_REQ"<<"+++++++" << sc_time_stamp() << "+++++++" << endl;
    tlm::tlm_sync_enum status;
    
    status = socket->nb_transport_fw( *trans, phase, delay );

}

tlm::tlm_sync_enum DMA::nb_transport_bw ( 
    tlm::tlm_generic_payload& trans,
    tlm::tlm_phase& phase, sc_time& delay)
    {
        
        m_peq.notify( trans, phase, delay );
        
        return tlm::TLM_ACCEPTED;
    
    }

void DMA::peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase){

    tlm::tlm_sync_enum status;
    sc_time delay;
    delay = sc_time(0, SC_PS); // Accept delay
    switch (phase) {

        case tlm::END_REQ:
           
           // cout << "END_REQ"<<"++++" << sc_time_stamp() << "++++" << endl;
            
            break;
        case tlm::BEGIN_RESP:{
                request_in_progress = 0;
                end_request_event.notify();
                trans.release();

                dram_done = 1;

                tlm::tlm_phase fw_phase = tlm::END_RESP;
                sc_time delay = sc_time(0, SC_PS);
                status = socket->nb_transport_fw( trans, fw_phase, delay );

                //cout << "REC_RESP"<<"+++++++" << sc_time_stamp() << "+++++++" << endl;
                //k++;
                //cout << " DMA_RESP" <<k<<endl;
        } break;
        case tlm::BEGIN_REQ :
                SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by initiator");
            break;
        case tlm::END_RESP:
                SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by initiator");
            break;

        default:
            break;
               
    }
}
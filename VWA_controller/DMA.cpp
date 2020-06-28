#include "DMA.h" 

// void DMA::state_register(){
//     while(1){
//         if(rst){
//             state = 5;
//         }   
//         else {
//             switch (state.read()){
//                 case IDLE:
                    
//                 break;

//                 case READ_DRAM :
//                     if (dram_done){
//                         state = WRITE_SRAM;
//                     }
//                     else {
//                         state = READ_DRAM;
//                     }
//                 break;

//                 case WRITE_SRAM :
//                     if ( isram_done | wsram_done ){
//                         if ( length.read() == 0 ){
//                             state = RESET;
//                         }
//                         else {
//                             state = READ_DRAM;
//                         }
//                     }
//                     else {
//                         state = WRITE_SRAM;
//                     }
//                 break;

//                 case READ_OUTPUT :
                    
//                 break;

//                 case WRITE_DRAM :
//                     if (dram_done){
//                         state = RESET;
//                     }
//                     else {
//                         state = WRITE_DRAM;
//                     }
//                 break;

//                 case RESET:
//                     state = IDLE;
//                 break;
//             }
//         }
//         wait();
//     }
    
// }

void DMA::dma_function(){
    while(1){
        if(!rst){
            state = 5;
        }   
        else {
            switch(state.read()){
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
                    cout << "dma receive -> source_addr : " << source_address.read() << endl;
                    cout << "dma receive -> target_addr : " << target_address.read() << endl;
                    input_SRAM_write = 0;
                    weight_SRAM_write = 0;
                    state = WAIT_DRAM_READ;
                    to_SRAM_mode = 1;
                    busy = 1;
                    if (target_address.read() < WEIGHT_SRAM_BASE ){
                        cout << "DMA write INPUTSRAM" << endl;
                        cout << "DMA length : " << length.read() << endl;
                        if (length.read() == 0){
                            source.write(source_address.read() + input_width.read()*4 );
                            source_base.write(source_address.read() );
                            length.write(data_length.read() - tile_high.read()*4 );
                            input_size.write((input_width.read())*(input_width.read()));
                            tile_count.write( tile_width.read()*tile_high.read() - tile_high.read());
                            transport(0,source_address.read(), data_buffer , tile_high.read()*4 );

                        }
                        else if ( tile_count.read() == tile_high.read() ){
                            tile_count.write( (tile_width.read()*tile_high.read()) );
                            source_base.write(source_base.read() + input_size.read()*4);
                            source.write(source_base.read() + input_size.read()*4 );
                            length.write(length.read() - tile_high.read()*4);
                            transport(0,source.read(), data_buffer , tile_high.read()*4 );
                        }
                        else {
                            tile_count.write(tile_count.read() - tile_high.read() );
                            source.write(source.read() + input_width.read()*4);
                            length.write(length.read() - tile_high.read()*4);
                            transport(0,source.read(), data_buffer , tile_high.read()*4 );

                        }
                        
                         

                        
                    }
                    else {
                        cout << "DMA write WEIGHTSRAM" << endl;
                        if (length.read() == 0){
                            if (sram_mode){
                                source.write(source_address.read() + 36 );
                                length.write(data_length.read() - 36 );
                            }
                            else{
                                source.write(source_address.read() + 100 );
                                length.write(data_length.read() - 100 );
                            }
                        }
                        else {
                            if (sram_mode){
                                source.write(source.read() + 36 );
                                length.write(length.read() - 36 ); 
                            }
                            else{ 
                                source.write(source.read() + 100 );
                                length.write(length.read() - 100 );
                            }
                        }

                        transport(0,source.read(), data_buffer , 36 ); 
                    }
                    
                break;

                case WRITE_SRAM :

                    
                    if ( length.read() == 0 ){
                        state = RESET;
                    }
                    else {
                        state = READ_DRAM;

                        
                    }
                   
                    if (target_address.read() < WEIGHT_SRAM_BASE )
                        {   
                            input_SRAM_write = 1;
                            to_SRAM_mode = sram_mode;
                            
                            for (int i = 0 ; i < 7; i++){
                                to_SRAM_data[i] = data_buffer[i];
                            }
                            if (length.read() == 0){
                                done = 1;
                            }
                        }
                        else {
                            weight_SRAM_write = 1;
                            to_SRAM_mode = sram_mode;
                            
                            for (int i = 0 ; i < 9; i++){
                                weight_data[i] = data_buffer[i];
                            }
                            
                            if (length.read() == 0){
                                done = 1;
                            }
                        }
                    

                break;

                case READ_OUTPUT :
                    busy = 1;
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
                    if (dram_done){
                        state = RESET;
                    }
                    else {
                        state = WRITE_DRAM;
                    }
                    transport(1,target_address.read(), data_buffer , data_length.read() );
                    if(dram_done){
                        done = 1;
                    }
                break;

                case RESET:
                    state = IDLE;
                    busy = 0;
                    input_SRAM_write = 0;
                    weight_SRAM_write = 0;
                    done = 0;
                    length = 0 ;
                    tile_count = 0;
                    input_size = 0;
                    for (int i = 0 ; i < 9; i++){
                        data_buffer[i] = 0;
                    }
                   
                break;

                case WAIT_DRAM_READ:
                    if (dram_done){
                        state = WRITE_SRAM;
                    }
                    else {
                        state = WAIT_DRAM_READ;
                    }
                break;
            }

        }   
        wait();
        
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
            dram_done = 0;
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
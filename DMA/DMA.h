#ifndef DMA_H
#define DMA_H



#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"
#include "mm.h"
#include "ps_config.h"

#define IDLE (0)
#define READ_DRAM (1)
#define WRITE_SRAM (2) 
#define READ_OUTPUT (3)
#define WRITE_DRAM (4) 
#define RESET (5)

SC_MODULE(DMA){

    tlm_utils::simple_initiator_socket<DMA> socket;

    SC_CTOR(DMA):
        socket("socket")
        ,request_in_progress(0)
        ,m_peq(this , &DMA::peq_cb)
    {
        socket.register_nb_transport_bw(this, &DMA::nb_transport_bw);
        SC_THREAD(state_register);
        sensitive << clk.pos();
        SC_METHOD(dma_function);
        sensitive << state << isram_done << wsram_done;

        last_source = 0xffffffff;
    }

    // from host
    sc_in<bool> clk;
    sc_in<bool> rst;
    sc_in<bool> read_in;
    sc_in<bool> write_back;
    sc_in<sc_uint<32> > source_address;
    sc_in<sc_uint<32> > target_address;
    sc_in<sc_uint<32> > data_length;
    sc_in<bool> sram_mode; // 0-> 6  ;  1 -> 8

    // to accelerator
    sc_out<bool> done;
    sc_out<bool> busy;

    // from input SRAM
    //sc_in<bool> data_vaild;
    sc_in<bool> isram_done;
    sc_in<bool> wsram_done;
    
    //to input SRAM
    sc_out<bool> input_SRAM_write;
    sc_out<sc_uint<32> > to_SRAM_length; // may not use
    sc_out<sc_uint<32> > to_SRAM_data[7];
    sc_out<bool> to_SRAM_mode;

    //to Weight SRAM
    sc_out<bool> weight_SRAM_write;
    sc_out<sc_uint<32> > weight_data[9];


    // internel reg
    sc_signal<sc_uint<32> > state;
    
    sc_uint<32> source;
    sc_uint<32> target;
    sc_signal<sc_uint<32> > new_target_address;
    sc_signal<sc_uint<32> > length;
    sc_signal<sc_uint<32> > len_counter;
    sc_signal<sc_uint<32> > last_source;

    bool dram_done = 0;
    uint32_t data_buffer[9]; 

    mm   m_mm;
    
    void state_register();
    void dma_function();
    void transport(bool rw,uint32_t addr, uint32_t* data, uint32_t length);
    
    virtual tlm::tlm_sync_enum nb_transport_bw( 
    tlm::tlm_generic_payload& trans,
    tlm::tlm_phase& phase, sc_time& delay );

    void peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase);

    tlm::tlm_generic_payload* request_in_progress;
    sc_event end_request_event;
    tlm_utils::peq_with_cb_and_phase<DMA> m_peq;
    int k=0;
};

#endif
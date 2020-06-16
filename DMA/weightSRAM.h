#ifndef WEIGHTSRAM_H_
#define WEIGHTSRAM_H_

#include "ps_config.h"
#include <systemc.h> 

SC_MODULE(weightSRAM){

    SC_CTOR(weightSRAM)
    {
        SC_THREAD(sram);
        sensitive << clk.pos();   
    }

    sc_in<bool> clk;
    sc_in<bool> rst;

    // from host
    sc_in<bool> pe_read;

    // from DMA
    sc_in<bool> dma_write;
    sc_in<bool> mode;
    sc_in<sc_uint<32> > data[9];
    sc_in<sc_uint<32> > length; 

    // to PE
    sc_out<sc_uint<32> > data_to_pe[WEIGHT_SRAM_BANK][3];
    sc_out<bool> vaild;

    //to DMA 
    sc_out<bool> done;

    // Queue
    uint32_t data_f;
    uint32_t data_b;
    sc_signal<bool> sram_mode;
    
    // sram
    uint32_t data_sram[WEIGHT_SRAM_BANK][3][18];
    
    void sram();
    
};

#endif
#ifndef INPUTSRAM_H_
#define INPUTSRAM_H_

#include "ps_config.h"
#include <systemc.h> 

SC_MODULE(inputSRAM){

    SC_CTOR(inputSRAM)
    {
        SC_THREAD(sram);
        sensitive << clk.pos();   
    }

    sc_in<bool> clk;
    sc_in<bool> rst;
    
    // from host
    sc_in<bool> pe_read;
    sc_in<bool> col_select;

    // from DMA
    sc_in<bool> dma_write;
    sc_in<bool> mode;
    sc_in<sc_uint<32> > data[7];
    sc_in<sc_uint<32> > length; 

    // to PE
    sc_out<sc_uint<32> > data_to_pe[INPUT_SRAM_BANK][7];
    sc_out<bool> vaild;

    //to DMA 
    sc_out<bool> done;

    // Queue
    uint32_t data_f;
    uint32_t data_b;
    sc_signal<bool> sram_mode;
    
    // sram
    uint32_t data_sram[INPUT_SRAM_BANK][7][120];
    
    void sram();
   

    
};

#endif



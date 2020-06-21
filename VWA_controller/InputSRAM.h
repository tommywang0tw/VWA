#ifndef INPUTSRAM_H_
#define INPUTSRAM_H_

#include "ps_config.h"
#include <systemc.h> 

SC_MODULE(InputSRAM){

    SC_CTOR(InputSRAM)
    {
    // data_sram = new int ** [384];
    // for (int i = 0;i < 384;i++)
    // {
    //     data_sram[i] = new int*[7];
    //     for (int j = 0;j < 7;j++)
    //     {
    //         data_sram[i][j] = new int[8];
    //     }
    // }
        SC_THREAD(sram);
        sensitive << clk.pos();   
    }

    sc_in<bool> clk;
    sc_in<bool> rst;
    
    // from host
    sc_in<bool> pe_read;
    sc_in<sc_uint<32> > col_select;

    // from DMA
    sc_in<bool> dma_write;
    sc_in<bool> mode;
    sc_in<sc_int<32> > data[7];
    sc_in<sc_uint<32> > length; 

    // to PE
    sc_out<sc_int<32> > data_to_pe[INPUT_SRAM_BANK][7];
    sc_out<bool> vaild;

    //to DMA 
    sc_out<bool> done;

    // Queue
    //uint32_t data_f;
    sc_uint<32> data_f;
    //uint32_t data_b;
    sc_uint<32> data_b;
    sc_signal<bool> sram_mode;
    
    // sram
    sc_int<32> data_sram[INPUT_SRAM_BANK][7][200];
    
    void sram();
   

    
};

#endif



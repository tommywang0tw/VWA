#ifndef OUTPUTSRAM_H
#define OUTPUTSRAM_H

#include "ps_config.h"
#include <string.h>
SC_MODULE(outputSRAM){

    SC_CTOR(outputSRAM)
    {
        SC_THREAD(sram);
        sensitive << clk.pos();   
    }

    sc_in<bool> clk;
    sc_in<bool> rst;

    sc_in<bool> top;
    sc_in<bool> vaild;
    SC_in<sc_uint<32> > in_data[7];
    sc_in<sc_uint<32> > row;
    sc_in<sc_uint<32> > col;  

    void sram();
    uint32_t output_SRAM[412][412][1024];

    uint32_t row_point;
    uint32_t col_point;
    uint32_t cha_point;
}

#endif
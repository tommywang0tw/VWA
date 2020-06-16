//PE.h

#ifndef PE_H
#define PE_H

#include "systemc.h"

SC_MODULE(PE) {

/* ============================================================
            Input Ports
    ============================================================
*/
    sc_in<sc_int<32> > input[7]; //from 1 bank of Input SRAM
    sc_in<sc_int<32> > weight[3]; //from 1 bank of Weight SRAM
    sc_in<bool> input_ctrl[7]; //ctrl signal for choosing the input should come from input SRAM or 0
/* ============================================================
            Output Ports
    ============================================================
*/
    sc_out<sc_int<32> > buffer[9]; //9 buffer to store partial sum

    void do_PE();
    
    SC_CTOR(PE){        
        SC_METHOD(do_PE);
        for (int i = 0; i < 7; i++){
          sensitive << input[i];
        }
        for (int j = 0; j < 3; j++){
          sensitive << weight[j];
        }       
        for(int k = 0; k < 7; k++) {
          sensitive << input_ctrl[k];
        }
    }
};

#endif
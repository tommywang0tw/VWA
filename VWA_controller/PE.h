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

/* ============================================================
            Output Ports
    ============================================================
*/
    sc_out<sc_int<32> > buffer[9]; //9 buffer to store partial sum

    void do_PE();
    
    SC_CTOR(PE){
        int i, j;
        
        SC_METHOD(do_PE);
        for (i = 0; i < 7; i++){
          sensitive << input[i];
        }
        for (j = 0; j < 3; j++){
          sensitive << weight[j];
        }       
    }
};

#endif
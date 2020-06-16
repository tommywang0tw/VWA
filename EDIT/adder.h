//ADDER.h

#ifndef ADDER_H
#define ADDER_H

#include "systemc.h"

SC_MODULE(ADDER) {

/* ============================================================
            Input Ports
    ============================================================
*/
    sc_in<sc_uint<32> > input[9]; 

/* ============================================================
            Output Ports
    ============================================================
*/
    sc_out<sc_uint<32> > out_data; 

    void do_ADD();
    
    SC_CTOR(ADDER){
        SC_METHOD(do_ADD);
        for (int i = 0; i < 9; i++){
          sensitive << input[i];
        }
    }
};

#endif
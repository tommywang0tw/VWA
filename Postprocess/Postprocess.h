
#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "systemc.h"

SC_MODULE(Postprocess) {


    sc_in<sc_uint<32> > input[7]; 
    sc_in<sc_uint<32> > bias; 

/* ============================================================
            Output Ports
    ============================================================
*/
    sc_out<sc_uint<32> > output;

    void run_Postprocess();
    
    SC_CTOR(Postprocess){
        
        SC_METHOD(run_Postprocess);
        for (int i = 0; i < 7; i++){
            sensitive << input[i];
        }
        sensitive << bias;
    }
};

#endif
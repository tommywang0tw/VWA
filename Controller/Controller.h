
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "systemc.h"

SC_MODULE(Controller) {


    sc_in<bool> clk;
    sc_in<bool> rst; 

/* ============================================================
            Output Ports
    ============================================================
*/
    sc_out<sc_uint<32> > signal_stage1;

    void run_Controller();
    
    SC_CTOR(Controller){
        SC_CTHREAD(run_Controller,clk.pos());
        reset_signal_is(rst, false);        
    }
};

#endif
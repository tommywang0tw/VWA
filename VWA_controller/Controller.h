//Controller.h

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "systemc.h"

SC_MODULE(CONTROLLER) {

/*  ============================================================
            Input Ports: clock and control signals 
    ============================================================
*/
	sc_in<bool> clk;			// clock
	sc_in<bool> rst;			// reset

/*  ============================================================
            Input Ports
    ============================================================
*/
    sc_in<bool> start;
    sc_in<sc_uint<32> > input_ch;       //num of input channel
    sc_in<sc_uint<32> > f_size;         //filter size

/*  ============================================================
            Output Ports
    ============================================================
*/
    sc_out<bool> write_to_pe;           //tell InputSRAM move data to PE
    sc_out<sc_uint<32> > out_input_ch;  //output num of input channel to InputSRAM
    sc_out<sc_uint<32> > out_f_size;    //output filter size to WeightSRAM
    sc_out<sc_uint<32> > input_col;    //column of input
    sc_out<sc_uint<32> > weight_col;    //column of weight
    sc_out<sc_uint<32> > stage1_ctrl; //stage 1 control signal
    sc_out<sc_uint<32> > stage3_ctrl1, stage3_ctrl2; //stage 3 control signal
    sc_out<bool> stage1_rst, stage3_rst;
/*  ============================================================
            Inner Data
    ============================================================
*/
    sc_uint<32> cycle_count; //current cycle 


    void do_Controller();
    
    SC_CTOR(CONTROLLER){
        SC_CTHREAD(do_Controller,clk.pos());
        reset_signal_is(rst, false);        
    }
};

#endif

#ifndef ACCUMULATOR_FIRST_H
#define ACCUMULATOR_FIRST_H

#include "systemc.h"

#define BUFFER_NUM 9
#define SH_REG_NUM 4

SC_MODULE(ACCUMULATOR_FIRST) {

/* Input Ports  */
    sc_in<sc_uint<32> > input[BUFFER_NUM]; //from 1 bank of Input SRAM
    sc_in<sc_uint<32> > ctrl_stage1;
    sc_in<sc_uint<32> > sr_index;
    sc_in<bool> clk;
    sc_in<bool> rst;

/* Registers  */
    //sc_signal<sc_uint<32> > input_regs[BUFFER_NUM];
    sc_signal<sc_uint<32> > output_regs[BUFFER_NUM];
    sc_signal<sc_uint<32> > sr_index_reg;
    sc_signal<sc_uint<32> > ctrl_reg;


/* Shift Registers  */
    sc_signal<sc_uint<32> > shift_regs[BUFFER_NUM][SH_REG_NUM];

/* Output Ports   */
    sc_out<sc_uint<32> > output[BUFFER_NUM]; //9 buffer to store partial sum

/* internal variables */
    int tail; // tail index for shift register

    void run_Accumulator();
    
    SC_CTOR(ACCUMULATOR_FIRST){
      SC_CTHREAD(run_Accumulator, clk.pos());
      reset_signal_is(rst, false);
    }
};

#endif
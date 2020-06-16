#ifndef ACCUMULATOR_THIRD_H
#define ACCUMULATOR_THIRD_H

#include "systemc.h"

#define BUFFER_NUM 9
#define T_SH_REG_NUM 5

SC_MODULE(ACCUMULATOR_THIRD) {

/* Input Ports  */
    sc_in<sc_uint<32> > input_T[BUFFER_NUM]; //Input from tree adder
    sc_in<sc_uint<32> > input_S[2]; //Input from Boundary SRAM
    sc_in<sc_uint<32> > ctrl_1, ctrl_2; //Two control signal for two muxes
    sc_in<bool> clk;
    sc_in<bool> rst;

/* Registers  */
    //sc_signal<sc_uint<32> > input_regs[BUFFER_NUM];  //Input register
    sc_signal<sc_uint<32> > ctrl1_reg;
    sc_signal<sc_uint<32> > ctrl2_reg;
    sc_signal<sc_uint<32> > output_index_reg;
/* Shift Registers  */
    sc_signal<sc_uint<32> > shift_regs[BUFFER_NUM][T_SH_REG_NUM];

/* Output Ports   */
    sc_out<sc_uint<32> > output_P[BUFFER_NUM-2]; // output to post process
    sc_out<sc_uint<32> > output_S[2]; // output to boundary SRAM

    void run_Accumulator();
    
    SC_CTOR(ACCUMULATOR_THIRD){ 
      SC_CTHREAD(run_Accumulator, clk.pos());
      reset_signal_is(rst, false);
    }
};

#endif
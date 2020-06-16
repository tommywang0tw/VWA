#include "Accumulator_Third.h"
#include "systemc.h"

int sc_main(int argc, char *argv[]){

    sc_time clkPrd(10, SC_NS);
    sc_signal<bool> rst;
    sc_clock clk("clock", clkPrd, 0.50);

    sc_signal<sc_uint<32> > input_T[BUFFER_NUM];
    sc_signal<sc_uint<32> > input_S[2];
    sc_signal<sc_uint<32> > ctrl_1, ctrl_2;
    sc_signal<sc_uint<32> > output_P[BUFFER_NUM-2];
    sc_signal<sc_uint<32> > output_S[2];

    ACCUMULATOR_THIRD Accumulator_Third("Accumulator_Third");

    for(int i = 0; i < BUFFER_NUM; i++) {
        Accumulator_Third.input_T[i](input_T[i]);
        if(i<7)
            Accumulator_Third.output_P[i](output_P[i]);
    }
    Accumulator_Third.input_S[0](input_S[0]);
    Accumulator_Third.input_S[1](input_S[1]);
    Accumulator_Third.output_S[0](output_S[0]);
    Accumulator_Third.output_S[1](output_S[1]);
    Accumulator_Third.ctrl_1(ctrl_1);
    Accumulator_Third.ctrl_2(ctrl_2);
    Accumulator_Third.clk(clk);
    Accumulator_Third.rst(rst);

    sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");

    //trace input signal
    sc_trace(tf, Accumulator_Third.input_T[0] ,"input[0]");
    sc_trace(tf, Accumulator_Third.input_T[1] ,"input[1]");
    sc_trace(tf, Accumulator_Third.input_T[2] ,"input[2]");
    sc_trace(tf, Accumulator_Third.input_T[3] ,"input[3]");
    sc_trace(tf, Accumulator_Third.input_T[4] ,"input[4]");
    sc_trace(tf, Accumulator_Third.input_T[5] ,"input[5]");
    sc_trace(tf, Accumulator_Third.input_T[6] ,"input[6]");
    sc_trace(tf, Accumulator_Third.input_T[7] ,"input[7]");
    sc_trace(tf, Accumulator_Third.input_T[8] ,"input[8]");
    
    //trace output signal
    sc_trace(tf, Accumulator_Third.output_P[0] ,"output[0]");
    sc_trace(tf, Accumulator_Third.output_P[1] ,"output[1]");
    sc_trace(tf, Accumulator_Third.output_P[2] ,"output[2]");
    sc_trace(tf, Accumulator_Third.output_P[3] ,"output[3]");
    sc_trace(tf, Accumulator_Third.output_P[4] ,"output[4]");
    sc_trace(tf, Accumulator_Third.output_P[5] ,"output[5]");
    sc_trace(tf, Accumulator_Third.output_P[6] ,"output[6]");

    //trace ctrl, rst signal
    sc_trace(tf, Accumulator_Third.rst, "rst");
    sc_trace(tf, Accumulator_Third.ctrl_1, "ctrl1");
    sc_trace(tf, Accumulator_Third.ctrl_2, "ctrl2");

    //trace shift registers
    sc_trace(tf, Accumulator_Third.shift_regs[0][0], "shift_regs(0)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[1][0], "shift_regs(1)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][0], "shift_regs(2)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[3][0], "shift_regs(3)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[4][0], "shift_regs(4)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[5][0], "shift_regs(5)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[6][0], "shift_regs(6)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[7][0], "shift_regs(7)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[8][0], "shift_regs(8)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[0][1], "shift_regs(0)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[1][1], "shift_regs(1)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][1], "shift_regs(2)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[3][1], "shift_regs(3)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[4][1], "shift_regs(4)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[5][1], "shift_regs(5)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[6][1], "shift_regs(6)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[7][1], "shift_regs(7)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[8][1], "shift_regs(8)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[0][2], "shift_regs(0)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[1][2], "shift_regs(1)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][2], "shift_regs(2)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[3][2], "shift_regs(3)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[4][2], "shift_regs(4)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[5][2], "shift_regs(5)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[6][2], "shift_regs(6)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[7][2], "shift_regs(7)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[8][2], "shift_regs(8)(2)");
    


    rst.write(false);
    sc_start(10,SC_NS);
    rst.write(true);
    ctrl_1.write(0);
    ctrl_2.write(0);
    for(int i=0; i<9; i++) {
        input_T[i].write(1);
    }
    input_S[0].write(1);
    input_S[1].write(2);

    sc_start(20,SC_NS);
    ctrl_1.write(1);
    for(int i=0; i<9; i++) {
        input_T[i].write(2);
    }

    sc_start(100,SC_NS);

    sc_close_vcd_trace_file(tf); 
    return (0);
}
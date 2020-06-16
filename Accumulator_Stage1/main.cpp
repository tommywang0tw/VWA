#include "Accumulator_First.h"
#include "systemc.h"

int sc_main(int argc, char *argv[]){

    sc_time clkPrd(10, SC_NS);
    sc_signal<bool> rst;
    sc_clock clk("clock", clkPrd, 0.50);

    sc_signal<sc_uint<32> > input[BUFFER_NUM];
    sc_signal<sc_uint<32> > ctrl;
    sc_signal<sc_uint<32> > output[BUFFER_NUM];

    ACCUMULATOR_FIRST Accumulator_First("Accumulator_First");

    for(int i = 0; i < BUFFER_NUM; i++) {
        Accumulator_First.input[i](input[i]);
        Accumulator_First.output[i](output[i]);
    }
    Accumulator_First.ctrl(ctrl);
    Accumulator_First.clk(clk);
    Accumulator_First.rst(rst);

    sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");

    //trace input signal
    sc_trace(tf, Accumulator_First.input[0] ,"input[0]");
    sc_trace(tf, Accumulator_First.input[1] ,"input[1]");
    sc_trace(tf, Accumulator_First.input[2] ,"input[2]");
    sc_trace(tf, Accumulator_First.input[3] ,"input[3]");
    sc_trace(tf, Accumulator_First.input[4] ,"input[4]");
    sc_trace(tf, Accumulator_First.input[5] ,"input[5]");
    sc_trace(tf, Accumulator_First.input[6] ,"input[6]");
    sc_trace(tf, Accumulator_First.input[7] ,"input[7]");
    sc_trace(tf, Accumulator_First.input[8] ,"input[8]");
    
    //trace output signal
    sc_trace(tf, Accumulator_First.output[0] ,"output[0]");
    sc_trace(tf, Accumulator_First.output[1] ,"output[1]");
    sc_trace(tf, Accumulator_First.output[2] ,"output[2]");
    sc_trace(tf, Accumulator_First.output[3] ,"output[3]");
    sc_trace(tf, Accumulator_First.output[4] ,"output[4]");
    sc_trace(tf, Accumulator_First.output[5] ,"output[5]");
    sc_trace(tf, Accumulator_First.output[6] ,"output[6]");
    sc_trace(tf, Accumulator_First.output[7] ,"output[7]");
    sc_trace(tf, Accumulator_First.output[8] ,"output[8]");

    //trace input register
    sc_trace(tf, Accumulator_First.input_regs[0], "input_regs(0)");
    sc_trace(tf, Accumulator_First.input_regs[1], "input_regs(1)");
    sc_trace(tf, Accumulator_First.input_regs[2], "input_regs(2)");
    sc_trace(tf, Accumulator_First.input_regs[3], "input_regs(3)");
    sc_trace(tf, Accumulator_First.input_regs[4], "input_regs(4)");
    sc_trace(tf, Accumulator_First.input_regs[5], "input_regs(5)");
    sc_trace(tf, Accumulator_First.input_regs[6], "input_regs(6)");
    sc_trace(tf, Accumulator_First.input_regs[7], "input_regs(7)");
    sc_trace(tf, Accumulator_First.input_regs[8], "input_regs(8)");

    //trace output register
    sc_trace(tf, Accumulator_First.output_regs[0], "output_regs(0)");
    sc_trace(tf, Accumulator_First.output_regs[1], "output_regs(1)");
    sc_trace(tf, Accumulator_First.output_regs[2], "output_regs(2)");
    sc_trace(tf, Accumulator_First.output_regs[3], "output_regs(3)");
    sc_trace(tf, Accumulator_First.output_regs[4], "output_regs(4)");
    sc_trace(tf, Accumulator_First.output_regs[5], "output_regs(5)");
    sc_trace(tf, Accumulator_First.output_regs[6], "output_regs(6)");
    sc_trace(tf, Accumulator_First.output_regs[7], "output_regs(7)");
    sc_trace(tf, Accumulator_First.output_regs[8], "output_regs(8)");

    //trace ctrl, rst signal
    sc_trace(tf, Accumulator_First.rst, "rst");
    sc_trace(tf, Accumulator_First.ctrl, "ctrl");

    //trace shift registers
    sc_trace(tf, Accumulator_First.shift_regs[0][0], "shift_regs(0)(0)");
    sc_trace(tf, Accumulator_First.shift_regs[1][0], "shift_regs(1)(0)");
    sc_trace(tf, Accumulator_First.shift_regs[2][0], "shift_regs(2)(0)");
    sc_trace(tf, Accumulator_First.shift_regs[3][0], "shift_regs(3)(0)");
    sc_trace(tf, Accumulator_First.shift_regs[4][0], "shift_regs(4)(0)");
    sc_trace(tf, Accumulator_First.shift_regs[5][0], "shift_regs(5)(0)");
    sc_trace(tf, Accumulator_First.shift_regs[6][0], "shift_regs(6)(0)");
    sc_trace(tf, Accumulator_First.shift_regs[7][0], "shift_regs(7)(0)");
    sc_trace(tf, Accumulator_First.shift_regs[8][0], "shift_regs(8)(0)");
    sc_trace(tf, Accumulator_First.shift_regs[0][1], "shift_regs(0)(1)");
    sc_trace(tf, Accumulator_First.shift_regs[1][1], "shift_regs(1)(1)");
    sc_trace(tf, Accumulator_First.shift_regs[2][1], "shift_regs(2)(1)");
    sc_trace(tf, Accumulator_First.shift_regs[3][1], "shift_regs(3)(1)");
    sc_trace(tf, Accumulator_First.shift_regs[4][1], "shift_regs(4)(1)");
    sc_trace(tf, Accumulator_First.shift_regs[5][1], "shift_regs(5)(1)");
    sc_trace(tf, Accumulator_First.shift_regs[6][1], "shift_regs(6)(1)");
    sc_trace(tf, Accumulator_First.shift_regs[7][1], "shift_regs(7)(1)");
    sc_trace(tf, Accumulator_First.shift_regs[8][1], "shift_regs(8)(1)");
    sc_trace(tf, Accumulator_First.shift_regs[0][2], "shift_regs(0)(2)");
    sc_trace(tf, Accumulator_First.shift_regs[1][2], "shift_regs(1)(2)");
    sc_trace(tf, Accumulator_First.shift_regs[2][2], "shift_regs(2)(2)");
    sc_trace(tf, Accumulator_First.shift_regs[3][2], "shift_regs(3)(2)");
    sc_trace(tf, Accumulator_First.shift_regs[4][2], "shift_regs(4)(2)");
    sc_trace(tf, Accumulator_First.shift_regs[5][2], "shift_regs(5)(2)");
    sc_trace(tf, Accumulator_First.shift_regs[6][2], "shift_regs(6)(2)");
    sc_trace(tf, Accumulator_First.shift_regs[7][2], "shift_regs(7)(2)");
    sc_trace(tf, Accumulator_First.shift_regs[8][2], "shift_regs(8)(2)");
    
    

    rst.write(false);
    sc_start(10,SC_NS);
    rst.write(true);
    for(int i=0; i<9; i++) {
        input[i].write(1);
        ctrl.write(0);
    }
    sc_start(10,SC_NS);
    for(int i=0; i<9; i++) {
        input[i].write(2);
        //ctrl.write(0);
    }
    sc_start(10,SC_NS);
    for(int i=0; i<9; i++) {
        input[i].write(3);
        //ctrl.write(1);
    }

    sc_start(100,SC_NS);

    sc_close_vcd_trace_file(tf); 
    return (0);
}
#include "PE.h"
#include "Accumulator_First.h"
#include "systemc.h"

int sc_main(int argc, char *argv[]){

    int input_matrix[7][6];
    for (int i=0; i<7; i++) {
        for(int j=0; j<6; j++) {
            input_matrix[i][j] = 6*i + j + 1;
            std::cout << input_matrix[i][j] << endl;
        }
    }

    int filter[3][3];
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            filter[i][j] = 3*i + j + 1;
        }
    }

    sc_signal<sc_uint<32> > input[7], weight[3], buffer[9];
    sc_time clkPrd(10, SC_NS);
    sc_signal<bool> rst;
    sc_clock clk("clock", clkPrd, 0.50);

    sc_signal<sc_uint<32> > ctrl;
    sc_signal<sc_uint<32> > output[BUFFER_NUM];
    sc_signal<sc_uint<32> > sr_index;

     PE pe("PE");
    ACCUMULATOR_FIRST Accumulator_First("ACCUMULATOR_FIRST");

    //bind ports
    for(int i = 0; i < 7; i++)
        pe.input[i](input[i]);
    for(int i = 0; i < 3; i++)
        pe.weight[i](weight[i]);
    for(int i = 0; i < 9; i++)
         pe.buffer[i](buffer[i]);

    for(int i = 0; i < BUFFER_NUM; i++) {
        Accumulator_First.input[i](buffer[i]);
        Accumulator_First.output[i](output[i]);
    }
    Accumulator_First.ctrl(ctrl);
    Accumulator_First.clk(clk);
    Accumulator_First.rst(rst);
    Accumulator_First.sr_index(sr_index);

    sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");

    //trace input signal
    sc_trace(tf, pe.input[0] ,"input[0]");
    sc_trace(tf, pe.input[1] ,"input[1]");
    sc_trace(tf, pe.input[2] ,"input[2]");
    sc_trace(tf, pe.input[3] ,"input[3]");
    sc_trace(tf, pe.input[4] ,"input[4]");
    sc_trace(tf, pe.input[5] ,"input[5]");
    sc_trace(tf, pe.input[6] ,"input[6]");
    
    //trace weight signal
    sc_trace(tf, pe.weight[0] ,"weight[0]");
    sc_trace(tf, pe.weight[1] ,"weight[1]");
    sc_trace(tf, pe.weight[2] ,"weight[2]");

    //trace output signal
    sc_trace(tf, pe.buffer[0] ,"buffer[0]");
    sc_trace(tf, pe.buffer[1] ,"buffer[1]");
    sc_trace(tf, pe.buffer[2] ,"buffer[2]");
    sc_trace(tf, pe.buffer[3] ,"buffer[3]");
    sc_trace(tf, pe.buffer[4] ,"buffer[4]");
    sc_trace(tf, pe.buffer[5] ,"buffer[5]");
    sc_trace(tf, pe.buffer[6] ,"buffer[6]");
    sc_trace(tf, pe.buffer[7] ,"buffer[7]");
    sc_trace(tf, pe.buffer[8] ,"buffer[8]");
    
//trace input signal
    sc_trace(tf, Accumulator_First.input[0] ,"A_input[0]");
    sc_trace(tf, Accumulator_First.input[1] ,"A_input[1]");
    sc_trace(tf, Accumulator_First.input[2] ,"A_input[2]");
    sc_trace(tf, Accumulator_First.input[3] ,"A_input[3]");
    sc_trace(tf, Accumulator_First.input[4] ,"A_input[4]");
    sc_trace(tf, Accumulator_First.input[5] ,"A_input[5]");
    sc_trace(tf, Accumulator_First.input[6] ,"A_input[6]");
    sc_trace(tf, Accumulator_First.input[7] ,"A_input[7]");
    sc_trace(tf, Accumulator_First.input[8] ,"A_input[8]");

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

    sc_trace(tf, Accumulator_First.sr_index, "sr_index");
    sc_trace(tf, Accumulator_First.sr_index_reg, "sr_index_reg");



    rst.write(false);
    ctrl.write(0);
    sc_start(10,SC_NS);
    rst.write(true);

    //cycle 1:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][0]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][0]);
    sr_index.write(0);
    ctrl.write(0);
    sc_start(10,SC_NS);

    // for(int i=0; i<9; i++) {
    //     buffer[i].write(1);
    //     ctrl.write(0);
    // }

    //cycle 2:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][1]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][0]);
    sr_index.write(1);
    ctrl.write(1);
    sc_start(10,SC_NS);



    //cycle 3:
    ctrl.write(1);
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][1]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][1]);
    sr_index.write(0);
    ctrl.write(0);
    sc_start(10,SC_NS);

    //cycle 4:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][2]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][0]);
    sr_index.write(2);
    ctrl.write(2);
    sc_start(10,SC_NS);

    //cycle 5:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][2]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][1]);
    sr_index.write(1);
    ctrl.write(1);
    sc_start(10,SC_NS);

    //cycle 6:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][2]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][2]);
    sr_index.write(0);
    ctrl.write(0);
    sc_start(10,SC_NS);

    //cycle 7:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][3]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][0]);
    sr_index.write(3);
    ctrl.write(3);
    sc_start(10,SC_NS);


    //cycle 8:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][3]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][1]);
    sr_index.write(2);
    ctrl.write(2);
    sc_start(10,SC_NS);

    //cycle 9:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][3]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][2]);
    sr_index.write(1);
    ctrl.write(1);
    sc_start(10,SC_NS);

    //cycle 10:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][4]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][1]);
    sr_index.write(3);
    ctrl.write(3);
    sc_start(10,SC_NS);

    //cycle 11:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][4]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][2]);
    sr_index.write(2);
    ctrl.write(2);
    sc_start(10,SC_NS);

    //cycle 12:
    //given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(input_matrix[i][5]);
    for(int i = 0; i < 3; i++)
        weight[i].write(filter[i][2]);
    sr_index.write(3);
    ctrl.write(3);
    sc_start(10,SC_NS);



    
    sc_start(200,SC_NS);

    sc_close_vcd_trace_file(tf); 
    return (0);
}
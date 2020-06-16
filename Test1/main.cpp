#include "PE.h"
#include "Accumulator_First.h"
#include "TreeAdder.h"
#include "Accumulator_Third.h"
#include "systemc.h"

int sc_main(int argc, char *argv[])
{

    int input_matrix[16][7][6];
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 6; j++)
            {

                input_matrix[k][i][j] = 6 * i + j + 1 + k;
            }
            //input_matrix[i][j] = 6*i + j + 1;
            //std::cout << input_matrix[i][j] << endl;
        }
    }

    for (int k = 0; k <8; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 6; j++)
            {

                input_matrix[k+8][i][j] = 6 * i + j + 1 + k;
            }
            //input_matrix[i][j] = 6*i + j + 1;
            //std::cout << input_matrix[i][j] << endl;
        }
    }

    int filter[16][3][3];
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {

                filter[k][i][j] = 3 * i + j + 1 + k;
            }
        }
    }

    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {

                filter[k+8][i][j] = 3 * i + j + 1 + k;
            }
        }
    }

    sc_signal<sc_uint<32>> input[8][7], weight[8][3], buffer[8][9];
    sc_time clkPrd(10, SC_NS);
    sc_signal<bool> stage1_rst, stage3_rst;
    sc_clock clk("clock", clkPrd, 0.50);

    sc_signal<sc_uint<32>> ctrl; //ctrl for stage1
    sc_signal<sc_uint<32> > ctrl_1, ctrl_2; //Two control signal for two muxes
    sc_signal<sc_uint<32>> output[8][BUFFER_NUM];
    sc_signal<sc_uint<32>> t_output[BUFFER_NUM];
    sc_signal<sc_uint<32>> s_input[2]; //boundary SRAM
    sc_signal<sc_uint<32> > output_P[BUFFER_NUM-2]; // output to post process
    sc_signal<sc_uint<32> > output_S[2]; // output to boundary SRAM 

    PE pe1("PE1");
    PE pe2("PE2");
    PE pe3("PE3");
    PE pe4("PE4");
    PE pe5("PE5");
    PE pe6("PE6");
    PE pe7("PE7");
    PE pe8("PE8");

    ACCUMULATOR_FIRST Accumulator_First1("ACCUMULATOR_FIRST1");
    ACCUMULATOR_FIRST Accumulator_First2("ACCUMULATOR_FIRST2");
    ACCUMULATOR_FIRST Accumulator_First3("ACCUMULATOR_FIRST3");
    ACCUMULATOR_FIRST Accumulator_First4("ACCUMULATOR_FIRST4");
    ACCUMULATOR_FIRST Accumulator_First5("ACCUMULATOR_FIRST5");
    ACCUMULATOR_FIRST Accumulator_First6("ACCUMULATOR_FIRST6");
    ACCUMULATOR_FIRST Accumulator_First7("ACCUMULATOR_FIRST7");
    ACCUMULATOR_FIRST Accumulator_First8("ACCUMULATOR_FIRST8");

    TreeAdder treeAdder("treeAdder");

    ACCUMULATOR_THIRD Accumulator_Third("ACCUMULATOR_THIRD");

    //bind ports
    for (int i = 0; i < 7; i++)
    {
        pe1.input[i](input[0][i]);
        pe2.input[i](input[1][i]);
        pe3.input[i](input[2][i]);
        pe4.input[i](input[3][i]);
        pe5.input[i](input[4][i]);
        pe6.input[i](input[5][i]);
        pe7.input[i](input[6][i]);
        pe8.input[i](input[7][i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pe1.weight[i](weight[0][i]);
        pe2.weight[i](weight[1][i]);
        pe3.weight[i](weight[2][i]);
        pe4.weight[i](weight[3][i]);
        pe5.weight[i](weight[4][i]);
        pe6.weight[i](weight[5][i]);
        pe7.weight[i](weight[6][i]);
        pe8.weight[i](weight[7][i]);
    }

    for (int i = 0; i < 9; i++)
    {
        pe1.buffer[i](buffer[0][i]);
        pe2.buffer[i](buffer[1][i]);
        pe3.buffer[i](buffer[2][i]);
        pe4.buffer[i](buffer[3][i]);
        pe5.buffer[i](buffer[4][i]);
        pe6.buffer[i](buffer[5][i]);
        pe7.buffer[i](buffer[6][i]);
        pe8.buffer[i](buffer[7][i]);
    }

    for (int i = 0; i < BUFFER_NUM; i++)
    {
        Accumulator_First1.input[i](buffer[0][i]);
        Accumulator_First1.output[i](output[0][i]);
        Accumulator_First2.input[i](buffer[1][i]);
        Accumulator_First2.output[i](output[1][i]);
        Accumulator_First3.input[i](buffer[2][i]);
        Accumulator_First3.output[i](output[2][i]);
        Accumulator_First4.input[i](buffer[3][i]);
        Accumulator_First4.output[i](output[3][i]);
        Accumulator_First5.input[i](buffer[4][i]);
        Accumulator_First5.output[i](output[4][i]);
        Accumulator_First6.input[i](buffer[5][i]);
        Accumulator_First6.output[i](output[5][i]);
        Accumulator_First7.input[i](buffer[6][i]);
        Accumulator_First7.output[i](output[6][i]);
        Accumulator_First8.input[i](buffer[7][i]);
        Accumulator_First8.output[i](output[7][i]);
    }
    Accumulator_First1.ctrl(ctrl);
    Accumulator_First1.clk(clk);
    Accumulator_First1.rst(stage1_rst);
    Accumulator_First2.ctrl(ctrl);
    Accumulator_First2.clk(clk);
    Accumulator_First2.rst(stage1_rst);
    Accumulator_First3.ctrl(ctrl);
    Accumulator_First3.clk(clk);
    Accumulator_First3.rst(stage1_rst);
    Accumulator_First4.ctrl(ctrl);
    Accumulator_First4.clk(clk);
    Accumulator_First4.rst(stage1_rst);
    Accumulator_First5.ctrl(ctrl);
    Accumulator_First5.clk(clk);
    Accumulator_First5.rst(stage1_rst);
    Accumulator_First6.ctrl(ctrl);
    Accumulator_First6.clk(clk);
    Accumulator_First6.rst(stage1_rst);
    Accumulator_First7.ctrl(ctrl);
    Accumulator_First7.clk(clk);
    Accumulator_First7.rst(stage1_rst);
    Accumulator_First8.ctrl(ctrl);
    Accumulator_First8.clk(clk);
    Accumulator_First8.rst(stage1_rst);

    Accumulator_Third.clk(clk);
    Accumulator_Third.rst(stage3_rst);

    for(int i = 0; i < BUFFER_NUM; i++)
        Accumulator_Third.input_T[i](t_output[i]);
    for(int i = 0; i < 2; i++)
        Accumulator_Third.input_S[i](s_input[i]);

    Accumulator_Third.ctrl_1(ctrl_1);
    Accumulator_Third.ctrl_2(ctrl_2);

    for(int i = 0; i < BUFFER_NUM - 2; i++)
        Accumulator_Third.output_P[i](output_P[i]);
    for(int i = 0; i < 2; i++)
        Accumulator_Third.output_S[i](output_S[i]);

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            treeAdder.input[i][j](output[i][j]);
        }
    }

    for (int i = 0; i < 9; i++)
        treeAdder.output[i](t_output[i]);

    sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");

    //trace input signal
    sc_trace(tf, pe1.input[0], "input[0]");
    sc_trace(tf, pe1.input[1], "input[1]");
    sc_trace(tf, pe1.input[2], "input[2]");
    sc_trace(tf, pe1.input[3], "input[3]");
    sc_trace(tf, pe1.input[4], "input[4]");
    sc_trace(tf, pe1.input[5], "input[5]");
    sc_trace(tf, pe1.input[6], "input[6]");

    //trace weight signal
    sc_trace(tf, pe1.weight[0], "weight[0]");
    sc_trace(tf, pe1.weight[1], "weight[1]");
    sc_trace(tf, pe1.weight[2], "weight[2]");

    //trace output signal
    sc_trace(tf, pe1.buffer[0], "buffer[0]");
    sc_trace(tf, pe1.buffer[1], "buffer[1]");
    sc_trace(tf, pe1.buffer[2], "buffer[2]");
    sc_trace(tf, pe1.buffer[3], "buffer[3]");
    sc_trace(tf, pe1.buffer[4], "buffer[4]");
    sc_trace(tf, pe1.buffer[5], "buffer[5]");
    sc_trace(tf, pe1.buffer[6], "buffer[6]");
    sc_trace(tf, pe1.buffer[7], "buffer[7]");
    sc_trace(tf, pe1.buffer[8], "buffer[8]");

    //trace input signal
    sc_trace(tf, Accumulator_First1.input[0], "A_input[0]");
    sc_trace(tf, Accumulator_First1.input[1], "A_input[1]");
    sc_trace(tf, Accumulator_First1.input[2], "A_input[2]");
    sc_trace(tf, Accumulator_First1.input[3], "A_input[3]");
    sc_trace(tf, Accumulator_First1.input[4], "A_input[4]");
    sc_trace(tf, Accumulator_First1.input[5], "A_input[5]");
    sc_trace(tf, Accumulator_First1.input[6], "A_input[6]");
    sc_trace(tf, Accumulator_First1.input[7], "A_input[7]");
    sc_trace(tf, Accumulator_First1.input[8], "A_input[8]");

    //trace output signal

    sc_trace(tf, Accumulator_First1.output[0], "output1[0]");
    sc_trace(tf, Accumulator_First1.output[1], "output1[1]");
    sc_trace(tf, Accumulator_First1.output[2], "output1[2]");
    sc_trace(tf, Accumulator_First1.output[3], "output1[3]");
    sc_trace(tf, Accumulator_First1.output[4], "output1[4]");
    sc_trace(tf, Accumulator_First1.output[5], "output1[5]");
    sc_trace(tf, Accumulator_First1.output[6], "output1[6]");
    sc_trace(tf, Accumulator_First1.output[7], "output1[7]");
    sc_trace(tf, Accumulator_First1.output[8], "output1[8]");

    sc_trace(tf, Accumulator_First2.output[0], "output2[0]");
    sc_trace(tf, Accumulator_First2.output[1], "output2[1]");
    sc_trace(tf, Accumulator_First2.output[2], "output2[2]");
    sc_trace(tf, Accumulator_First2.output[3], "output2[3]");
    sc_trace(tf, Accumulator_First2.output[4], "output2[4]");
    sc_trace(tf, Accumulator_First2.output[5], "output2[5]");
    sc_trace(tf, Accumulator_First2.output[6], "output2[6]");
    sc_trace(tf, Accumulator_First2.output[7], "output2[7]");
    sc_trace(tf, Accumulator_First2.output[8], "output2[8]");

    sc_trace(tf, Accumulator_First3.output[0], "output3[0]");
    sc_trace(tf, Accumulator_First3.output[1], "output3[1]");
    sc_trace(tf, Accumulator_First3.output[2], "output3[2]");
    sc_trace(tf, Accumulator_First3.output[3], "output3[3]");
    sc_trace(tf, Accumulator_First3.output[4], "output3[4]");
    sc_trace(tf, Accumulator_First3.output[5], "output3[5]");
    sc_trace(tf, Accumulator_First3.output[6], "output3[6]");
    sc_trace(tf, Accumulator_First3.output[7], "output3[7]");
    sc_trace(tf, Accumulator_First3.output[8], "output3[8]");

    sc_trace(tf, Accumulator_First4.output[0], "output4[0]");
    sc_trace(tf, Accumulator_First4.output[1], "output4[1]");
    sc_trace(tf, Accumulator_First4.output[2], "output4[2]");
    sc_trace(tf, Accumulator_First4.output[3], "output4[3]");
    sc_trace(tf, Accumulator_First4.output[4], "output4[4]");
    sc_trace(tf, Accumulator_First4.output[5], "output4[5]");
    sc_trace(tf, Accumulator_First4.output[6], "output4[6]");
    sc_trace(tf, Accumulator_First4.output[7], "output4[7]");
    sc_trace(tf, Accumulator_First4.output[8], "output4[8]");

    sc_trace(tf, Accumulator_First5.output[0], "output5[0]");
    sc_trace(tf, Accumulator_First5.output[1], "output5[1]");
    sc_trace(tf, Accumulator_First5.output[2], "output5[2]");
    sc_trace(tf, Accumulator_First5.output[3], "output5[3]");
    sc_trace(tf, Accumulator_First5.output[4], "output5[4]");
    sc_trace(tf, Accumulator_First5.output[5], "output5[5]");
    sc_trace(tf, Accumulator_First5.output[6], "output5[6]");
    sc_trace(tf, Accumulator_First5.output[7], "output5[7]");
    sc_trace(tf, Accumulator_First5.output[8], "output5[8]");

    sc_trace(tf, Accumulator_First6.output[0], "output6[0]");
    sc_trace(tf, Accumulator_First6.output[1], "output6[1]");
    sc_trace(tf, Accumulator_First6.output[2], "output6[2]");
    sc_trace(tf, Accumulator_First6.output[3], "output6[3]");
    sc_trace(tf, Accumulator_First6.output[4], "output6[4]");
    sc_trace(tf, Accumulator_First6.output[5], "output6[5]");
    sc_trace(tf, Accumulator_First6.output[6], "output6[6]");
    sc_trace(tf, Accumulator_First6.output[7], "output6[7]");
    sc_trace(tf, Accumulator_First6.output[8], "output6[8]");

    sc_trace(tf, Accumulator_First7.output[0], "output7[0]");
    sc_trace(tf, Accumulator_First7.output[1], "output7[1]");
    sc_trace(tf, Accumulator_First7.output[2], "output7[2]");
    sc_trace(tf, Accumulator_First7.output[3], "output7[3]");
    sc_trace(tf, Accumulator_First7.output[4], "output7[4]");
    sc_trace(tf, Accumulator_First7.output[5], "output7[5]");
    sc_trace(tf, Accumulator_First7.output[6], "output7[6]");
    sc_trace(tf, Accumulator_First7.output[7], "output7[7]");
    sc_trace(tf, Accumulator_First7.output[8], "output7[8]");

    sc_trace(tf, Accumulator_First8.output[0], "output8[0]");
    sc_trace(tf, Accumulator_First8.output[1], "output8[1]");
    sc_trace(tf, Accumulator_First8.output[2], "output8[2]");
    sc_trace(tf, Accumulator_First8.output[3], "output8[3]");
    sc_trace(tf, Accumulator_First8.output[4], "output8[4]");
    sc_trace(tf, Accumulator_First8.output[5], "output8[5]");
    sc_trace(tf, Accumulator_First8.output[6], "output8[6]");
    sc_trace(tf, Accumulator_First8.output[7], "output8[7]");
    sc_trace(tf, Accumulator_First8.output[8], "output8[8]");

    //trace output register
    sc_trace(tf, Accumulator_First1.output_regs[0], "output_regs(0)");
    sc_trace(tf, Accumulator_First1.output_regs[1], "output_regs(1)");
    sc_trace(tf, Accumulator_First1.output_regs[2], "output_regs(2)");
    sc_trace(tf, Accumulator_First1.output_regs[3], "output_regs(3)");
    sc_trace(tf, Accumulator_First1.output_regs[4], "output_regs(4)");
    sc_trace(tf, Accumulator_First1.output_regs[5], "output_regs(5)");
    sc_trace(tf, Accumulator_First1.output_regs[6], "output_regs(6)");
    sc_trace(tf, Accumulator_First1.output_regs[7], "output_regs(7)");
    sc_trace(tf, Accumulator_First1.output_regs[8], "output_regs(8)");

    //trace ctrl, rst signal
    sc_trace(tf, Accumulator_First1.rst, "rst");
    sc_trace(tf, Accumulator_First1.ctrl, "ctrl");

    //trace shift registers
    sc_trace(tf, Accumulator_First1.shift_regs[0][0], "shift_regs(0)(0)");
    sc_trace(tf, Accumulator_First1.shift_regs[1][0], "shift_regs(1)(0)");
    sc_trace(tf, Accumulator_First1.shift_regs[2][0], "shift_regs(2)(0)");
    sc_trace(tf, Accumulator_First1.shift_regs[3][0], "shift_regs(3)(0)");
    sc_trace(tf, Accumulator_First1.shift_regs[4][0], "shift_regs(4)(0)");
    sc_trace(tf, Accumulator_First1.shift_regs[5][0], "shift_regs(5)(0)");
    sc_trace(tf, Accumulator_First1.shift_regs[6][0], "shift_regs(6)(0)");
    sc_trace(tf, Accumulator_First1.shift_regs[7][0], "shift_regs(7)(0)");
    sc_trace(tf, Accumulator_First1.shift_regs[8][0], "shift_regs(8)(0)");
    sc_trace(tf, Accumulator_First1.shift_regs[0][1], "shift_regs(0)(1)");
    sc_trace(tf, Accumulator_First1.shift_regs[1][1], "shift_regs(1)(1)");
    sc_trace(tf, Accumulator_First1.shift_regs[2][1], "shift_regs(2)(1)");
    sc_trace(tf, Accumulator_First1.shift_regs[3][1], "shift_regs(3)(1)");
    sc_trace(tf, Accumulator_First1.shift_regs[4][1], "shift_regs(4)(1)");
    sc_trace(tf, Accumulator_First1.shift_regs[5][1], "shift_regs(5)(1)");
    sc_trace(tf, Accumulator_First1.shift_regs[6][1], "shift_regs(6)(1)");
    sc_trace(tf, Accumulator_First1.shift_regs[7][1], "shift_regs(7)(1)");
    sc_trace(tf, Accumulator_First1.shift_regs[8][1], "shift_regs(8)(1)");
    sc_trace(tf, Accumulator_First1.shift_regs[0][2], "shift_regs(0)(2)");
    sc_trace(tf, Accumulator_First1.shift_regs[1][2], "shift_regs(1)(2)");
    sc_trace(tf, Accumulator_First1.shift_regs[2][2], "shift_regs(2)(2)");
    sc_trace(tf, Accumulator_First1.shift_regs[3][2], "shift_regs(3)(2)");
    sc_trace(tf, Accumulator_First1.shift_regs[4][2], "shift_regs(4)(2)");
    sc_trace(tf, Accumulator_First1.shift_regs[5][2], "shift_regs(5)(2)");
    sc_trace(tf, Accumulator_First1.shift_regs[6][2], "shift_regs(6)(2)");
    sc_trace(tf, Accumulator_First1.shift_regs[7][2], "shift_regs(7)(2)");
    sc_trace(tf, Accumulator_First1.shift_regs[8][2], "shift_regs(8)(2)");
    sc_trace(tf, Accumulator_First1.shift_regs[0][3], "shift_regs(0)(3)");
    sc_trace(tf, Accumulator_First1.shift_regs[1][3], "shift_regs(1)(3)");
    sc_trace(tf, Accumulator_First1.shift_regs[2][3], "shift_regs(2)(3)");
    sc_trace(tf, Accumulator_First1.shift_regs[3][3], "shift_regs(3)(3)");
    sc_trace(tf, Accumulator_First1.shift_regs[4][3], "shift_regs(4)(3)");
    sc_trace(tf, Accumulator_First1.shift_regs[5][3], "shift_regs(5)(3)");
    sc_trace(tf, Accumulator_First1.shift_regs[6][3], "shift_regs(6)(3)");
    sc_trace(tf, Accumulator_First1.shift_regs[7][3], "shift_regs(7)(3)");
    sc_trace(tf, Accumulator_First1.shift_regs[8][3], "shift_regs(8)(3)");

    //trace tree adder out
    sc_trace(tf, treeAdder.output[0], "tree_adder(0)");
    sc_trace(tf, treeAdder.output[1], "tree_adder(1)");
    sc_trace(tf, treeAdder.output[2], "tree_adder(2)");
    sc_trace(tf, treeAdder.output[3], "tree_adder(3)");
    sc_trace(tf, treeAdder.output[4], "tree_adder(4)");
    sc_trace(tf, treeAdder.output[5], "tree_adder(5)");
    sc_trace(tf, treeAdder.output[6], "tree_adder(6)");
    sc_trace(tf, treeAdder.output[7], "tree_adder(7)");
    sc_trace(tf, treeAdder.output[8], "tree_adder(8)");

    //trace the 3rd stage Acc. output
    sc_trace(tf, Accumulator_Third.output_P[0], "Accumulator_Third.output(0)");
    sc_trace(tf, Accumulator_Third.output_P[1], "Accumulator_Third.output(1)");
    sc_trace(tf, Accumulator_Third.output_P[2], "Accumulator_Third.output(2)");
    sc_trace(tf, Accumulator_Third.output_P[3], "Accumulator_Third.output(3)");
    sc_trace(tf, Accumulator_Third.output_P[4], "Accumulator_Third.output(4)");
    sc_trace(tf, Accumulator_Third.output_P[5], "Accumulator_Third.output(5)");
    sc_trace(tf, Accumulator_Third.output_P[6], "Accumulator_Third.output(6)");

    //trace the 3rd stage Acc. shift registers
    sc_trace(tf, Accumulator_Third.shift_regs[2][0], "Accumulator_Third.shift_regs(2)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][1], "Accumulator_Third.shift_regs(2)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][2], "Accumulator_Third.shift_regs(2)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][3], "Accumulator_Third.shift_regs(2)(3)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][4], "Accumulator_Third.shift_regs(2)(4)");
    sc_trace(tf, Accumulator_Third.ctrl2_reg, "Accumulator.ctrl2_reg");
    sc_trace(tf, Accumulator_Third.input_T[2], "Accumulator_Third.input_T(2)");

    stage1_rst.write(false);
    stage3_rst.write(false);
    ctrl.write(0);
    ctrl_1.write(1);
    ctrl_2.write(4);
    s_input[0].write(0);
    s_input[1].write(0);
    sc_start(10, SC_NS);
    stage1_rst.write(true);
    stage3_rst.write(true);

    //cycle 1:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k][i].write(input_matrix[k][i][0]);
        }
    }
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k][i].write(filter[k][i][0]);
        }
    }

    ctrl.write(0);
    sc_start(10, SC_NS);

    // for(int i=0; i<9; i++) {
    //     buffer[i].write(1);
    //     ctrl_stage1.write(0);
    // }

    //cycle 2:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k][i].write(input_matrix[k][i][1]);
        }
    }
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k][i].write(filter[k][i][0]);
        }
    }

    ctrl.write(1);
    sc_start(10, SC_NS);

    //cycle 3:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k][i].write(input_matrix[k][i][1]);
        }
    }

    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k][i].write(filter[k][i][1]);
        }
    }

    ctrl.write(0);
    sc_start(10, SC_NS);

    //cycle 4:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k][i].write(input_matrix[k][i][2]);
        }
    }
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k][i].write(filter[k][i][0]);
        }
    }

    ctrl.write(2);
    sc_start(10, SC_NS);

    //cycle 5:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k][i].write(input_matrix[k][i][2]);
        }
    }

    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k][i].write(filter[k][i][1]);
        }
    }

    ctrl.write(1);
    sc_start(10, SC_NS);

    //cycle 6:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k][i].write(input_matrix[k][i][2]);
        }
    }

    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k][i].write(filter[k][i][2]);
        }
    }

    ctrl.write(0);
    sc_start(10, SC_NS);

    //cycle 7:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k][i].write(input_matrix[k][i][3]);
        }
    }

    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k][i].write(filter[k][i][0]);
        }
    }

    ctrl.write(3);
    sc_start(10, SC_NS);

    //cycle 8:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k][i].write(input_matrix[k][i][3]);
        }
    }
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k][i].write(filter[k][i][1]);
        }
    }

    ctrl.write(2);
    ctrl_2.write(0);
    sc_start(10, SC_NS);

    //cycle 9:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k][i].write(input_matrix[k][i][3]);
        }
    }
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k][i].write(filter[k][i][2]);
        }
    }
    ctrl.write(1);
    ctrl_2.write(4);
    sc_start(10, SC_NS);

    //cycle 10:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k][i].write(input_matrix[k][i][4]);
        }
    }
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k][i].write(filter[k][i][1]);
        }
    }
    ctrl.write(3);
    sc_start(10, SC_NS);

    //cycle 11:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k][i].write(input_matrix[k][i][4]);
        }
    }

    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k][i].write(filter[k][i][2]);
        }
    }
    ctrl.write(2);
    ctrl_2.write(1);
    sc_start(10, SC_NS);

    //cycle 12:
    //given input and weight
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k][i].write(input_matrix[k][i][5]);
        }
    }
    for (int k = 0; k < 8; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k][i].write(filter[k][i][2]);
        }
    }
    ctrl.write(3);
    ctrl_2.write(4);
    sc_start(10, SC_NS);

    //cycle13:
    //ctrl.write(0);
    ctrl_2.write(2);
    sc_start(10, SC_NS);

    //cycle14:
    ctrl_2.write(3);
    sc_start(10, SC_NS);

    //cycle15:
    ctrl_2.write(4);
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k-8][i].write(0);
        }
    }
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k-8][i].write(0);
        }
    }
    stage1_rst.write(false);
    sc_start(20, SC_NS);

    //cycle16:
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k-8][i].write(input_matrix[k][i][0]);
        }
    }
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k-8][i].write(filter[k][i][0]);
        }
    }
    stage1_rst.write(true);
    ctrl.write(0);

    sc_start(10, SC_NS);
    
    //cycle17:
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k-8][i].write(input_matrix[k][i][1]);
        }
    }
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k-8][i].write(filter[k][i][0]);
        }
    }
    ctrl.write(1);
    sc_start(10, SC_NS);

    //cycle 18:
    //given input and weight
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k-8][i].write(input_matrix[k][i][1]);
        }
    }

    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k-8][i].write(filter[k][i][1]);
        }
    }

    ctrl.write(0);
    sc_start(10, SC_NS);

    //cycle 19:
    //given input and weight
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k-8][i].write(input_matrix[k][i][2]);
        }
    }
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k-8][i].write(filter[k][i][0]);
        }
    }

    ctrl.write(2);
    sc_start(10, SC_NS);

    //cycle 20:
    //given input and weight
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k-8][i].write(input_matrix[k][i][2]);
        }
    }

    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k-8][i].write(filter[k][i][1]);
        }
    }

    ctrl.write(1);
    sc_start(10, SC_NS);

    //cycle 21:
    //given input and weight
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k-8][i].write(input_matrix[k][i][2]);
        }
    }

    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k-8][i].write(filter[k][i][2]);
        }
    }

    ctrl.write(0);
    sc_start(10, SC_NS);

    //cycle 22:
    //given input and weight
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k-8][i].write(input_matrix[k][i][3]);
        }
    }

    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k-8][i].write(filter[k][i][0]);
        }
    }

    ctrl.write(3);
    sc_start(10, SC_NS);

    //cycle 23:
    //given input and weight
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k-8][i].write(input_matrix[k][i][3]);
        }
    }
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k-8][i].write(filter[k][i][1]);
        }
    }

    ctrl.write(2);
    ctrl_2.write(0);
    sc_start(10, SC_NS);

    //cycle 24:
    //given input and weight
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            input[k-8][i].write(input_matrix[k][i][3]);
        }
    }
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {

            weight[k-8][i].write(filter[k][i][2]);
        }
    }
    ctrl.write(1);
    ctrl_2.write(4);
    sc_start(10, SC_NS);

    //cycle 25:
    //given input and weight
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k-8][i].write(input_matrix[k][i][4]);
        }
    }
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k-8][i].write(filter[k][i][1]);
        }
    }
    ctrl.write(3);
    sc_start(10, SC_NS);

    //cycle 26:
    //given input and weight
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k-8][i].write(input_matrix[k][i][4]);
        }
    }

    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k-8][i].write(filter[k][i][2]);
        }
    }
    ctrl.write(2);
    ctrl_2.write(1);
    sc_start(10, SC_NS);

    //cycle 27:
    //given input and weight
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 7; i++)
        {
            input[k-8][i].write(input_matrix[k][i][5]);
        }
    }
    for (int k = 8; k < 16; k++)
    {
        for (int i = 0; i < 3; i++)
        {
            weight[k-8][i].write(filter[k][i][2]);
        }
    }
    ctrl.write(3);
    ctrl_2.write(4);
    sc_start(10, SC_NS);

    //cycle 28:
    ctrl_2.write(2);
    sc_start(10, SC_NS);

    //cycle 29:
    ctrl_2.write(3);
    sc_start(10, SC_NS);

    //cycle30, 31
    ctrl_2.write(4);
    sc_start(20, SC_NS);

    sc_close_vcd_trace_file(tf);
    return (0);
}
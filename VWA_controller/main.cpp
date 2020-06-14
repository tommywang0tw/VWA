#include "Controller.h"
#include "InputSRAM.h"
#include "WeightSRAM.h"
#include "PE.h"
#include "Accumulator_First.h"
#include "TreeAdder.h"
#include "Accumulator_Third.h"
#include "systemc.h"


int sc_main(int argc, char *argv[])
{

    sc_report_handler::set_actions (SC_WARNING, SC_DO_NOTHING);

    sc_signal<sc_int<32> > inputsram_out_data[8][7];
    sc_signal<sc_int<32> > weightsram_out_data[8][3];
    sc_signal<sc_int<32> > pe_buffer[8][9];
    sc_signal<sc_int<32> > output[8][BUFFER_NUM];
    sc_signal<sc_int<32> > t_output[BUFFER_NUM];
    sc_signal<sc_int<32> > s_input[2]; //boundary SRAM
    sc_signal<sc_int<32> > output_P[BUFFER_NUM-2]; // output to post process
    sc_signal<sc_int<32> > output_S[2]; // output to boundary SRAM 
    sc_signal<bool> controller_rst, rst;
    sc_signal<bool> write_to_pe;
    sc_signal<bool> start;

    sc_signal<sc_uint<32> > input_ch, f_size, input_col, weight_col;
    sc_signal<sc_uint<32> > stage1_ctrl, stage3_ctrl1, stage3_ctrl2;
    sc_signal<bool> stage1_rst, stage3_rst;


    sc_time clkPrd(10, SC_NS);
    sc_clock clk("clock", clkPrd, 0.50);


    CONTROLLER controller("CONTROLLER");
    InputSRAM  inputsram("INPUTSRAM");
    WeightSRAM weightsram("WEIGHTSRAM");
    

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

//controller port connect
    controller.clk(clk);
    controller.rst(controller_rst);
    controller.start(start);
    controller.input_ch(input_ch);
    controller.f_size(f_size);
    controller.write_to_pe(write_to_pe);
    controller.out_input_ch(input_ch);
    controller.out_f_size(f_size);
    controller.input_col(input_col);
    controller.weight_col(weight_col);
    controller.stage1_ctrl(stage1_ctrl);
    controller.stage3_ctrl1(stage3_ctrl1);
    controller.stage3_ctrl2(stage3_ctrl2);
    controller.stage1_rst(stage1_rst);
    controller.stage3_rst(stage3_rst);

//inputsram port connect
    inputsram.clk(clk);
    inputsram.rst(rst);
    inputsram.write_to_pe(write_to_pe);
    inputsram.input_ch(input_ch);
    inputsram.input_col(input_col);

    for(int i = 0; i < 8; i++){
        for(int j = 0 ; j < 7; j++){
            inputsram.out_data[i][j](inputsram_out_data[i][j]);
        }
    }

//Weightsram port connect
    weightsram.clk(clk);
    weightsram.rst(rst);
    weightsram.write_to_pe(write_to_pe);
    weightsram.input_ch(input_ch);
    weightsram.f_size(f_size);
    weightsram.weight_col(weight_col);

    for(int i = 0; i < 8; i++){
        for(int j = 0 ; j < 3; j++){
            weightsram.out_data[i][j](weightsram_out_data[i][j]);
        }
    }    

//PE port connect
    for (int i = 0; i < 7; i++)
    {
        pe1.input[i](inputsram.out_data[0][i]);
        pe2.input[i](inputsram.out_data[1][i]);
        pe3.input[i](inputsram.out_data[2][i]);
        pe4.input[i](inputsram.out_data[3][i]);
        pe5.input[i](inputsram.out_data[4][i]);
        pe6.input[i](inputsram.out_data[5][i]);
        pe7.input[i](inputsram.out_data[6][i]);
        pe8.input[i](inputsram.out_data[7][i]);
    }

    for (int i = 0; i < 3; i++)
    {
        pe1.weight[i](weightsram_out_data[0][i]);
        pe2.weight[i](weightsram_out_data[1][i]);
        pe3.weight[i](weightsram_out_data[2][i]);
        pe4.weight[i](weightsram_out_data[3][i]);
        pe5.weight[i](weightsram_out_data[4][i]);
        pe6.weight[i](weightsram_out_data[5][i]);
        pe7.weight[i](weightsram_out_data[6][i]);
        pe8.weight[i](weightsram_out_data[7][i]);
    }

    for (int i = 0; i < 9; i++)
    {
        pe1.buffer[i](pe_buffer[0][i]);
        pe2.buffer[i](pe_buffer[1][i]);
        pe3.buffer[i](pe_buffer[2][i]);
        pe4.buffer[i](pe_buffer[3][i]);
        pe5.buffer[i](pe_buffer[4][i]);
        pe6.buffer[i](pe_buffer[5][i]);
        pe7.buffer[i](pe_buffer[6][i]);
        pe8.buffer[i](pe_buffer[7][i]);
    }

//acc stage1 port connect
    for (int i = 0; i < BUFFER_NUM; i++)
    {
        Accumulator_First1.input[i](pe_buffer[0][i]);
        Accumulator_First1.output[i](output[0][i]);
        Accumulator_First2.input[i](pe_buffer[1][i]);
        Accumulator_First2.output[i](output[1][i]);
        Accumulator_First3.input[i](pe_buffer[2][i]);
        Accumulator_First3.output[i](output[2][i]);
        Accumulator_First4.input[i](pe_buffer[3][i]);
        Accumulator_First4.output[i](output[3][i]);
        Accumulator_First5.input[i](pe_buffer[4][i]);
        Accumulator_First5.output[i](output[4][i]);
        Accumulator_First6.input[i](pe_buffer[5][i]);
        Accumulator_First6.output[i](output[5][i]);
        Accumulator_First7.input[i](pe_buffer[6][i]);
        Accumulator_First7.output[i](output[6][i]);
        Accumulator_First8.input[i](pe_buffer[7][i]);
        Accumulator_First8.output[i](output[7][i]);
    }
    Accumulator_First1.ctrl(stage1_ctrl);
    Accumulator_First1.clk(clk);
    Accumulator_First1.rst(stage1_rst);
    Accumulator_First2.ctrl(stage1_ctrl);
    Accumulator_First2.clk(clk);
    Accumulator_First2.rst(stage1_rst);
    Accumulator_First3.ctrl(stage1_ctrl);
    Accumulator_First3.clk(clk);
    Accumulator_First3.rst(stage1_rst);
    Accumulator_First4.ctrl(stage1_ctrl);
    Accumulator_First4.clk(clk);
    Accumulator_First4.rst(stage1_rst);
    Accumulator_First5.ctrl(stage1_ctrl);
    Accumulator_First5.clk(clk);
    Accumulator_First5.rst(stage1_rst);
    Accumulator_First6.ctrl(stage1_ctrl);
    Accumulator_First6.clk(clk);
    Accumulator_First6.rst(stage1_rst);
    Accumulator_First7.ctrl(stage1_ctrl);
    Accumulator_First7.clk(clk);
    Accumulator_First7.rst(stage1_rst);
    Accumulator_First8.ctrl(stage1_ctrl);
    Accumulator_First8.clk(clk);
    Accumulator_First8.rst(stage1_rst);

//Treeadder port connect
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            treeAdder.input[i][j](output[i][j]);
        }
    }

    for (int i = 0; i < 9; i++)
        treeAdder.output[i](t_output[i]);

//acc stage3 port connect

    Accumulator_Third.clk(clk);
    Accumulator_Third.rst(stage3_rst);

    for(int i = 0; i < BUFFER_NUM; i++)
        Accumulator_Third.input_T[i](t_output[i]);
    for(int i = 0; i < 2; i++)
        Accumulator_Third.input_S[i](s_input[i]);

    Accumulator_Third.ctrl_1(stage3_ctrl1);
    Accumulator_Third.ctrl_2(stage3_ctrl2);

    for(int i = 0; i < BUFFER_NUM - 2; i++)
        Accumulator_Third.output_P[i](output_P[i]);
    for(int i = 0; i < 2; i++)
        Accumulator_Third.output_S[i](output_S[i]);



    sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");

//trace controller signal
    sc_trace(tf, controller.clk, "clk");
    sc_trace(tf, controller.rst, "rst");
    sc_trace(tf, controller.start, "start");
    sc_trace(tf, controller.input_ch, "input_ch");
    sc_trace(tf, controller.write_to_pe, "write_to_pe");
    sc_trace(tf, controller.input_col, "input_col");
    sc_trace(tf, controller.weight_col, "weight_col");

//trace inputsram signal
    sc_trace(tf, inputsram.out_data[0][0], "inputsramout_data[0][0]");
    sc_trace(tf, inputsram.out_data[0][1], "inputsramout_data[0][1]");
    sc_trace(tf, inputsram.out_data[0][2], "inputsramout_data[0][2]");
    sc_trace(tf, inputsram.out_data[0][3], "inputsramout_data[0][3]");
    sc_trace(tf, inputsram.out_data[0][4], "inputsramout_data[0][4]");
    sc_trace(tf, inputsram.out_data[0][5], "inputsramout_data[0][5]");
    sc_trace(tf, inputsram.out_data[0][6], "inputsramout_data[0][6]");

    sc_trace(tf, inputsram.out_data[7][0], "inputsramout_data[7][0]");
    sc_trace(tf, inputsram.out_data[7][1], "inputsramout_data[7][1]");
    sc_trace(tf, inputsram.out_data[7][2], "inputsramout_data[7][2]");
    sc_trace(tf, inputsram.out_data[7][3], "inputsramout_data[7][3]");
    sc_trace(tf, inputsram.out_data[7][4], "inputsramout_data[7][4]");
    sc_trace(tf, inputsram.out_data[7][5], "inputsramout_data[7][5]");
    sc_trace(tf, inputsram.out_data[7][6], "inputsramout_data[7][6]");

//trace weightsram signal
    sc_trace(tf, weightsram.out_data[0][0], "weightsramout_data[0][0]");
    sc_trace(tf, weightsram.out_data[0][1], "weightsramout_data[0][1]");
    sc_trace(tf, weightsram.out_data[0][2], "weightsramout_data[0][2]");
    sc_trace(tf, weightsram.out_data[1][0], "weightsramout_data[1][0]");
    sc_trace(tf, weightsram.out_data[1][1], "weightsramout_data[1][1]");
    sc_trace(tf, weightsram.out_data[1][2], "weightsramout_data[1][2]");
    sc_trace(tf, weightsram.out_data[2][0], "weightsramout_data[2][0]");
    sc_trace(tf, weightsram.out_data[2][1], "weightsramout_data[2][1]");
    sc_trace(tf, weightsram.out_data[2][2], "weightsramout_data[2][2]");
    sc_trace(tf, weightsram.out_data[3][0], "weightsramout_data[3][0]");
    sc_trace(tf, weightsram.out_data[3][1], "weightsramout_data[3][1]");
    sc_trace(tf, weightsram.out_data[3][2], "weightsramout_data[3][2]");
    sc_trace(tf, weightsram.out_data[4][0], "weightsramout_data[4][0]");
    sc_trace(tf, weightsram.out_data[4][1], "weightsramout_data[4][1]");
    sc_trace(tf, weightsram.out_data[4][2], "weightsramout_data[4][2]");
    sc_trace(tf, weightsram.out_data[5][0], "weightsramout_data[5][0]");
    sc_trace(tf, weightsram.out_data[5][1], "weightsramout_data[5][1]");
    sc_trace(tf, weightsram.out_data[5][2], "weightsramout_data[5][2]");
    sc_trace(tf, weightsram.out_data[6][0], "weightsramout_data[6][0]");
    sc_trace(tf, weightsram.out_data[6][1], "weightsramout_data[6][1]");
    sc_trace(tf, weightsram.out_data[6][2], "weightsramout_data[6][2]");
    sc_trace(tf, weightsram.out_data[7][0], "weightsramout_data[7][0]");
    sc_trace(tf, weightsram.out_data[7][1], "weightsramout_data[7][1]");
    sc_trace(tf, weightsram.out_data[7][2], "weightsramout_data[7][2]");

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
    sc_trace(tf, Accumulator_Third.shift_regs[0][0], "Accumulator_Third.shift_regs(0)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[0][1], "Accumulator_Third.shift_regs(0)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[0][2], "Accumulator_Third.shift_regs(0)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[0][3], "Accumulator_Third.shift_regs(0)(3)");
    sc_trace(tf, Accumulator_Third.shift_regs[0][4], "Accumulator_Third.shift_regs(0)(4)");

    sc_trace(tf, Accumulator_Third.shift_regs[1][0], "Accumulator_Third.shift_regs(1)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[1][1], "Accumulator_Third.shift_regs(1)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[1][2], "Accumulator_Third.shift_regs(1)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[1][3], "Accumulator_Third.shift_regs(1)(3)");
    sc_trace(tf, Accumulator_Third.shift_regs[1][4], "Accumulator_Third.shift_regs(1)(4)");

    sc_trace(tf, Accumulator_Third.shift_regs[2][0], "Accumulator_Third.shift_regs(2)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][1], "Accumulator_Third.shift_regs(2)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][2], "Accumulator_Third.shift_regs(2)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][3], "Accumulator_Third.shift_regs(2)(3)");
    sc_trace(tf, Accumulator_Third.shift_regs[2][4], "Accumulator_Third.shift_regs(2)(4)");

    sc_trace(tf, Accumulator_Third.shift_regs[3][0], "Accumulator_Third.shift_regs(3)(0)");
    sc_trace(tf, Accumulator_Third.shift_regs[3][1], "Accumulator_Third.shift_regs(3)(1)");
    sc_trace(tf, Accumulator_Third.shift_regs[3][2], "Accumulator_Third.shift_regs(3)(2)");
    sc_trace(tf, Accumulator_Third.shift_regs[3][3], "Accumulator_Third.shift_regs(3)(3)");
    sc_trace(tf, Accumulator_Third.shift_regs[3][4], "Accumulator_Third.shift_regs(3)(4)");

    sc_trace(tf, Accumulator_Third.ctrl2_reg, "Accumulator.ctrl2_reg");
    sc_trace(tf, Accumulator_Third.input_T[2], "Accumulator_Third.input_T(2)");


    stage1_rst.write(false);
    stage3_rst.write(false);
    stage1_ctrl.write(0);
    stage3_ctrl1.write(1);
    stage3_ctrl2.write(4);
    s_input[0].write(0);
    s_input[1].write(0);
    controller_rst.write(false);
    rst.write(false);
    sc_start(10, SC_NS);

    stage1_rst.write(true);
    stage3_rst.write(true);
    controller_rst.write(true);
    rst.write(true);
    start.write(1);
    input_ch.write(8);
    sc_start(500, SC_NS);

    sc_close_vcd_trace_file(tf);
    return 0;
}
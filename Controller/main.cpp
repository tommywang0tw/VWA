#include "systemc.h"
#include "Controller.h"

int sc_main(int argc, char *argv[]){

    sc_signal<sc_uint<32> > clk;
    sc_signal<sc_uint<32> > rst;
    sc_signal<sc_uint<32> > signal_stage1;

    Controller Controller("Controller");

    Controller.clk(clk); 
    Controller.rst(rst); 
    Controller.signal_stage1(signal_stage1); 

    sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");

    sc_trace(tf, Controller.output ,"output");

    for(int i = 0; i < 7; i++) {
        input[i] = i+2;
    }

    bias = 1;
    
    sc_start(100,SC_NS);
    for (size_t i = 0; i < 7; i++)
    {
        std::cout << "output[" << i << "]=" << output[i].read() << std::endl;
    }
    
    sc_close_vcd_trace_file(tf); 

    return (0);
}
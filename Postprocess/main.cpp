#include "systemc.h"
#include "Postprocess.h"

int sc_main(int argc, char *argv[]){

    sc_signal<sc_uint<32> > input[7];
    sc_signal<sc_uint<32> > bias;
    sc_signal<sc_uint<32> > output[7];

    Postprocess Postprocess("Postprocess");

    for(int i = 0; i < 7; i++) {
        Postprocess.input[i](input[i]); 
        Postprocess.output[i](output[i]);
    }
    Postprocess.bias(bias); 

    sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");

    sc_trace(tf, Postprocess.output ,"output");

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
#include "TreeAdder.h"
#include "systemc.h"

int sc_main(int argc, char *argv[]){

    sc_signal<sc_uint<32> > input[9][8];
    sc_signal<sc_uint<32> > output[9];

    TreeAdder TreeAdder("TreeAdder");

    for(int i = 0; i<9; i++) {
        for(int j=0; j<8; j++) {
            TreeAdder.input[i][j](input[i][j]); 
        }
    }

    for(int i=0; i<9; i++) {
        TreeAdder.output[i](output[i]);
    }
    sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");

    //trace input signal
    sc_trace(tf, TreeAdder.output[0] ,"output0");
    sc_trace(tf, TreeAdder.output[1] ,"output1");
    sc_trace(tf, TreeAdder.output[2] ,"output2");
    sc_trace(tf, TreeAdder.output[3] ,"output3");
    sc_trace(tf, TreeAdder.output[4] ,"output4");
    sc_trace(tf, TreeAdder.output[5] ,"output5");
    sc_trace(tf, TreeAdder.output[6] ,"output6");
    sc_trace(tf, TreeAdder.output[7] ,"output7");
    sc_trace(tf, TreeAdder.output[8] ,"output8");

    sc_start(10,SC_NS);
//given input and weight
    for(int i = 0; i < 9; i++) {
        for(int j=0; j<8; j++) {
            input[i][j] = i+1;
        }
    }
    
    sc_start(20,SC_NS);

    sc_close_vcd_trace_file(tf); 
    return (0);
}
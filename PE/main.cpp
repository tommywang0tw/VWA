#include "PE.h"
#include "systemc.h"

int sc_main(int argc, char *argv[]){

    sc_signal<sc_uint<32> > input[7], weight[3], buffer[9];
    int i, j, k;

    PE pe("PE");

    for(i = 0; i < 7; i++)
        pe.input[i](input[i]);
    for(j = 0; j < 3; j++)
        pe.weight[j](weight[j]);
    for(k = 0; k < 9; k++)
        pe.buffer[k](buffer[k]);

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

    sc_start(10,SC_NS);
//given input and weight
    for(int i = 0; i < 7; i++)
        input[i].write(i);
    for(int i = 0; i < 3; i++)
        weight[i].write(i);
    
    sc_start(20,SC_NS);

    sc_close_vcd_trace_file(tf); 
    return (0);
}
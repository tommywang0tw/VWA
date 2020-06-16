#include "PE.h"
#include "adder.h"
#include "systemc.h"

int sc_main(int argc, char *argv[]){

    sc_signal<sc_uint<32> > input[7], weight[3], buffer[9], out_data;
    int i, j, k;

    PE pe("PE");
    ADDER adder("ADDER");

    for(i = 0; i < 7; i++)
        pe.input[i](input[i]);
    for(j = 0; j < 3; j++)
        pe.weight[j](weight[j]);
    for(k = 0; k < 9; k++){
        pe.buffer[k](buffer[k]);
        adder.input[k](buffer[k]);
    }
    adder.out_data(out_data);

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

    //trace adder input signal
    sc_trace(tf, adder.input[0] ,"adder_input[0]");
    sc_trace(tf, adder.input[1] ,"adder_input[1]");
    sc_trace(tf, adder.input[2] ,"adder_input[2]");
    sc_trace(tf, adder.input[3] ,"adder_input[3]");
    sc_trace(tf, adder.input[4] ,"adder_input[4]");
    sc_trace(tf, adder.input[5] ,"adder_input[5]");
    sc_trace(tf, adder.input[6] ,"adder_input[6]");
    sc_trace(tf, adder.input[7] ,"adder_input[7]");
    sc_trace(tf, adder.input[8] ,"adder_input[8]");

    //trace adder output signal
    sc_trace(tf, adder.out_data , "adder_outdata");

    sc_start(10,SC_NS);

//given input and weight
    for(int i = 0; i < 7; i++){
        input[i].write(i+1);
        //cout << "input[0] : " << input[i].read() << " time : " << sc_time_stamp() << endl;
    }
    for(int i = 0; i < 3; i++)
        weight[i].write(i);
    
    //sc_start(10,SC_NS);
    sc_start(1,SC_NS);
    cout << "out_data : " << out_data.read() << " time : " << sc_time_stamp() << endl;

    cout << "after delay : " << sc_time_stamp() << "input[0] : " << input[0].read()  << endl;
    sc_close_vcd_trace_file(tf); 
    return (0);
}
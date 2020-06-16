//BoundarySRAM.h

#ifndef BOUNDARYSRAM_H
#define BOUNDARYSRAM_H

#include "systemc.h"

SC_MODULE(BoundarySRAM) {

/*  ============================================================
            Input Ports: clock and control signals 
    ============================================================
*/
	sc_in<bool> clk;			    // clock
	sc_in<bool> rst;			    // reset

    //from controller
    sc_in<bool> w;        //write data to boundary      
    sc_in<bool> r;        //read data from boundary
    sc_in<sc_uint<32> > read_address;   
    sc_in<sc_uint<32> > write_address;     

/*  ============================================================
            Input Ports
    ============================================================
*/
    //data from Accumulator
    sc_in<sc_int<32> > in_data[2]; //input 2 data from Accumulator 


/*  ============================================================
            Output Ports
    ============================================================
*/
    sc_out<sc_int<32> > out_data[2]; //output 2 data to Accumulator 

/*  ============================================================
            Inner Data
    ============================================================
*/
	int boundary[2][824]; //2*(8*103)
    sc_uint<1> write_row;//which row to write 
    sc_uint<1> read_row;//which row to read 
    int write_row_cal; 
    int read_row_cal; 
		
    void do_BoundarySRAM();
    
    SC_CTOR(BoundarySRAM)
    {

        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 824; j++)
                {
                    boundary[i][j] = 0;
            }           
        }
        SC_CTHREAD(do_BoundarySRAM, clk.pos());
		reset_signal_is(rst, false);           
    }
};

#endif
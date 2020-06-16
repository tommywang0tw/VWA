//WeightSRAM.h

#ifndef WEIGHTSRAM_H
#define WEIGHTSRAM_H

#include "systemc.h"

SC_MODULE(WeightSRAM) {

/*  ============================================================
            Input Ports: clock and control signals 
    ============================================================
*/
	sc_in<bool> clk;			    // clock
	sc_in<bool> rst;			    // reset

    //from controller
    sc_in<bool> write_to_pe;        //write weight to PE      
    sc_in<sc_uint<32> > input_ch;   //num of input channel
    sc_in<sc_uint<32> > f_size;     //filter size
    sc_in<sc_uint<32> > weight_col;  //column of weight 

/*  ============================================================
            Input Ports
    ============================================================
*/
    //data from DRAM

/*  ============================================================
            Output Ports
    ============================================================
*/
    sc_out<sc_int<32> > out_data[8][3]; //output 7 data to 8 PE 

/*  ============================================================
            Inner Data
    ============================================================
*/
	int filter[16][3][3];		// 3(Width) x 3(Height) x 16(channel) data in Weights SRAM
    int cur_ch;                         // current channel
    int cur_tile_col;                   // current tile column


    void do_WeightSRAM();
    
    SC_CTOR(WeightSRAM)
    {
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
                    filter[k+8][i][j] = 3 * i + j + 1 +k;
                }
            }
        }

        SC_CTHREAD(do_WeightSRAM, clk.pos());
		reset_signal_is(rst, false);           
    }
};

#endif
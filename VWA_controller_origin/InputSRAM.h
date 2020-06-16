//InputSRAM.h

#ifndef INPUTSRAM_H
#define INPUTSRAM_H

#include "systemc.h"

SC_MODULE(InputSRAM) {

/*  ============================================================
            Input Ports: clock and control signals 
    ============================================================
*/
	sc_in<bool> clk;			    // clock
	sc_in<bool> rst;			    // reset

    sc_in<bool> write_to_pe;        //from controller
    sc_in<sc_uint<32> > input_ch;   //num of input channel
    sc_in<sc_uint<32> > input_col;  //column of input

/*  ============================================================
            Input Ports
    ============================================================
*/
    // data from DRAM

/*  ============================================================
            Output Ports
    ============================================================
*/
    sc_out<sc_int<32> > out_data[8][7]; //output 7 data to 8 PE 

/*  ============================================================
            Inner Data
    ============================================================
*/
	int input_matrix[16][7][6];			// 6(Width) x 7(Height) x 16(channel) data in Input SRAM
    int cur_ch;                         // current channel
    int cur_tile_col;                   // current tile column


    void do_InputSRAM();
    
    SC_CTOR(InputSRAM)
    {
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
        SC_CTHREAD(do_InputSRAM, clk.pos());
		reset_signal_is(rst, false);         
    }
};

#endif
//WeightSRAM.h

#ifndef WEIGHTSRAM_H
#define WEIGHTSRAM_H

#include "systemc.h"
#include "ps_config.h"

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

    sc_in<sc_uint<32> > weight_bank1_addr; 
    sc_in<sc_uint<32> > weight_bank2_addr; 
    sc_in<sc_uint<32> > weight_bank3_addr; 
    sc_in<sc_uint<32> > weight_bank4_addr; 
    sc_in<sc_uint<32> > weight_bank5_addr; 
    sc_in<sc_uint<32> > weight_bank6_addr; 
    sc_in<sc_uint<32> > weight_bank7_addr; 
    sc_in<sc_uint<32> > weight_bank8_addr; 

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
    int weight_data[8][576]; 

    int cur_ch;                         // current channel
    int cur_tile_col;                   // current tile column


    void do_WeightSRAM();
    
    SC_CTOR(WeightSRAM)
    {

        for (int k = 0; k < 8; k++)
        {
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    weight_data[k][i + 3 * j] = 3 * i + j + 1 + k;
                    cout << "weight_data index : " << i + 3 * j << " = " <<  3 * i + j + 1 + k << endl;
                }
            }
        }
        cout << "weight[0][8]" << weight_data[0][8] << endl;
        for (int k = 0; k < 8; k++)
        {
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    weight_data[k][i + 3 * j + 9] = 3 * i + j + 1 ;
                }
            }
        }

        for (int k = 0; k < 8; k++)
        {
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    weight_data[k][i + 3 * j + 18] = 3 * i + j + 1 ;
                }
            }
        }

        for (int k = 0; k < 8; k++)
        {
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    weight_data[k][i + 3 * j + 27] = 3 * i + j + 1 ;
                }
            }
        }
            
        SC_CTHREAD(do_WeightSRAM, clk.pos());
		reset_signal_is(rst, false);           
    }
};

#endif
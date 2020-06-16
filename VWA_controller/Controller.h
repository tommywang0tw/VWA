//Controller.h

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "systemc.h"

#define B_SRAM_BANK_SIZE 3296

SC_MODULE(CONTROLLER) {

/*  ============================================================
            Input Ports: clock and control signals 
    ============================================================
*/
	sc_in<bool> clk;			// clock
	sc_in<bool> rst;			// reset

/*  ============================================================
            Input Ports
    ============================================================
*/
    sc_in<bool> start;
    sc_in<sc_uint<32> > input_ch;       //num of input channel
    sc_in<sc_uint<32> > f_size;         //filter size
    sc_in<sc_uint<32> > in_image_col, in_image_row;

/*  ============================================================
            Output Ports
    ============================================================
*/
    sc_out<bool> write_to_pe;           //tell InputSRAM move data to PE
    sc_out<sc_uint<32> > out_f_size;    //output filter size to WeightSRAM
    sc_out<sc_uint<32> > inputS_ch;  //output num of input channel to InputSRAM
    sc_out<sc_uint<32> > inputS_row;
    sc_out<sc_uint<32> > inputS_col;    //column of input
    sc_out<sc_uint<32> > weight_col;    //column of weight
    sc_out<sc_uint<32> > weight_ch;
    sc_out<sc_uint<32> > stage1_ctrl; //stage 1 control signal
    sc_out<sc_uint<32> > stage3_ctrl1, stage3_ctrl2; //stage 3 control signal
    sc_out<bool> stage1_rst, stage3_rst;
    sc_out<bool> read_boundary, write_boundary;
    sc_out<sc_uint<32> > boundary_read_addr, boundary_write_addr; //boundary w/r address
    sc_out<bool> PE_input_ctrl[8][7]; //for eight PE
/*  ============================================================
            Inner Data
    ============================================================
*/
    sc_uint<32> cycle_count; //current cycle 
    sc_uint<32> current_tile_col; //current tile counter in col
    sc_uint<32> num_of_tile_col;  

    sc_uint<32> current_tile_row; //current tile counter in row
    sc_uint<32> num_of_tile_row;  

    sc_uint<32> read_addr; //add 8 each time
    sc_uint<32> write_addr; //add 8 each time

    sc_uint<32> stage3_rst_count;

    //Flags for abnormal tiles
    bool right_bad_tile;
    bool bottom_bad_tile;

    void do_Controller();
    
    SC_CTOR(CONTROLLER){
        SC_CTHREAD(do_Controller,clk.pos());
        reset_signal_is(rst, false);        
    }
};

#endif
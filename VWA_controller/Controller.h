//Controller.h

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "systemc.h"

#define B_SRAM_BANK_SIZE 3296

#include "ps_config.h"


SC_MODULE(CONTROLLER) {

/*  ============================================================
            Input Ports: clock and control signals 
    ============================================================
*/
	sc_in<bool> clk;			// clock
	sc_in<bool> rst;			// reset

    sc_in<bool> dma_done;           //from dma
    
    //sc_in<bool> input_done;            //from dma
/*  ============================================================
            Input Ports
    ============================================================
*/
    sc_in<bool> start;
    sc_in<sc_uint<32> > input_size;
    sc_in<sc_uint<32> > input_ch;       //num of input channel
    sc_in<sc_uint<32> > f_size;         //filter size
    sc_in<sc_uint<32> > f_num;          //number of filter

/*  ============================================================
            Output Ports
    ============================================================
*/
    sc_out<bool> write_to_pe;           //tell InputSRAM move data to PE
    sc_out<bool> read_boundary, write_boundary;
    sc_out<sc_uint<32> > boundary_read_addr, boundary_write_addr; //boundary w/r address
    sc_out<bool> PE_input_ctrl[8][7]; //for eight PE
    sc_out<sc_uint<32> > out_input_ch;  //output num of input channel to InputSRAM(input ch = filter ch)
    sc_out<sc_uint<32> > out_f_size;    //output filter size to WeightSRAM
    sc_out<sc_uint<32> > inputS_col;    //column of input
    sc_out<sc_uint<32> > stage1_ctrl; //stage 1 control signal
    sc_out<sc_uint<32> > stage3_ctrl1, stage3_ctrl2; //stage 3 control signal
    sc_out<bool> stage1_rst, stage3_rst;
    
    //to DMA
    sc_out<sc_uint<32> > out_input_size;
    sc_out<sc_uint<32> > tile_height;
    sc_out<sc_uint<32> > tile_width;
    sc_out<bool> read_in;
    sc_out<bool> write_back;
    sc_out<sc_uint<32> > source_address;
    sc_out<sc_uint<32> > target_address;
    sc_out<sc_uint<32> > data_length;
    sc_out<bool> sram_mode;
    



    // to weightSRAM
    sc_out<sc_uint<32> > weight_bank_addr[8];

    //to inputSRAM
    sc_out<sc_uint<32> > input_bank_addr[8];
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
    sc_uint<32> tmp_weight_addr; //control register
    sc_uint<32> tmp_input_addr;  //control register
    sc_uint<32> tmp_input_ch;    //control register
    sc_uint<32> tmp_filter_ch;   //control register
    sc_uint<32> tmp_tile_height;
    sc_uint<32> tmp_tile_width;
    sc_uint<32> height;
    sc_uint<32> width;
    sc_uint<32> channel;
    sc_uint<32> input_move_count;
    sc_uint<32> total_input_move_count;
    sc_uint<32> tile_count;
    sc_uint<32> tile_inform[4][1000]; //record every tiles information, tile width, height and address
    sc_uint<32> next_input_addr;

    sc_uint<32> tmp_src_addr;    //control register  
    sc_uint<32> tmp_tgt_addr;    //control register
    sc_uint<32> tmp_dram_input_addr;   //control register
    sc_uint<32> tmp_dram_weight_addr;   //control register
    sc_uint<32> total_weight;
    sc_uint<32> conv_input_time;
    sc_uint<32> conv_weight_time;
    sc_uint<32> input_data_ready;
    sc_uint<32> filter_col;

    sc_uint<32> tmp_move_weight_count;  //how many times u need to move weights data
    sc_uint<32> move_weight_count;
    sc_uint<32> weight_count;
    sc_uint<32> state;
    sc_uint<32> counter;
    
    int unroll_image_col, unroll_image_size;

    void do_Controller();
    
    SC_CTOR(CONTROLLER){
        SC_CTHREAD(do_Controller,clk.pos());
        reset_signal_is(rst, false);        
    }
};

#endif
#include "InputSRAM.h"

void InputSRAM::do_InputSRAM(){

// reset
    cur_ch = 0;
    cur_tile_col = 0;

    while(1){

        wait();
        
        if(write_to_pe.read()){

            for(int i = 0; i < 8; i++){
                for(int j = 0; j < 7; j++){
                    out_data[i][j] = input_matrix[input_ch.read()+i][input_row.read()+j][input_col.read()];
                }
            }
/*
            cur_ch++;
            cur_tile_col++;

            if(cur_ch > input_ch.read()){ //change to next tile channel 1
                cur_ch = 0;
                cur_tile_col = 0;
            }
*/
        }
        else{

            for(int i = 0; i < 8; i++){
                for(int j = 0; j < 7; j++){
                    out_data[i][j] = 0;
                }
            }

        }
    }
}
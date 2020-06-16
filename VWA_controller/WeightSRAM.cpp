#include "WeightSRAM.h"

void WeightSRAM::do_WeightSRAM(){

// reset
    
    while(1){

        wait();
        
        if(write_to_pe.read()){

            for(int i = 0; i < 8; i++){     //supppose filter channel = 8
                for(int j = 0; j < 3; j++){
                        out_data[i][j] = filter[input_ch.read() + i][j][weight_col.read()];
                }
            }
        }

        else{

            for(int i = 0; i < 8; i++){     //clear output
                for(int j = 0; j < 3; j++){
                        out_data[i][j] = 0;
                }
            }           
        }

    
    }
}
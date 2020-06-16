#include "WeightSRAM.h"

void WeightSRAM::do_WeightSRAM(){

// reset   

    while(1){

        wait();
        
        if(write_to_pe.read())
        {
            //weight address convert to index
            for(int i = 0; i < 3; i++)
            {
                out_data[0][i] = weight_data[(weight_bank1_addr.read() - WEIGHT_SRAM_BASE) / 4 / 576][(weight_bank1_addr.read() - WEIGHT_SRAM_BASE) / 4 % 576 + i];
                out_data[1][i] = weight_data[(weight_bank2_addr.read() - WEIGHT_SRAM_BASE) / 4 / 576][(weight_bank2_addr.read() - WEIGHT_SRAM_BASE) / 4 % 576 + i]; 
                out_data[2][i] = weight_data[(weight_bank3_addr.read() - WEIGHT_SRAM_BASE) / 4 / 576][(weight_bank3_addr.read() - WEIGHT_SRAM_BASE) / 4 % 576 + i]; 
                out_data[3][i] = weight_data[(weight_bank4_addr.read() - WEIGHT_SRAM_BASE) / 4 / 576][(weight_bank4_addr.read() - WEIGHT_SRAM_BASE) / 4 % 576 + i]; 
                out_data[4][i] = weight_data[(weight_bank5_addr.read() - WEIGHT_SRAM_BASE) / 4 / 576][(weight_bank5_addr.read() - WEIGHT_SRAM_BASE) / 4 % 576 + i]; 
                out_data[5][i] = weight_data[(weight_bank6_addr.read() - WEIGHT_SRAM_BASE) / 4 / 576][(weight_bank6_addr.read() - WEIGHT_SRAM_BASE) / 4 % 576 + i]; 
                out_data[6][i] = weight_data[(weight_bank7_addr.read() - WEIGHT_SRAM_BASE) / 4 / 576][(weight_bank7_addr.read() - WEIGHT_SRAM_BASE) / 4 % 576 + i]; 
                out_data[7][i] = weight_data[(weight_bank8_addr.read() - WEIGHT_SRAM_BASE) / 4 / 576][(weight_bank8_addr.read() - WEIGHT_SRAM_BASE) / 4 % 576 + i];
                cout << "addr : " << (weight_bank1_addr.read() - WEIGHT_SRAM_BASE) / 4 / 576 << endl; 
                //cout << "weight[0][8]" << weight_data[0][8] << endl;
                cout << weight_data[(weight_bank1_addr.read() - WEIGHT_SRAM_BASE) / 4 / 576][(weight_bank1_addr.read() - WEIGHT_SRAM_BASE) / 4 % 576 + i] << endl;
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
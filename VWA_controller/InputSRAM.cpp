#include "InputSRAM.h"

void InputSRAM::sram(){
    while(1){
        if(rst){
            for(int i=0; i< WEIGHT_SRAM_BANK;i++)
            {
                for(int j=0; j < 3;j++)
                {   
                    for(int k=0; k < 120;k++)
                    {
                        data_sram[i][j][k] = 0;
                    }
                }
            }
            data_f = 0;

            data_b = 0;
            done.write(0);
            vaild.write(0);
            sram_mode.write(0);
        }
        if (dma_write & !done){
            sram_mode.write(mode);
            if (mode){
                for(int i=0; i < 7;i++)
                {
                    data_sram[(data_f/TILE_WIDTH)%8][i][ (data_f/(8*TILE_WIDTH))*TILE_WIDTH + (data_f%TILE_WIDTH)] = data[i].read();
                }     
                data_f++;
            }
            else {          
                for(int i=0; i < 7;i++)
                {
                    data_sram[(data_f/TILE_WIDTH)%6][i][(data_f/(6*TILE_WIDTH))*TILE_WIDTH + (data_f%TILE_WIDTH)] = data[i].read();
                }
                data_f++;     
            }    
            done.write(1);      
        }
        else if (pe_read){
            vaild.write(1);
            if(sram_mode){
                for(int i=0; i< 8;i++)
                {
                    for(int j=0; j < 7;j++)
                    {
                        data_to_pe[i][j].write(data_sram[i][j][col_select.read()]);
                    }
                }
            }
            else{
                for(int i=0; i < 6;i++)
                {
                    for(int j=0; j < 7;j++)
                    {
                        data_to_pe[i][j].write(data_sram[i][j][col_select.read()]);
                    }
                }
            }
        }
        else {
            done.write(0);
            vaild.write(0);
        }      
        wait();
    }
}


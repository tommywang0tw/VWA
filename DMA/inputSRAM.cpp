#include "inputSRAM.h"

void inputSRAM::sram(){
    while(1){
        if(rst){
            for(int i=0; i< WEIGHT_SRAM_BANK;i++)
            {
                for(int j=0; j < 3;j++)
                {   
                    for(int k=0; j < 18;j++)
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
                    data_sram[(data_f/TILE_WIDTH)%8][i][data_f%TILE_WIDTH] = data[i].read();
                }     
                data_f++;
            }
            else {          
                for(int i=0; i < 7;i++)
                {
                data_sram[(data_f/TILE_WIDTH)%6][i][data_f%TILE_WIDTH] = data[i].read();
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
                        data_to_pe[i][j].write(data_sram[i][j][data_b]);
                    }
                }
                data_b++;
            }
            else{
                for(int i=0; i < 6;i++)
                {
                    for(int j=0; j < 7;j++)
                    {
                        data_to_pe[i][j].write(data_sram[i][j][data_b]);
                    }
                }
                data_b++;
            }
        }
        else {
            done.write(0);
            vaild.write(0);
        }      
        wait();
    }
}


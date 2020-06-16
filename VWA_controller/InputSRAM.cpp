#include "InputSRAM.h"

void InputSRAM::sram(){
    while(1){
        if(!rst){
            
            for(int i=0; i< WEIGHT_SRAM_BANK;i++)
            {
                for(int j=0; j < 7;j++)
                {   
                    for(int k=0; k < 120;k++)
                    {
                        //cout << "i = " << i << "j = " << j  << " k = " << k  << " to pe: " <<  data_to_pe[i][j] <<endl;
                        data_to_pe[i][j].write(0);
                    }
                }
            }
            cout << "rstrst" << endl;;
            data_f = 0;

            data_b = 0;
            done.write(0);
            vaild.write(0);
            sram_mode.write(0);
        }
        else if (dma_write & !done){
            //cout << "sram_mode =-=====" << mode;
            // sram_mode.write(mode);
            // if (mode){
                for(int i=0; i < 7;i++)
                {
                    data_sram[(data_f/TILE_WIDTH)%8][i][ (data_f/(8*TILE_WIDTH))*TILE_WIDTH + (data_f%TILE_WIDTH)] = data[i].read();
                }     
                data_f++;
            // }
            // else {          
            //     for(int i=0; i < 7;i++)
            //     {
            //         data_sram[(data_f/TILE_WIDTH)%6][i][(data_f/(6*TILE_WIDTH))*TILE_WIDTH + (data_f%TILE_WIDTH)] = data[i].read();
            //     }
            //     data_f++;     
            // }    
            done.write(1);      
        }
        else if (pe_read){
            //cout << "pe_read" << endl;
            vaild.write(1);
            // if(mode){
                for(int i=0; i< 8;i++)
                {
                    for(int j=0; j < 7;j++)
                    {
                        data_to_pe[i][j].write(data_sram[i][j][col_select.read()]);
                        //cout << "col_select: " << col_select.read()<< endl;
                        //cout << "pe out : " << data_sram[i][j][col_select.read()] << endl;
                    }
                }
            // }
            // else{
            //     for(int i=0; i < 6;i++)
            //     {
            //         for(int j=0; j < 7;j++)
            //         {
            //             data_to_pe[i][j].write(data_sram[i][j][col_select.read()]);
            //         }
            //     }
            // }
        }
        else {
            
            done.write(0);
            vaild.write(0);
        }      
        
        wait();
    }
}


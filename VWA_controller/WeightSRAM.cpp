#include "WeightSRAM.h"

void WeightSRAM::sram(){
    while(1){
        if (!rst){
            for(int i=0; i< WEIGHT_SRAM_BANK;i++)
            {
                for(int j=0; j < 3;j++)
                {   
                    for(int k=0; j < 18;j++)
                    {
                        weight_sram[i][j][k] = 0;
                    }
                }
            }
            data_f = 0;
            data_b = 0;
            done.write(0);
            vaild.write(0);
            sram_mode.write(0);
        }
        else if (dma_write & !done){
            sram_mode.write(mode);
            // if (mode){               
                for(int i=0; i < 9;i++)
                {
                    weight_sram[data_f%8][i%3][(data_f/8)*3+i/3] = data[i].read();
                }
            // }
            // else {
            //     for(int i=0; i < 9;i++)
            //     {
            //         weight_sram[data_f%6][i%3][(data_f/6)*3+i/3] = data[i].read();
            //     }
            // }
            data_f = data_f + 1;
            done.write(1);
        }
        else if (pe_read){
            //cout << "weight bank add: " << weight_bank_addr[0].read() << endl;
            vaild.write(1);
            // if (mode){
                for(int i=0; i< 8;i++)
                {
                    for(int j=0; j < 3;j++)
                    {   
                        data_to_pe[i][j].write(weight_sram[(weight_bank_addr[i].read() - WEIGHT_SRAM_BASE) / 4 / 576][j][ ( (weight_bank_addr[i].read() - WEIGHT_SRAM_BASE) / 4 % 576 ) /3]);
                    }
                }
            // }
            // else {
            //     for(int i=0; i< 6;i++)
            //     {
            //         for(int j=0; j < 3;j++)
            //         {
            //             data_to_pe[i][j].write(weight_sram[(weight_bank_addr[i].read() - WEIGHT_SRAM_BASE) / 4 / 576][j][ ( (weight_bank_addr[i].read() - WEIGHT_SRAM_BASE) / 4 %576 ) /12]);
            //         }
            //     }
            // }
            data_b++;
        }
        else {
            done.write(0);
            vaild.write(0);
        }
        wait();
    }
}


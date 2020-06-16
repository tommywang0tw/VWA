#include "outputSRAM.h"

void outputSRAM::sram(){
    while(1){
        if(rst){
            memset(outputSRAM,'0',695271424);
        }
        else{
            if (vaild){
                if(top){

                    for (int i = 0 ; i <5; i++){
                        output_SRAM[i][col_point][cha_point] = in_data[i+2];
                    }
                    if(col_point < col){
                        col_point++;
                    }
                    else {
                        col_point = 0;
                        row_point += 5;
                    }
                    
                }
                else{
                    if ( (row_point + 7) > row-1 ){ 

                    }
                    else{

                    }

                    for (int i = 0 ; i <7; i++){
                        output_SRAM[row + i][col_point][cha_point] = in_data[i];
                    }
                    if(col_point < col){
                        col_point++;
                    }
                    else {
                        col_point = 0;
                        row_point += 7;
                    }
                    
                    
                }
            }
            
        }
        wait();
    }       
}
#include "Controller.h"

void CONTROLLER::do_Controller(){

// reset
    cycle_count = 0;
    tmp_weight_addr = WEIGHT_SRAM_BASE;

    while(1){
        
        wait();
        
        if(start.read()){
            cout << "start!!!!!!" << endl;
            out_input_ch.write(input_ch.read());
            out_f_size.write(f_size.read());
        }

        cycle_count++;

        switch (cycle_count)
        {
            case 1:                  //cycle 1
                write_to_pe.write(1);
                weight_bank1_addr.write(tmp_weight_addr); 
                weight_bank2_addr.write(tmp_weight_addr + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 7 * WEIGHT_SRAM_BANK_SIZE);
                input_col.write(0);
                stage1_ctrl.write(0);
                stage1_rst.write(true);
                cout << "cycle 1 : ggggggg" << endl;
                break;

            case 2:                  //cycle 2
                weight_bank1_addr.write(tmp_weight_addr); 
                weight_bank2_addr.write(tmp_weight_addr + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 7 * WEIGHT_SRAM_BANK_SIZE);            
                input_col.write(1);
                stage1_ctrl.write(1);
                break;

            case 3:                  //cycle 3
                weight_bank1_addr.write(tmp_weight_addr + 12); 
                cout << "cycle3 : gggggg" << endl;
                weight_bank2_addr.write(tmp_weight_addr + 12 + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 12 + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 12 + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 12 + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 12 + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 12 + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 12 + 7 * WEIGHT_SRAM_BANK_SIZE);  
                input_col.write(1);
                stage1_ctrl.write(0);
                cout << "cycle3 : gggggg" << endl;
                break;

            case 4:                  //cycle 4
                weight_bank1_addr.write(tmp_weight_addr); 
                weight_bank2_addr.write(tmp_weight_addr + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 7 * WEIGHT_SRAM_BANK_SIZE);  
                input_col.write(2);
                stage1_ctrl.write(2);
                break;

            case 5:                  //cycle 5
                weight_bank1_addr.write(tmp_weight_addr + 12); 
                weight_bank2_addr.write(tmp_weight_addr + 12 + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 12 + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 12 + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 12 + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 12 + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 12 + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 12 + 7 * WEIGHT_SRAM_BANK_SIZE);  
                input_col.write(2);
                stage1_ctrl.write(1);
                break;

            case 6:                  //cycle 6
                weight_bank1_addr.write(tmp_weight_addr + 24); 
                weight_bank2_addr.write(tmp_weight_addr + 24 + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 24 + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 24 + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 24 + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 24 + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 24 + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 24 + 7 * WEIGHT_SRAM_BANK_SIZE);
                input_col.write(2);
                stage1_ctrl.write(0);
                break;

            case 7:                  //cycle 7
                weight_bank1_addr.write(tmp_weight_addr); 
                weight_bank2_addr.write(tmp_weight_addr + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 7 * WEIGHT_SRAM_BANK_SIZE);  
                input_col.write(3);
                stage1_ctrl.write(3);
                break;

            case 8:                  //cycle 8
                weight_bank1_addr.write(tmp_weight_addr + 12); 
                weight_bank2_addr.write(tmp_weight_addr + 12 + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 12 + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 12 + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 12 + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 12 + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 12 + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 12 + 7 * WEIGHT_SRAM_BANK_SIZE); 
                input_col.write(3);
                stage1_ctrl.write(2);
                stage3_ctrl2.write(0);
                break;           

            case 9:                  //cycle 9
                weight_bank1_addr.write(tmp_weight_addr + 24); 
                weight_bank2_addr.write(tmp_weight_addr + 24 + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 24 + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 24 + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 24 + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 24 + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 24 + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 24 + 7 * WEIGHT_SRAM_BANK_SIZE);
                input_col.write(3);
                stage1_ctrl.write(1);
                stage3_ctrl2.write(4);
                break;

            case 10:                 //cycle 10
                weight_bank1_addr.write(tmp_weight_addr + 12); 
                weight_bank2_addr.write(tmp_weight_addr + 12 + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 12 + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 12 + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 12 + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 12 + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 12 + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 12 + 7 * WEIGHT_SRAM_BANK_SIZE); 
                input_col.write(4);
                stage1_ctrl.write(3);
                break;

            case 11:                 //cycle 11
                weight_bank1_addr.write(tmp_weight_addr + 24); 
                weight_bank2_addr.write(tmp_weight_addr + 24 + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 24 + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 24 + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 24 + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 24 + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 24 + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 24 + 7 * WEIGHT_SRAM_BANK_SIZE);    
                input_col.write(4);
                stage1_ctrl.write(2);
                stage3_ctrl2.write(1);
                break;

            case 12:                 //cycle 12
                weight_bank1_addr.write(tmp_weight_addr + 24); 
                weight_bank2_addr.write(tmp_weight_addr + 24 + 1 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank3_addr.write(tmp_weight_addr + 24 + 2 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank4_addr.write(tmp_weight_addr + 24 + 3 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank5_addr.write(tmp_weight_addr + 24 + 4 * WEIGHT_SRAM_BANK_SIZE); 
                weight_bank6_addr.write(tmp_weight_addr + 24 + 5 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank7_addr.write(tmp_weight_addr + 24 + 6 * WEIGHT_SRAM_BANK_SIZE);
                weight_bank8_addr.write(tmp_weight_addr + 24 + 7 * WEIGHT_SRAM_BANK_SIZE);  
                input_col.write(5);
                stage1_ctrl.write(3);
                stage3_ctrl2.write(4);
                break;

            case 13:                 //cycle 13
                stage3_ctrl2.write(2);
                break;

            case 14:                 //cycle 14
                stage3_ctrl2.write(3);
                break;

            case 15:                 //cycle 15
                stage3_ctrl2.write(4);
                write_to_pe.write(0);
                stage1_rst.write(false);
                break;

            
            default:
                break;
        }
        if(tmp_weight_addr == (WEIGHT_SRAM_SIZE))
            tmp_weight_addr = WEIGHT_SRAM_BASE;
    }   
}
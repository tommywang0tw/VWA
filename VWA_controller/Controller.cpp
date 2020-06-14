#include "Controller.h"

void CONTROLLER::do_Controller(){

// reset
    cycle_count = 0;

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
                input_col.write(0);
                weight_col.write(0);
                stage1_ctrl.write(0);
                stage1_rst.write(true);
                break;

            case 2:                  //cycle 2
                input_col.write(1);
                weight_col.write(0);
                stage1_ctrl.write(1);
                break;

            case 3:                  //cycle 3
                input_col.write(1);
                weight_col.write(1);
                stage1_ctrl.write(0);
                break;

            case 4:                  //cycle 4
                input_col.write(2);
                weight_col.write(0);
                stage1_ctrl.write(2);
                break;

            case 5:                  //cycle 5
                input_col.write(2);
                weight_col.write(1);
                stage1_ctrl.write(1);
                break;

            case 6:                  //cycle 6
                input_col.write(2);
                weight_col.write(2);
                stage1_ctrl.write(0);
                break;

            case 7:                  //cycle 7
                input_col.write(3);
                weight_col.write(0);
                stage1_ctrl.write(3);
                break;

            case 8:                  //cycle 8
                input_col.write(3);
                weight_col.write(1);
                stage1_ctrl.write(2);
                stage3_ctrl2.write(0);
                break;           

            case 9:                  //cycle 9
                input_col.write(3);
                weight_col.write(2);
                stage1_ctrl.write(1);
                stage3_ctrl2.write(4);
                break;

            case 10:                 //cycle 10
                input_col.write(4);
                weight_col.write(1);
                stage1_ctrl.write(3);
                break;

            case 11:                 //cycle 11
                input_col.write(4);
                weight_col.write(2);
                stage1_ctrl.write(2);
                stage3_ctrl2.write(1);
                break;

            case 12:                 //cycle 12
                input_col.write(5);
                weight_col.write(2);
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

            case 16:                 //cycle 16
                stage3_ctrl2.write(4);
                write_to_pe.write(0);
                stage1_rst.write(false);
                cycle_count = 0;
                break;
            

            default:
                break;
        }
    }   
}
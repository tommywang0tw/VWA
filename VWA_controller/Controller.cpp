#include "Controller.h"

#define WEIGHT_DRAM_TO_SRAM 0
#define MOVING_WEIGHT 1
#define INPUT_DRAM_TO_SRAM 2
#define MOVING_INPUT 3
#define DO_CONV 4

void CONTROLLER::do_Controller(){

// reset
    cycle_count = 0;
    tmp_input_addr = INPUT_SRAM_BASE;
    tmp_weight_addr = WEIGHT_SRAM_BASE;
    tmp_dram_input_addr = DRAM4_BASE;
    tmp_dram_weight_addr = DRAM4_WEIGHT_BASE;
    tmp_tile_height = 0;
    tmp_tile_width = 0;
    conv_time = 0;
    input_data_ready = 0;
    tmp_input_ch = 0;
    input_move_count = 0;
    tile_count = 0;

    total_weight = f_size * f_size * input_ch * f_num;
    if(total_weight % (f_size * f_size * 512))
        move_weight_count = 1 + (total_weight /(f_size * f_size * 512));
    else
        move_weight_count = total_weight / (f_size * f_size * 512);

    tmp_move_weight_count = move_weight_count;
    weight_count = total_weight;

    while(1){
        
        wait();
        
        if(start.read())
        {
            cout << "start!!!!!!" << endl;
            out_input_ch.write(input_ch.read());
            out_f_size.write(f_size.read());
        }

        switch (state)
        {

            case INPUT_DRAM_TO_SRAM:
                    
                read_in.write(true);

                source_address.write(tmp_dram_input_addr);
                target_address.write(tmp_input_addr);

                //calculate tile width and tile height
                if(tmp_tile_width + 7 <= input_size)
                    width = 6;
                else
                    width = input_size - tmp_tile_width + 2;  //2 is overlap

                if(tmp_tile_height + 7 <= input_size)
                    height = 7;
                else 
                    height = input_size - tmp_tile_height;
                

                tile_width.write(width);
                tile_height.write(height);
                input_size.write(input_size);
                data_length.write(width * height * input_ch);   //data length = tile width x tile height x input channel

                if(f_size > 0)
                    sram_mode.write(false);     //3 x 3 filter size
                else
                    sram_mode.write(true);      //5 x 5 filter size   

                input_move_count += width * height * input_ch;
                tile_inform[0][tile_count] = tile_width;
                tile_inform[1][tile_count] = tile_height;
                tile_inform[2][tile_count] = tmp_input_addr;
                tile_count++;
                state = MOVING_INPUT;

            break;

            case MOVING_INPUT:

                read_in.write(false);
                if(dma_done)
                {   
                    if(width == 6)
                    {
                        tmp_tile_width += 4;
                        tmp_dram_input_addr += input_size * 4 * 4; //first 4 is 4 column, second 4 is convert to address
                    }
                    else //if width < 6 means one row finish, change to next row and reset tmp_tile_width to 0
                    {
                        tmp_tile_width = 0;
                        tmp_tile_height +=  7;
                        tmp_dram_input_addr = DRAM4_BASE + tmp_tile_height * 4;
                    }

                    tmp_input_addr += width * height * input_ch * 4;

                    if(tmp_dram_input_addr > (DRAM4_BASE + (input_size * input_size * 4)) //all input is finished, go to next state
                        state = WEIGHT_DRAM_TO_SRAM;
                    else if(((INPUT_SRAM_SIZE/4) - input_move_count) < (7 * 6 * input_ch)) //if remain smaller than 1 tile * input_ch, then go to next state_
                        state = WEIGHT_DRAM_TO_SRAM;
                    else  
                        state = INPUT_DRAM_TO_SRAM;
                }

            break;

            case WEIGHT_DRAM_TO_SRAM:

                if(total_weight < (WEIGHT_SRAM_SIZE / 4))           //weights only need to move one time
                {
                    source_address.write(tmp_dram_weight_addr);
                    data_length.write(weight_count);
                    tmp_move_weight_count--;
                    weight_count = 0;     
                }
                else if(tmp_move_weight_count > 1 && (total_weight > (WEIGHT_SRAM_SIZE / 4)))
                {
                    source_address.write(tmp_dram_weight_addr);
                    data_length.write(f_size * f_size * 512);
                    weight_count -= f_size * f_size * 512;
                    tmp_move_weight_count--;
                    tmp_dram_weight_addr += f_size * f_size * 512;
                }
                else if(tmp_move_weight_count == 1 && (total_weight > (WEIGHT_SRAM_SIZE / 4))) 
                {
                    source_address.write(tmp_dram_weight_addr);
                    data_length.write(weight_count);
                    weight_count = total_weight;
                    tmp_move_weight_count == move_weight_count;
                    tmp_dram_weight_addr = DRAM4_WEIGHT_BASE;
                }

                target_address.write(WEIGHT_SRAM_BASE);
                if(f_size > 0)
                    sram_mode.write(false);     //3 x 3 filter size
                else
                    sram_mode.write(true);      //5 x 5 filter size
                
                read_in.write(true);
                state = MOVING_WEIGHT;

            break;

            case MOVING_WEIGHT:

                read_in.write(false);
                if(dma_done)
                    state = DO_CONV;
            
            break;

            case DO_CONV:

                cycle_count++;

                conv_input_time++;
                conv_weight_time++;

                switch (cycle_count)
                {
                    case 1:                  //cycle 1
                        write_to_pe.write(1);
                        for(int i = 0; i < 8; i++)
                            weight_bank[i].write(tmp_weight_addr + i * WEIGHT_SRAM_BANK_SIZE)
                        input_col.write(0);
                        stage1_ctrl.write(0);
                        stage1_rst.write(true);
                        break;

                    case 2:                  //cycle 2
                        for(int i = 0; i < 8; i++)
                            weight_bank[i].write(tmp_weight_addr + i * WEIGHT_SRAM_BANK_SIZE)  
                        input_col.write(1);
                        stage1_ctrl.write(1);
                        break;

                    case 3:                  //cycle 3
                        for(int i = 0; i < 8; i++)
                                weight_bank[i].write(tmp_weight_addr + 12 + i * WEIGHT_SRAM_BANK_SIZE);
                        input_col.write(1);
                        stage1_ctrl.write(0);
                        break;

                    case 4:                  //cycle 4
                        for(int i = 0; i < 8; i++)
                                weight_bank[i].write(tmp_weight_addr + i * WEIGHT_SRAM_BANK_SIZE);
                        input_col.write(2);
                        stage1_ctrl.write(2);
                        break;

                    case 5:                  //cycle 5
                        for(int i = 0; i < 8; i++)
                                weight_bank[i].write(tmp_weight_addr + 12 + i * WEIGHT_SRAM_BANK_SIZE);
                        input_col.write(2);
                        stage1_ctrl.write(1);
                        break;

                    case 6:                  //cycle 6
                        for(int i = 0; i < 8; i++)
                                weight_bank[i].write(tmp_weight_addr + 24 + i * WEIGHT_SRAM_BANK_SIZE);
                        input_col.write(2);
                        stage1_ctrl.write(0);
                        break;

                    case 7:                  //cycle 7
                        for(int i = 0; i < 8; i++)
                                weight_bank[i].write(tmp_weight_addr + i * WEIGHT_SRAM_BANK_SIZE);
                        input_col.write(3);
                        stage1_ctrl.write(3);
                        break;

                    case 8:                  //cycle 8
                        for(int i = 0; i < 8; i++)
                                weight_bank[i].write(tmp_weight_addr + 12 + i * WEIGHT_SRAM_BANK_SIZE);
                        input_col.write(3);
                        stage1_ctrl.write(2);
                        stage3_ctrl2.write(0);
                        break;           

                    case 9:                  //cycle 9
                        for(int i = 0; i < 8; i++)
                                weight_bank[i].write(tmp_weight_addr + 24 + i * WEIGHT_SRAM_BANK_SIZE);
                        input_col.write(3);
                        stage1_ctrl.write(1);
                        stage3_ctrl2.write(4);
                        break;

                    case 10:                 //cycle 10
                        for(int i = 0; i < 8; i++)
                                weight_bank[i].write(tmp_weight_addr + 12 + i * WEIGHT_SRAM_BANK_SIZE);
                        input_col.write(4);
                        stage1_ctrl.write(3);
                        break;

                    case 11:                 //cycle 11
                        for(int i = 0; i < 8; i++)
                                weight_bank[i].write(tmp_weight_addr + 24 + i * WEIGHT_SRAM_BANK_SIZE);
                        input_col.write(4);
                        stage1_ctrl.write(2);
                        stage3_ctrl2.write(1);
                    case 12:                 //cycle 12
                        for(int i = 0; i < 8; i++)
                                weight_bank[i].write(tmp_weight_addr + 24 + i * WEIGHT_SRAM_BANK_SIZE);
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

                //******************************edit*********************************************
                if(conv_input_time == (tile_count * input_ch) / 8 ) //all input in inputSRAM are be used, load next input
                {
                    state = INPUT_DRAM_TO_SRAM;
                    conv_input_time = 0;
                    tile_count = 0;
                    input_move_count = 0;
                    tmp_input_addr  = INPUT_SRAM_BASE;
                }
                else if(conv_weight_time == (total_weight / move_weight_count / (f_size * f_size * 8)))//all weight are be used, load next weight
                {
                    state = WEIGHT_DRAM_TO_SRAM;
                    conv_weight_time = 0;
                }

                //******************************edit*********************************************
                //calculate next input address
                next_input_addr += 7 x 6 x 8

                //calculate next weight address
                tmp_weight_addr += 36;
                if(tmp_weight_addr == (WEIGHT_SRAM_BASE + WEIGHT_SRAM_BANK_SIZE * 4))
                    tmp_weight_addr = WEIGHT_SRAM_BASE;

            break;
                
            default:
                break;      
        }

        //====================================================================
    }   
}
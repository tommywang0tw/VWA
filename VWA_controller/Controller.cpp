#include "Controller.h"
#define INPUT_DRAM_TO_SRAM 0
#define MOVING_INPUT 1
#define WEIGHT_DRAM_TO_SRAM 2
#define MOVING_WEIGHT 3
#define DO_CONV 4

void CONTROLLER::do_Controller()
{

    // reset
    cycle_count = 0;
    tmp_input_addr = INPUT_SRAM_BASE;
    tmp_weight_addr = WEIGHT_SRAM_BASE;
    tmp_dram_input_addr = DRAM4_BASE;
    tmp_dram_weight_addr = DRAM4_WEIGHT_BASE;
    tmp_tile_height = 0;
    tmp_tile_width = 0;
    input_data_ready = 0;
    tmp_input_ch = 0;
    input_move_count = 0;
    tile_count = 0;
    current_tile_col = 1;
    current_tile_row = 1;
    read_addr = 3296; //add 8 each time
    write_addr = 0;   //add 8 each time
    read_boundary.write(0);
    write_boundary.write(0);
    bottom_bad_tile = false;
    right_bad_tile = false;
    state = INPUT_DRAM_TO_SRAM;
    total_weight = f_size.read() * f_size.read() * input_ch.read() * f_num.read();
    if (total_weight % (f_size.read() * f_size.read() * 512))
        move_weight_count = 1 + (total_weight / (f_size.read() * f_size.read() * 512));
    else
        move_weight_count = total_weight / (f_size.read() * f_size.read() * 512);

    tmp_move_weight_count = move_weight_count;
    weight_count = total_weight;

    while (1)
    {
        wait();
        if (start.read())
        {
            //cout << "start!!!!!!" << endl;
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
            if (tmp_tile_width + 6 <= input_size.read())
                width = 6;
            else
                width = input_size.read() - tmp_tile_width + 2; //2 is overlap

            if (tmp_tile_height + 7 <= input_size.read())
                height = 7;
            else
                height = input_size.read() - tmp_tile_height;

            tile_width.write(width);
            tile_height.write(height);
            out_input_size.write(input_size.read());
            data_length.write(width * height * input_ch.read()); //data length = tile width x tile height x input channel

            if (f_size.read() > 0)
                sram_mode.write(false); //3 x 3 filter size
            else
                sram_mode.write(true); //5 x 5 filter size

            input_move_count += width * height * input_ch.read();
            tile_inform[0][tile_count] = tile_width;
            tile_inform[1][tile_count] = tile_height;
            tile_inform[2][tile_count] = tmp_input_addr;
            tile_count++;
            state = MOVING_INPUT;
            break;

        case MOVING_INPUT:
            //cout << "do_MOVING_INPUT" << endl;
            read_in.write(false);
            if (dma_done)
            {
                cout << "width : " << tmp_tile_width << endl;
                cout << "height : " << tmp_tile_height << endl;
                if((width < 6) || (tmp_tile_width + 6 == input_size.read())){  //if width < 6 means one row finish, change to next row and reset tmp_tile_width to 0
                    tmp_tile_width = 0;
                    tmp_tile_height += 7;
                    tmp_dram_input_addr = DRAM4_BASE + tmp_tile_height * 4;
                }
                else
                {
                        tmp_tile_width += 4;
                        tmp_dram_input_addr += input_size.read() * 4 * 4; //first 4 is 4 column, second 4 is convert to address
                }

                tmp_input_addr += width * height * input_ch.read() * 4;
                cout << "input move count : " << input_move_count << endl;
                    if((input_size.read() * input_size.read() * input_ch.read()))
                        state = WEIGHT_DRAM_TO_SRAM;
                    else if(tmp_dram_input_addr > (DRAM4_BASE + (input_size.read() * input_size.read() * 4))) //all input is finished, go to next state
                        state = WEIGHT_DRAM_TO_SRAM;
                    else if(((INPUT_SRAM_SIZE/4) - input_move_count) < (7 * 6 * input_ch.read())) //if remain smaller than 1 tile * input_ch, then go to next state_
                        state = WEIGHT_DRAM_TO_SRAM;
                    else  
                        state = INPUT_DRAM_TO_SRAM;
            }

            break;

        case WEIGHT_DRAM_TO_SRAM:
            cout << "do WEIGHT_DRAM_TO_SRAM" << endl;
            if (total_weight < (WEIGHT_SRAM_SIZE / 4)) //weights only need to move one time
            {
                source_address.write(tmp_dram_weight_addr);
                data_length.write(weight_count);
                tmp_move_weight_count--;
                weight_count = 0;
            }
            else if (tmp_move_weight_count > 1 && (total_weight > (WEIGHT_SRAM_SIZE / 4)))
            {
                source_address.write(tmp_dram_weight_addr);
                data_length.write(f_size.read() * f_size.read()* 512);
                weight_count -= f_size.read() * f_size.read() * 512;
                tmp_move_weight_count--;
                tmp_dram_weight_addr += f_size.read() * f_size.read() * 512;
            }
            else if (tmp_move_weight_count == 1 && (total_weight > (WEIGHT_SRAM_SIZE / 4)))
            {
                source_address.write(tmp_dram_weight_addr);
                data_length.write(weight_count);
                weight_count = total_weight;
                tmp_move_weight_count == move_weight_count;
                tmp_dram_weight_addr = DRAM4_WEIGHT_BASE;
            }

            target_address.write(WEIGHT_SRAM_BASE);
            if (f_size.read() > 0)
                sram_mode.write(false); //3 x 3 filter size
            else
                sram_mode.write(true); //5 x 5 filter size

            read_in.write(true);
            state = MOVING_WEIGHT;

            break;

        case MOVING_WEIGHT:

            read_in.write(false);
            if (dma_done)
                state = DO_CONV;

            break;

        case DO_CONV:
        {   
            cout << "do_Conv" << endl;
            stage3_ctrl1.write(0);
            //handle the right most abnormal tile
            int stop_cycle_count;
            int tile_col = 6;
            if (((input_size.read() - 6) % 4) == 0)
            {
                num_of_tile_col = ((input_size.read() - 6) / 4) + 1;
            }
            else
            {
                right_bad_tile = true;
                num_of_tile_col = ((input_size.read() - 6) / 4) + 2; //including of bad tile
                int bad_tile_col = input_size.read() - ((num_of_tile_col - 2) * 4 + 6) + 2;
                //cout << "bad_tile_col:" << bad_tile_col << endl;
                cout << "current_tile_col" << current_tile_col << endl;
                cout << "num_of_tile_col" << num_of_tile_col << endl;
                if (current_tile_col == num_of_tile_col)
                    tile_col = bad_tile_col;
            }

            //handle the bottom most abnormal tile
            if (input_size.read() % 7 == 0)
                num_of_tile_row = input_size.read() / 7;
            else
            {
                bottom_bad_tile = true;
                num_of_tile_row = input_size.read() / 7 + 1;
            }

            //PE input mux
            if (current_tile_row == num_of_tile_row && bottom_bad_tile == true)
            {
                for (int i = 0; i < 7; i++)
                {
                    if (i >= input_size.read() % 7)
                    {
                        for (int j = 0; j < 8; j++)
                            PE_input_ctrl[j][i].write(false);
                    }
                    else
                    {
                        for (int j = 0; j < 8; j++)
                            PE_input_ctrl[j][i].write(true);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 7; i++)
                    for (int j = 0; j < 8; j++)
                        PE_input_ctrl[j][i].write(true);
            }

            if (start.read())
            {
                //cout << "start!!!!!!" << endl;
                //tt            out_input_ch.write(input_ch.read());
                out_f_size.write(f_size.read());
            }
            cycle_count++;
            // if(cycle_count>)
            //     cycle_count = 0;
            bool endOfTile = false;
            int input_read_col;
            if (stage3_rst_count == ((input_ch.read()) / 8))
            {
                endOfTile = true;
                stage3_ctrl1.write(0);
            }
            else
            {
                stage3_ctrl1.write(1);
                endOfTile = false;
            }

            int num_op_per_tile = input_ch.read()/8;
            int tile_start_col = ( (current_tile_col-1)*num_op_per_tile + (stage3_rst_count-1) + (current_tile_row-1)*num_of_tile_col - 1)*6;
            switch (cycle_count){
            case 1: //cycle 1
                input_read_col = 0;
                inputS_col.write(input_read_col + tile_start_col);
                write_to_pe.write(1);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(0);
                stage1_rst.write(true);
                stage3_rst.write(true);

                /*--boundary dont read/write--*/
                read_boundary.write(0);
                write_boundary.write(0);
                boundary_write_addr.write(write_addr);
                boundary_read_addr.write(read_addr);
                break;

            case 2: //cycle 2
                input_read_col = 1;
                inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + i * WEIGHT_SRAM_BANK_SIZE);
                break;

            case 3: //cycle 3
                inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + 12 + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(1);
                break;

            case 4: //cycle 4
                input_read_col = 2;
                inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(0);
                break;

            case 5: //cycle 5
                inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + 12 + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(2);
                break;

            case 6: //cycle 6
                inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + 24 + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(1);
                break;

            case 7: //cycle 7
                input_read_col = 3;
                if (input_read_col < tile_col)
                    inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(0);
                break;

            case 8: //cycle 8
                if (input_read_col < tile_col)
                    inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + 12 + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(3);
                if (endOfTile) //boundary read
                    read_boundary.write(1);
                break;

            case 9: //cycle 9
                if (input_read_col < tile_col)
                    inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + 24 + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(2);
                stage3_ctrl2.write(0);
                if (endOfTile)
                { //increment read addr
                    read_boundary.write(0);
                    read_addr += 8;
                    boundary_read_addr.write(read_addr);
                }
                break;

            case 10: //cycle 10
                input_read_col = 4;
                if (input_read_col < tile_col)
                    inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + 12 + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(1);
                stage3_ctrl2.write(4);
                if (endOfTile) //boundary write
                {
                    write_boundary.write(1);
                }
                break;

            case 11: //cycle 11
                if (input_read_col < tile_col)
                    inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + 24 + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(3);

                if (endOfTile) //increment write addr and boundary read
                {
                    read_boundary.write(1);
                    write_boundary.write(0);
                    write_addr += 8;
                    boundary_write_addr.write(write_addr);
                }
                break;

            case 12: //cycle 12
                input_read_col = 5;
                if (input_read_col <= tile_col)
                    inputS_col.write(input_read_col + tile_start_col);
                for(int i = 0; i < 8; i++)
                    weight_bank_addr[i].write(tmp_weight_addr + 24 + i * WEIGHT_SRAM_BANK_SIZE);
                stage1_ctrl.write(2);
                stage3_ctrl2.write(1);
                if (endOfTile) //increment read addr
                {
                    read_boundary.write(0);
                    read_addr += 8;
                    boundary_read_addr.write(read_addr);
                }
                break;

            case 13: //cycle 13
                stage1_ctrl.write(3);
                stage3_ctrl2.write(4);
                if (endOfTile) //boundary write and read
                {
                    read_boundary.write(1);
                    write_boundary.write(1);
                }
                break;

            case 14: //cycle 14
                stage3_ctrl2.write(2);
                if (endOfTile) // boundary read and increment write addr
                {
                    read_boundary.write(1);
                    write_boundary.write(0);
                    write_addr += 8;
                    boundary_write_addr.write(write_addr);
                    read_addr += 8;
                    boundary_read_addr.write(read_addr);
                }
                break;

            case 15: //cycle 15
                stage3_ctrl2.write(3);
                write_to_pe.write(0);
                if (endOfTile)
                { //boundary write and read and increment read addr
                    write_boundary.write(1);
                    read_boundary.write(0);
                    if (read_addr >= 6584)
                        read_addr = 0;
                    else
                        read_addr += 8;
                    boundary_read_addr.write(read_addr);
                }
                break;

            case 16: //cycle 16
                stage3_ctrl2.write(4);
                write_to_pe.write(0);
                if (endOfTile)
                { //boundary write and increment read and write addr
                    write_boundary.write(1);
                    write_addr += 8;
                    boundary_write_addr.write(write_addr);
                }
                break;

            case 17:
                cycle_count = 0;
                conv_input_time++;
                conv_weight_time++;
                if (endOfTile)
                {
                    /*--boundary dont read/write--*/
                    read_boundary.write(0);
                    write_boundary.write(0);

                    if (write_addr >= 6584)
                        write_addr = 0;
                    else
                        write_addr += 8;

                    if (current_tile_col == num_of_tile_col)
                    { //go to next row...
                        if (read_addr > write_addr)
                        {
                            read_addr = 0;
                            write_addr = B_SRAM_BANK_SIZE;
                        }
                        else
                        {
                            read_addr = B_SRAM_BANK_SIZE;
                            write_addr = 0;
                        }
                        current_tile_row++;
                        cout << "current tile row: " << current_tile_row << endl;
                        current_tile_col = 1;
                    }
                    else
                    {
                        current_tile_col++;
                    }
                }

                stage3_ctrl2.write(4);
                stage1_rst.write(false);
                if (input_ch.read() < 8)
                    stage3_rst.write(false);
                else
                {
                    if (stage3_rst_count == ((input_ch.read()) / 8))
                    { //reset stage3_rst
                        stage3_rst.write(false);
                        stage3_rst_count = 1;
                    }
                    else
                    {
                        stage3_rst.write(true);
                        stage3_rst_count++;
                    }
                }
                break;

            default:
                break;
            }

            //******************************edit*********************************************
            if (conv_input_time == ((tile_count * input_ch.read()) / 8 * (f_num.read())) )//all input in inputSRAM are already used, load next input
            {
                state = INPUT_DRAM_TO_SRAM;
                conv_input_time = 0;
                tile_count = 0;
                input_move_count = 0;
                tmp_input_addr = INPUT_SRAM_BASE;
            }
            else if (conv_weight_time == (total_weight / move_weight_count / (f_size.read() * f_size.read() * 8))) //all weight are already used, load next weight
            {
                state = WEIGHT_DRAM_TO_SRAM;
                conv_weight_time = 0;
            }

                //calculate next weight address
                tmp_weight_addr += 36;
            if (tmp_weight_addr == (WEIGHT_SRAM_BASE + WEIGHT_SRAM_BANK_SIZE * 4))
                tmp_weight_addr = WEIGHT_SRAM_BASE;
            
        }   break;
        
        default:
            break;
        }
        //====================================================================
    }
}
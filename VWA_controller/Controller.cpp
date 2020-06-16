#include "Controller.h"

void CONTROLLER::do_Controller()
{

    // reset
    cycle_count = 0;
    stage3_rst_count = 1;
    current_tile_col = 1;
    current_tile_row = 1;
    read_addr = 3296; //add 8 each time
    write_addr = 0;   //add 8 each time
    read_boundary.write(0);
    write_boundary.write(0);
    bottom_bad_tile = false;
    right_bad_tile = false;

    while (1)
    {
        wait();
        stage3_ctrl1.write(0);
        //handle the right most abnormal tile
        int stop_cycle_count;
        int tile_col = 6;
        if (((in_image_col.read() - 6) % 4) == 0)
        {
            num_of_tile_col = ((in_image_col.read() - 6) / 4) + 1;
        }
        else
        {
            right_bad_tile = true;
            num_of_tile_col = ((in_image_col.read() - 6) / 4) + 2; //including of bad tile
            int bad_tile_col = in_image_col.read() - ((num_of_tile_col - 2) * 4 + 6) + 2;
            //cout << "bad_tile_col:" << bad_tile_col << endl;
            cout << "current_tile_col" << current_tile_col << endl;
            cout << "num_of_tile_col" << num_of_tile_col << endl;
            if (current_tile_col == num_of_tile_col)
                tile_col = bad_tile_col;
        }

        //handle the bottom most abnormal tile
        if (in_image_row.read() % 7 == 0)
            num_of_tile_row = in_image_row.read() / 7;
        else
        {
            bottom_bad_tile = true;
            num_of_tile_row = in_image_row.read() / 7 + 1;
        }

        //PE input mux
        if (current_tile_row == num_of_tile_row && bottom_bad_tile == true)
        {
            for (int i = 0; i < 7; i++)
            {
                if (i >= in_image_row.read() % 7)
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
        } else {
            stage3_ctrl1.write(1);
            endOfTile = false;
        }
        switch (cycle_count)
        {
        case 1: //cycle 1
            input_read_col = 0;
            inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_ch.write(0 + (stage3_rst_count - 1) * 8);
            write_to_pe.write(1);
            weight_col.write(0);
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
            inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(0);
            break;

        case 3: //cycle 3
            inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(1);
            stage1_ctrl.write(1);
            break;

        case 4: //cycle 4
            input_read_col = 2;
            inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(0);
            stage1_ctrl.write(0);
            break;

        case 5: //cycle 5
            inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(1);
            stage1_ctrl.write(2);
            break;

        case 6: //cycle 6
            inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(2);
            stage1_ctrl.write(1);
            break;

        case 7: //cycle 7
            input_read_col = 3;
            if (input_read_col < tile_col)
                inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(0);
            stage1_ctrl.write(0);
            break;

        case 8: //cycle 8
            if (input_read_col < tile_col)
                inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(1);
            stage1_ctrl.write(3);
            if (endOfTile) //boundary read
                read_boundary.write(1);
            break;

        case 9: //cycle 9
            if (input_read_col < tile_col)
                inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(2);
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
                inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(1);
            stage1_ctrl.write(1);
            stage3_ctrl2.write(4);
            if (endOfTile) //boundary write
            {
                write_boundary.write(1);
            }
            break;

        case 11: //cycle 11
            if (input_read_col < tile_col)
                inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(2);
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
                inputS_col.write(input_read_col + (current_tile_col - 1) * 4);
            inputS_row.write(0 + (current_tile_row - 1) * 7);
            inputS_ch.write(0 + (stage3_rst_count - 1) * 8);
            weight_col.write(2);
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
    }
}
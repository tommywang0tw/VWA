#include "Accumulator_Third.h"

void ACCUMULATOR_THIRD::run_Accumulator()
{
    //reset
    // for(int i = 0; i < BUFFER_NUM; i++) {
    //     input_regs[i] = 0;
    // }
    for (int i = 0; i < (BUFFER_NUM); i++)
    {
        if (i < (BUFFER_NUM - 2))
            output_P[i].write(0);
        else
            output_S[i - 7].write(0);
        for (int j = 0; j < T_SH_REG_NUM; j++)
        {
            shift_regs[i][j] = 0;
        }
    }

    while (1)
    {

        wait();
        //read input to input registers
        // for(int i=0; i<BUFFER_NUM; i++) {
        //     input_regs[i].write(input_T[i].read());
        // }

        if (ctrl_2.read() != 4)
            output_index_reg.write(ctrl_2.read());

        switch (ctrl_1.read())
        {       //The first mux
        case 0: //Add with SRAM
            for (int i = 0; i < BUFFER_NUM; i++)
            {
                if (ctrl_2.read() != 4)
                {
                    if (i < 2)
                    {
                        if (input_S[0].read() == 48400)
                        {
                            cout << "inputT[0]:" << input_T[0] << endl;
                            cout << "shift_regs[0]: " << shift_regs[i][ctrl_2.read()].read() << endl;
                            cout << "sum: " << input_T[i].read() + input_S[i].read() + shift_regs[i][ctrl_2.read()].read() << endl;
                        }
                        shift_regs[i][ctrl_2.read()].write(input_T[i].read() + input_S[i].read() + shift_regs[i][ctrl_2.read()].read());
                    }
                    else
                    {
                        if (i >= 7) // i=7, 8
                            output_S[i - 7].write(input_T[i].read() + shift_regs[i][ctrl_2.read()].read());
                        shift_regs[i][ctrl_2.read()].write(input_T[i].read() + shift_regs[i][ctrl_2.read()].read());
                    }
                }
            }
        break;
        case 1: //Add without SRAM
            for (int i = 0; i < BUFFER_NUM; i++)
            {
                if (ctrl_2.read() != 4)
                {
                    if (i >= 7) // i=7, 8
                        output_S[i - 7].write(input_T[i].read() + shift_regs[i][ctrl_2.read()].read());
                    shift_regs[i][ctrl_2.read()].write(shift_regs[i][ctrl_2.read()].read() + input_T[i].read());
                }
            }
        break;
        default:
            //not possible
            break;
        }

        //write output register to output ports
        for (int i = 0; i < (BUFFER_NUM - 2); i++)
        {
            output_P[i].write(shift_regs[i][output_index_reg.read()].read());
        }
    }
}
#include "Accumulator_First.h"

void ACCUMULATOR_FIRST::run_Accumulator(){
    //reset
    tail = 0;
    for(int i = 0; i < BUFFER_NUM; i++) {
        //input_regs[i] = 0;
        output_regs[i] = 0;
        for(int j = 0; j < SH_REG_NUM; j++) {
            shift_regs[i][j] = 0;
        }
    }

    while (1) {
        wait();
        //read input to input registers
        // for(int i=0; i<BUFFER_NUM; i++) {
        //     input_regs[i].write(input[i].read());
        //     std::cout << input[i] << " gg" << input_regs[i].read() << sc_time_stamp() << endl;
        // }

        sr_index_reg.write(sr_index.read());
        ctrl_reg.write(ctrl_stage1.read());
        //add input registers with one of the shift registers chosen by ctrl
        for(int i=0; i<BUFFER_NUM; i++) {
            int out = shift_regs[i][ctrl_reg.read()].read() + input[i].read();
            //std:cout << "out" << out <<endl;
            output_regs[i].write( out );
            shift_regs[i][sr_index_reg.read()].write(out);
        }
        
        //write output register to output ports
        for(int i=0; i<BUFFER_NUM; i++) {
            output[i].write( output_regs[i].read() );
        }

    }


}
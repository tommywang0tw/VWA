#include "Accumulator_Third.h"

void ACCUMULATOR_THIRD::run_Accumulator(){
    //reset
    // for(int i = 0; i < BUFFER_NUM; i++) {
    //     input_regs[i] = 0;
    // }
    for(int i=0; i< (BUFFER_NUM); i++) {
        for(int j=0; j<T_SH_REG_NUM; j++) {
            shift_regs[i][j] = 0;
        }
    }

    while (1) {

        wait();
        //read input to input registers
        // for(int i=0; i<BUFFER_NUM; i++) {
        //     input_regs[i].write(input_T[i].read());
        // }

        ctrl1_reg.write( ctrl_1.read() );
        ctrl2_reg.write( ctrl_2.read() );
        output_index_reg.write( ctrl2_reg.read());

        switch (ctrl_1.read()) {  //The first mux
            case 0:  //Add with SRAM
                for(int i=0; i<BUFFER_NUM; i++) {
                    if(i<2)
                        shift_regs[i][ctrl2_reg.read()].write(input_T[i].read() + input_S[i].read() + shift_regs[i][ctrl2_reg.read()].read());
                    else
                        shift_regs[i][ctrl2_reg.read()].write(input_T[i].read() + shift_regs[i][ctrl_2.read()].read());
                }
            case 1:  //Add without SRAM
                for(int i=0; i<BUFFER_NUM; i++) {
                    shift_regs[i][ctrl2_reg.read()].write( shift_regs[i][ctrl2_reg.read()].read() + input_T[i].read() );
                }
            default:
                //not possible
                break;
        }

        //write output register to output ports
        for(int i=0; i< (BUFFER_NUM-2) ; i++) {
            output_P[i].write( shift_regs[i][output_index_reg.read()].read() );
        }
    }
}
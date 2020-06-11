#include "Accumulator_Third.h"

void ACCUMULATOR_THIRD::run_Accumulator(){
    //reset
    tail = 0;
    for(int i = 0; i < BUFFER_NUM; i++) {
        input_regs[i] = 0;
    }
    for(int i=0; i< (BUFFER_NUM); i++) {
        for(int j=0; j<SH_REG_NUM; j++) {
            shift_regs[i][j] = 0;
        }
    }

    while (1) {

        wait();
        //read input to input registers
        for(int i=0; i<BUFFER_NUM; i++) {
            input_regs[i].write(input_T[i].read());
        }

        switch (ctrl_1.read()) {  //The first mux
            case 0:
                for(int i=0; i<BUFFER_NUM; i++) {
                    if(i<2)
                        shift_regs[i][tail].write(input_regs[i].read() + input_S[i].read());
                    else
                        shift_regs[i][tail].write(input_regs[i].read());
                }
            case 1:
                for(int i=0; i<BUFFER_NUM; i++) {
                    shift_regs[i][tail].write( shift_regs[i][ctrl_2.read()].read() + input_regs[i].read() );
                }
            default:
                //not possible
                break;
        }

        //write output register to output ports
        for(int i=0; i< (BUFFER_NUM-2) ; i++) {
            output_P[i].write( shift_regs[i][SH_REG_NUM-1].read() );
        }

        if(tail == 2) {
            tail = 0;
        } else {
            tail++;
        }
    }


}
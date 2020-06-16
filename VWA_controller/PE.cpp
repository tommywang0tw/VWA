#include "PE.h"

void PE::do_PE(){

    int PE_input_value[7];
    for(int i=0; i<7;i++) {
        if(input_ctrl[i].read())
            PE_input_value[i] = input[i].read();
        else 
            PE_input_value[i] = 0;
    }

    /* buffer 0 */
    buffer[0].write(PE_input_value[0] * weight[2].read());

    /* buffer 1 */
    buffer[1].write((PE_input_value[0] * weight[1].read()) + (PE_input_value[1] * weight[2].read())); 
    
    /* buffer 2 */
    buffer[2].write((PE_input_value[0] * weight[0].read()) + (PE_input_value[1] * weight[1].read()) + (PE_input_value[2] * weight[2].read()));

    /* buffer 3 */
    buffer[3].write((PE_input_value[1] * weight[0].read()) + (PE_input_value[2] * weight[1].read()) + (PE_input_value[3] * weight[2].read()));

    /* buffer 4 */
    buffer[4].write((PE_input_value[2] * weight[0].read()) + (PE_input_value[3] * weight[1].read()) + (PE_input_value[4] * weight[2].read()));

    /* buffer 5 */
    buffer[5].write((PE_input_value[3] * weight[0].read()) + (PE_input_value[4] * weight[1].read()) + (PE_input_value[5] * weight[2].read()));

    /* buffer 6 */
    buffer[6].write((PE_input_value[4] * weight[0].read()) + (PE_input_value[5] * weight[1].read()) + (PE_input_value[6] * weight[2].read()));

    /* buffer 7 */
    buffer[7].write((PE_input_value[5] * weight[0].read()) + (PE_input_value[6] * weight[1].read()));

    /* buffer 8 */
    buffer[8].write(PE_input_value[6] * weight[0].read());

}
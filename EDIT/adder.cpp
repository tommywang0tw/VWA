#include "adder.h"

void ADDER::do_ADD(){

    out_data.write(input[0].read() + input[1].read() + 
                    input[2].read() + input[3].read() + 
                    input[4].read() + input[5].read() + 
                    input[6].read() + input[7].read() + 
                    input[8].read());
    cout << "input[0] : " << input[0].read() << " time : " << sc_time_stamp() << endl;
    cout << "out_data : " << out_data.read() << " time : " << sc_time_stamp() << endl;
}
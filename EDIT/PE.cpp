#include "PE.h"

void PE::do_PE(){

    /* buffer 0 */
    buffer[0].write(input[0].read() * weight[2].read());
    cout << "input[0] : " << input[0].read() << " time : " << sc_time_stamp() << endl;
    cout << "buffer[0] : " << buffer[0].read() << " time : " << sc_time_stamp() << endl;
    /* buffer 1 */
    buffer[1].write((input[0].read() * weight[1].read()) + (input[1].read() * weight[2].read())); 
    
    /* buffer 2 */
    buffer[2].write((input[0].read() * weight[0].read()) + (input[1].read() * weight[1].read()) + (input[2].read() * weight[2].read()));

    /* buffer 3 */
    buffer[3].write((input[1].read() * weight[0].read()) + (input[2].read() * weight[1].read()) + (input[3].read() * weight[2].read()));

    /* buffer 4 */
    buffer[4].write((input[2].read() * weight[0].read()) + (input[3].read() * weight[1].read()) + (input[4].read() * weight[2].read()));

    /* buffer 5 */
    buffer[5].write((input[3].read() * weight[0].read()) + (input[4].read() * weight[1].read()) + (input[5].read() * weight[2].read()));

    /* buffer 6 */
    buffer[6].write((input[4].read() * weight[0].read()) + (input[5].read() * weight[1].read()) + (input[6].read() * weight[2].read()));

    /* buffer 7 */
    buffer[7].write((input[5].read() * weight[0].read()) + (input[6].read() * weight[1].read()));

    /* buffer 8 */
    buffer[8].write(input[6].read() * weight[0].read());

}
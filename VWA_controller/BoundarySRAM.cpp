#include "BoundarySRAM.h"

void BoundarySRAM::do_BoundarySRAM(){

// reset
write_row=0;
read_row=0;
    
    while(1){

        wait();

        if(write_address.read()<3296){
            write_row = 0;
            write_row_cal = write_address.read()/4;
        }
        else{
            write_row = 1;
            write_row_cal = (write_address.read()-3296)/4;
        }


        if(read_address.read()<3296){
            read_row = 0;
            read_row_cal = read_address.read()/4;
        }
        else{
            
            read_row = 1;
            read_row_cal = (read_address.read()-3296)/4;
        }


        if(w.read()){
            if(r.read()){ //write ==1 and read ==1
                //wrte data

                boundary[write_row][write_row_cal] = in_data[0].read();
                boundary[write_row][write_row_cal+1] = in_data[1].read();
                

                //read data
                out_data[0].write(boundary[read_row][read_row_cal]);
                out_data[1].write(boundary[read_row][read_row_cal+1]);

            }
            else{ //write ==1 and read ==0
                //wrte data
                boundary[write_row][write_row_cal] = in_data[0].read();
                boundary[write_row][write_row_cal+1] = in_data[1].read();
            }
        }
        else{
            if(r.read()){ //write ==0 and read ==1
                //read data
                out_data[0].write(boundary[read_row][read_row_cal]);
                out_data[1].write(boundary[read_row][read_row_cal+1]);
            }
            else{ //write ==0 and read ==0
                //do nothing 
            }
        }

    
    }
}
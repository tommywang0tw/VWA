
#ifndef TREEADDER_H
#define TREEADDER_H

#include "systemc.h"

SC_MODULE(TreeAdder) {


    sc_in<sc_uint<32> > input[9][8]; 

/* ============================================================
            Output Ports
    ============================================================
*/
    sc_out<sc_uint<32> > output[9];

    void run_TreeAdder();
    
    SC_CTOR(TreeAdder){
        
        SC_METHOD(run_TreeAdder);
        for (int i = 0; i < 9; i++){
          for (int j = 0; j < 8; j++)
            sensitive << input[i][j];
        }
    }
};

#endif
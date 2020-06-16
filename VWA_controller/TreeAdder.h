
#ifndef TREEADDER_H
#define TREEADDER_H

#include "systemc.h"

SC_MODULE(TreeAdder) {


    sc_in<sc_int<32> > input[8][9]; 

/* ============================================================
            Output Ports
    ============================================================
*/
    sc_out<sc_int<32> > output[9];

    void run_TreeAdder();
    
    SC_CTOR(TreeAdder){
        
        SC_METHOD(run_TreeAdder);
        for (int i = 0; i < 8; i++){
          for (int j = 0; j < 9; j++)
            sensitive << input[i][j];
        }
    }
};

#endif
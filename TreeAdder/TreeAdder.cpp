#include "TreeAdder.h"

void TreeAdder::run_TreeAdder(){
    for(int i=0; i<9; i++) {
        int sum = 0;
        for(int j=0; j<8; j++) {
            sum = sum + input[i][j].read();
        }
        output[i].write(sum);
    }
}
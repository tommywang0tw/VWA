#include "Postprocess.h"

int Relu(int result) {
    return result = result < 0 ? 0 : result;
}

void Postprocess::run_Postprocess(){
    int result[7] = {0};
    for(int i = 0; i < 7; i++) {
        result[i]=Relu(input[i].read()+bias.read());
        std::cout << "input[" << i << "]=" << input[i] << std::endl;
        output[i].write(result[i]);
    }
    
}
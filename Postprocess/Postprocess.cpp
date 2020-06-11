#include "Postprocess.h"


void Postprocess::run_Postprocess(){
    int sum = 0;
    for(int i=0; i< 7; i++) {
        sum = sum + input[i].read();
        std::cout << "input[" << i << "]=" << input[i] << std::endl;
    }
    std::cout << "sum before " << sum << std::endl;
    sum = sum < 0 ? 0 : sum;
    sum = sum + bias.read();
    std::cout << "sum after " << sum << std::endl;
    output.write(sum);
}
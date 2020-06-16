#include <iomanip>
#include <iostream>

#define INPUT_SRAM_BASE (0x50000000)
#define WEIGHT_SRAM_BASE (0x51000000) //Weight SRAM size 3 x 3 x 512 x 4(bytes) = 18432bytes (0x4800)
//#define WEIGHT_SRAM2_BASE (WEIGHT_SRAM1_BASE + WEIGHT_SRAM_SIZE) //0x51000000 + 18432bytes(= 0x4800) = 51004800bytes

#define INPUT_SRAM_SIZE (0x15000)              //7 x 384 x 8 x 4 = 86016bytes(= 0x15000)
#define WEIGHT_SRAM_SIZE (0X4800)
#define WEIGHT_SRAM_BANK_SIZE (0x900)



#define INPUT_SRAM_BANK (8)
#define WEIGHT_SRAM_BANK (8)
#define INPUT_SRAM_BANK_BITS (3)
#define WEIGHT_SRAM_BASE_BITS (3)
#define TILE_WIDTH (6) 
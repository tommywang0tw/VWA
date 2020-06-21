#include <iomanip>
#include <iostream>

//====================================================
// Simulation Platform
//====================================================

	#ifndef TIME_SCALE
	#define TIME_SCALE (SC_NS)
	#endif

	#ifndef CLK_CYCLE
	#define CLK_CYCLE (5.0)
	#endif

	// Note: When you set TIME_SCALE as SC_NS, then
	// CLK_CYCLE = (5.0) 200MHz, (2.5) 400MhHz, (1.66) 600MHz, (1.25) 800Mhz, (1.0) 1000MHz, (0.833) 1200Mhz
	// Make sure that CLC_CYCLE is a float type value.

//====================================================
// Memory Subsystem
//====================================================
// DRAM Configuration

#define ADDR_LENGTH (32) 
#define DATA_LENGTH (32) 
#define DATA_BUS_BANDWIDTH (32)        //Configuable

//~~~~~~~~
//  DRAM
//~~~~~~~~
#define MEM_SIZE (0x00700000) //MEM_SIZE (0x2000000)bytes,Configuable, uint32_t mem[MEMSIZE] 


#define ROW_BIT (11)
#define COLUMN_BIT (10)
#define BANK_BIT (8)

#ifdef FAST_SIM
	#define tRP (1)
	#define tRCD (1)
	#define tCL (1)
	#define tRAS (1)
	#define magic_delay (1)
#else  // using DDR4-2400
	#define tRP (0)
	#define tRCD (0)
	#define tCL (0)
	#define tRAS (0)
	#define magic_delay (0) // DRAM bus cycle time (ns) (0.8333)
#endif

//****************
#define DRAM4_ID (4)
#define DRAM4_BASE (0X00000000)
#define DRAM4_WEIGHT_BASE (0X00300000)
#define DRAM4_SIZE (MEM_SIZE)

#define DRIVER_READ (0)
#define DRIVER_WRITE (1)

#define INPUT_SRAM_BASE (0x50000000)
#define WEIGHT_SRAM_BASE (0x51000000)



#define INPUT_SRAM_SIZE (0x15000)              //7 x 384 x 8 x 4 = 86016bytes(= 0x15000)
#define WEIGHT_SRAM_SIZE (0X4800)
#define WEIGHT_SRAM_BANK_SIZE (0x900)



#define INPUT_SRAM_BANK (8)
#define WEIGHT_SRAM_BANK (8)
#define INPUT_SRAM_BANK_BITS (3)
#define WEIGHT_SRAM_BASE_BITS (3)
#define TILE_WIDTH (6) 

#include <iomanip>
#include <systemc.h>

#ifndef _GENERAL_FUNC
#define _GENERAL_FUNC
	void print_module(char const a[]);
	void print_module(char const a[], unsigned int SM_ID);

	union Data
	{
		bool		b1;
		uint16_t	f16;
		int32_t		s32;
		uint32_t	u32;
		float		f32;
		int64_t		s64;
		uint64_t	u64;
		double		f64;
	};
#endif
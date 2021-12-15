#ifndef __COMMON__
#define __COMMON__

#include <stdio.h>
#include <stdint.h>

// 64 kB memory
#define MEM_SIZE 0x1<<16

// Interrupt Vector Table
#define INTERRUPT_VEC_TBL_BASE 0x1000

// System Call Vector Table
// 0x10 SC Vectors * 0x04 Bytes/Handler = 0x40 Vector Table Region
#define SC_VEC_TBL_BASE 0x2000

#ifdef DEBUG
    #define dprintf(...) printf(__VA_ARGS__)
#else
    #define dprintf(...) 
#endif

#endif

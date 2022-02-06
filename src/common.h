#ifndef __COMMON__
#define __COMMON__

#include <stdio.h>
#include <stdint.h>

#define READ_WORD(base_addr) (mem[base_addr] << 24) | (mem[base_addr + 1] << 16) | (mem[base_addr + 2] << 8) | mem[base_addr + 3]

#define READ_HALFWORD(base_addr) (mem[base_addr + 2] << 8) | mem[base_addr + 3]

#define READ_BYTE(base_addr) mem[base_addr]

#define WRITE_WORD(base_addr, val32) \
    mem[base_addr] = (val32 >> 24) & 0x0ff; \
    mem[base_addr + 1] = (val32 >> 16) & 0x0ff; \
    mem[base_addr + 2] = (val32 >> 8) & 0x0ff; \
    mem[base_addr + 3] = val32 & 0x0ff;

#define WRITE_HALFWORD(base_addr, val16) \
    mem[base_addr + 2] = (val16 >> 8) & 0x0ff; \
    mem[base_addr + 3] = val16 & 0x0ff;

#define WRITE_BYTE(base_addr, val8) \
    mem[base_addr + 3] = val8 & 0x0ff;

#define GPR_FILE_SIZE 16

// 64 kB memory
#define MEM_SIZE 0x1<<16

// System Call Vector Table
// 0x10 SC Vectors * 0x04 Bytes/Handler_Address = 0x40 Vector Table Region
#define SC_VEC_TBL_BASE 0x100

// Interrupt Vector Table
#define INTERRUPT_VEC_TBL_BASE 0x200

#ifdef DEBUG
    #define dprintf(...) printf(__VA_ARGS__)
#else
    #define dprintf(...) 
#endif

#endif

#ifndef __INSTRUCTION_SET__
#define __INSTRUCTION_SET__

#include "common.h"

enum opcodes {
// memory access
    LD_REG,     // 0
    LD_IMM,     // 1
    ST_REG,     // 2
    ST_IMM,     // 3
// arithmetic
    ADD_REG,    // 4
    ADD_IMM,    // 5
    MUL_REG,    // 6
    MUL_IMM,    // 7
// bitwise logical
    AND_REG,    // 8 
    AND_IMM,    // 9 
    OR_REG,     // 10
    OR_IMM,     // 11
// bitwise
    SHIFTR_REG, // 12
    SHIFTR_IMM, // 13
    SHIFTL_REG, // 14
    SHIFTL_IMM, // 15
// program flow
    BR,         // 16
    SYS_CALL,   // 17
    INVALID
};

enum data_sizes {
    BYTE = 1,
    HALFWORD = 2,
    WORD = 3
};

enum condition_codes {
    UC = 0,
    EQ = 1, 
    NEQ = 2, 
    GT = 3, 
    LT = 4
};

uint32_t and(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t data_size);

uint32_t or(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t data_size);

uint32_t shiftl(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t data_size);

uint32_t shiftr(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t data_size);

uint32_t add(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t data_size);

uint32_t multiply(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t data_size);

uint32_t load(uint32_t address, uint8_t *mem, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags);

void store(uint32_t address, uint8_t *mem, uint32_t data, uint32_t data_size);

void branch(uint32_t condition_code, uint32_t address, uint32_t link, uint32_t msr, uint32_t *ip, uint32_t *lr);

void sys_call(uint32_t sc_vector, uint32_t *ip, uint32_t *lr, uint8_t *mem); 

#endif

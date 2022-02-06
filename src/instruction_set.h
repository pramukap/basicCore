#ifndef __INSTRUCTION_SET__
#define __INSTRUCTION_SET__

#include "common.h"

#define PRIV_MODE 0x1
#define USER_MODE 0x0

enum opcodes {
// memory access
    LD_REG = 1,     
    LD_IMM = 2,     
    ST_REG = 3,     
    ST_IMM = 4,     
// arithmetic
    ADD_REG = 5,    
    ADD_IMM = 6,    
    MUL_REG = 7,    
    MUL_IMM = 8,    
// bitwise logical
    AND_REG = 9,        
    AND_IMM = 10,       
    OR_REG = 11,        
    OR_IMM = 12,        
// bitwise
    SHIFTR_REG = 13, 
    SHIFTR_IMM = 14, 
    SHIFTL_REG = 15, 
    SHIFTL_IMM = 16, 
// program flow
    BR = 17,         
    SYS_CALL = 18,   
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

void set_cond_flags(uint32_t data, uint32_t signed_bit, uint32_t overflow_flag, uint32_t *msr);

uint32_t and(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags);

uint32_t or(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags);

uint32_t shiftl(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags);

uint32_t shiftr(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags);

uint32_t add(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags);

uint32_t multiply(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags);

uint32_t load(uint32_t address, uint8_t *mem, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags);

void store(uint32_t address, uint8_t *mem, uint32_t data, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags);

void branch(uint32_t condition_code, uint32_t address, uint32_t link, uint32_t ret, uint32_t priv_to_user, 
            uint32_t *msr, uint32_t *ip, uint32_t *lr, uint32_t *usp, uint32_t *pidr, 
            uint8_t *mem, uint32_t *gpr_file);

void sys_call(uint32_t sc_vector, uint32_t *msr, uint32_t *ip, uint32_t *lr, uint32_t *usp, uint32_t *pidr, uint8_t *mem, uint32_t *gpr_file);

#endif

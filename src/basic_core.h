#ifndef __BASIC_CORE__
#define __BASIC_CORE__

#include "common.h"

typedef struct core_state_type {
    uint32_t ip;
    uint32_t ir;
    uint32_t lr;
    uint32_t usp;
    uint32_t psp;
    uint32_t msr;
    uint32_t pidr;
    uint32_t gpr_file[GPR_FILE_SIZE];
} core_state;

uint32_t core_fetch(uint32_t *ip, uint8_t *mem); 

uint32_t core_decode_execute_complete(core_state *basic_core, uint8_t *mem);

#endif

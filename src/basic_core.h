#ifndef __BASIC_CORE__
#define __BASIC_CORE__

#include "common.h"

typedef struct core_state_type {
    uint32_t ip;    // instruction pointer
    uint32_t ir;    // instruction register
    uint32_t lr;    // link register
    uint32_t usp;   // user-mode stack pointer
    uint32_t psp;   // priv-mode stack pointer
    uint32_t msr;   // machine state register
    uint32_t pidr;  // process id register
    uint32_t gpr_file[GPR_FILE_SIZE];
} core_state;

uint32_t core_fetch(uint32_t *ip, uint8_t *mem); 

uint32_t core_decode_execute_complete(core_state *basic_core, uint8_t *mem);

#endif

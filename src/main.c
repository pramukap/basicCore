#include "instruction_set.h"
#include "basic_core.h"

uint8_t mem[MEM_SIZE] = {0};

core_state basic_core;

int main() {
    // init
    basic_core.ip = 0x0;
    basic_core.ir = 0x0;
    basic_core.lr = 0x0;
    basic_core.sp = 0x0;
    basic_core.msr = 0x0;
    basic_core.pidr = 0x0;
    
    for (int i = 0; i < GPR_FILE_SIZE; i++) {
        basic_core.gpr_file[i] = 0x0;
    }

    // insert program here

    printf("Initial state:\n");
    for (int i = 0; i < GPR_FILE_SIZE; i++) {
        printf("GPR %i: 0x%08X\n", i, basic_core.gpr_file[i]);
    }
    printf("-----\n");

    // run core
    int num_steps = 20;
    int i = 0;
    while (i < num_steps) {
        basic_core.ir = core_fetch(&basic_core.ip, mem);
        core_decode_execute_complete(&basic_core, mem); 

        printf("Step %d:\n", i + 1);
        printf("IR: 0x%08X\n", basic_core.ir);
        printf("IP: 0x%08X\n", basic_core.ip);
        printf("LR: 0x%08X\n", basic_core.lr);
        printf("MSR: 0x%08X\n", basic_core.msr);
        for (int i = 0; i < GPR_FILE_SIZE; i++) {
            printf("GPR %i: 0x%08X\n", i, basic_core.gpr_file[i]);
        }
        printf("-----\n");

        i++;
    }    
    return 0;
}


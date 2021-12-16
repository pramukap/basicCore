#include "instruction_set.h"
#include "basic_core.h"

uint8_t mem[MEM_SIZE] = {0};

core_state basic_core;

void print_context(core_state core, int curr_step) {
        printf("Cycle %d:\n", curr_step);
        
        printf("\tIP: 0x%08X ", core.ip);
        printf("IR: 0x%08X ", core.ir);
        printf("LR: 0x%08X ", core.lr);
        printf("MSR: 0x%08X\n\t", core.msr);

        for (int i = 0; i < GPR_FILE_SIZE; i++) {
            printf("GPR %02i: 0x%08X ", i, core.gpr_file[i]);
            if (i % 4 == 3) {
                printf("\n\t");
            }
        }
        printf("-----\n");
}

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
    print_context(basic_core, -1);

    // run core
    int num_steps = 20;
    int i = 0;
    while (i < num_steps) {
        basic_core.ir = core_fetch(&basic_core.ip, mem);
        core_decode_execute_complete(&basic_core, mem); 

        print_context(basic_core, i);

        i++;
    }    
    return 0;
}


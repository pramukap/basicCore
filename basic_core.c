#include "basic_core.h"
#include "instruction_set.h"

uint32_t core_fetch(uint32_t *ip, uint8_t *mem) {
    uint32_t instr = (mem[*ip] << 24) |  (mem[*ip + 1] << 16) | (mem[*ip + 2] << 8) | (mem[*ip + 3] << 0);
    *ip += 4;

    dprintf("addr: 0x%08x, instruction: 0x%08x\n", (*ip) - 4, instr);
    
    return instr;
}

uint32_t core_decode_execute_complete(core_state *basic_core, uint8_t *mem) {
    uint32_t ir, *msr, *ip, *lr, *gpr_file;
    uint32_t opcode, regA, regB, regC, imm12, imm4, imm2, imm1, link; 
    uint32_t addr, data, data_size, op1, op2, condition_code, set_condition_flags;
    uint32_t result;

    ir = basic_core->ir;
    msr = &(basic_core->msr);
    ip = &(basic_core->ip);
    lr = &(basic_core->lr);
    gpr_file = basic_core->gpr_file;
    
    opcode = (ir >> (31 - 4)) & 0x1f;
    regA = (ir >> (31 - 11)) & 0x0f;
    regB = (ir >> (31 - 19)) & 0x0f;
    regC = (ir >> (31 - 27)) & 0x0f;
    imm12 = ir & 0x0fff;
    imm4 = (ir >> (31 - 19)) & 0x0f;
    imm2 = (ir >> (31 - 15)) & 0x03;
    imm1 = (ir >> (31 - 6)) & 0x01;
    link = (ir >> (31 - 15)) & 0x01;

    // sign-extend imm12
    if (imm12 >> 11) {
        imm12 |= (~imm12 & ~0x0fff);
    }

    dprintf("opcode: %d\n", opcode);

    switch (opcode) {
        case LD_REG:
            // check for valid addr and trigger exception
            if (regB == 0) {
                addr = 0x0 + gpr_file[regC]; 
            } else {
                addr = gpr_file[regB] + gpr_file[regC]; 
            }
            data_size = imm2;
            set_condition_flags = imm1;

            result = load(addr, mem, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case ST_REG:
            // check for valid addr and trigger exception
            if (regB == 0) {
                addr = 0x0 + gpr_file[regC]; 
            } else {
                addr = gpr_file[regB] + gpr_file[regC]; 
            }
            data_size = imm2;
            set_condition_flags = imm1;

            store(addr, mem, gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case LD_IMM:
            // check for valid addr and trigger exception
            if (regB == 0) {
                addr = *ip + imm12; 
            } else {
                addr = gpr_file[regB] + imm12;
            }
            data_size = imm2;
            set_condition_flags = imm1;

            dprintf("Address: 0x%08x\n", addr);
            dprintf("Destination reg: %d\n", regA);

            result = load(addr, mem, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case ST_IMM:
            // check for valid addr and trigger exception
            if (regB == 0) {
                addr = *ip + imm12; 
            } else {
                addr = gpr_file[regB] + imm12;
            }
            data_size = imm2;
            set_condition_flags = imm1;

            dprintf("Address: 0x%08x\n", addr);
            dprintf("Destination reg: %d\n", regA);

            store(addr, mem, gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case ADD_REG:
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = gpr_file[regC];
            data_size = imm2;
            set_condition_flags = imm1;

            result = add(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case MUL_REG:
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = gpr_file[regC];
            data_size = imm2;
            set_condition_flags = imm1;

            result = multiply(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case ADD_IMM:
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = imm12;
            data_size = imm2;
            set_condition_flags = imm1;

            result = add(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case MUL_IMM:
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = imm12;
            data_size = imm2;
            set_condition_flags = imm1;

            result = multiply(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case AND_REG:    
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = gpr_file[regC];
            data_size = imm2;
            set_condition_flags = imm1;

            result = and(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case AND_IMM:    
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = imm12;
            data_size = imm2;
            set_condition_flags = imm1;

            result = and(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case OR_REG:    
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = gpr_file[regC];
            data_size = imm2;
            set_condition_flags = imm1;

            result = or(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case OR_IMM:    
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = imm12;
            data_size = imm2;
            set_condition_flags = imm1;

            result = or(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case SHIFTR_REG: 
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = gpr_file[regC];
            data_size = imm2;
            set_condition_flags = imm1;

            result = shiftr(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case SHIFTR_IMM: 
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = imm12;
            data_size = imm2;
            set_condition_flags = imm1;

            result = shiftr(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case SHIFTL_REG: 
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = gpr_file[regC];
            data_size = imm2;
            set_condition_flags = imm1;

            result = shiftl(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case SHIFTL_IMM: 
            if (regB == 0) {
                op1 = 0;
            } else {
                op1 = gpr_file[regB];
            } 
            op2 = imm12;
            data_size = imm2;
            set_condition_flags = imm1;

            result = shiftl(op1, op2, &gpr_file[regA], msr, data_size, set_condition_flags);
            break;

        case BR:
            condition_code = imm4;
            if (regA == 0) {
                addr = *ip + imm12;
            } else {
                addr = gpr_file[regA] + imm12;
            }
            branch(condition_code, addr, link, *msr, ip, lr);
            break;

        case SYS_CALL:
            break;

        default:
            // trigger invalid instruction exception
            ;
    }
}


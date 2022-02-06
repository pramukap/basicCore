#include "instruction_set.h"

void set_cond_flags(uint32_t data, uint32_t signed_bit, uint32_t overflow_flag, uint32_t *msr) {
    uint32_t condition_flags = 0; 

    // zero
    if (data == 0) { 
        condition_flags |= (1 << 4);
    // neg
    } else if (signed_bit) {
        condition_flags |= (1 << 3);
    // pos
    } else {
        condition_flags |= (1 << 2);
    }
    
    if (overflow_flag != -1) {
        condition_flags |= (overflow_flag << 1);
    }
    
    *msr &= ~(0x1f << (31 - 16));
    *msr |= ((condition_flags & 0x1f) << (31 - 16));
}


uint32_t and(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags) {
    uint64_t result;
    uint32_t signed_bit = 0;
    uint32_t overflow_flag = 0;

    switch (data_size) {
        case BYTE:
            result = (uint8_t)(op1 & 0x0ff) & (uint8_t)(op2 & 0x0ff);
            signed_bit = (result >> 7) & 0x01;
            overflow_flag = ((result >> 8) > 0);
            result &= 0x0ff;
            break;

        case HALFWORD:
            result = (uint16_t)(op1 & 0x0ffff) & (uint16_t)(op2 & 0x0ffff);
            signed_bit = (result >> 15) & 0x01;
            overflow_flag = ((result >> 16) > 0);
            result &= 0x0ffff;
            break;

        case WORD:
            result = (op1 & 0x0ffffffff) & (op2 & 0x0ffffffff); 
            signed_bit = (result >> 31) & 0x01;
            overflow_flag = ((result >> 32) > 0);
            result &= 0x0ffffffff;
            break;

        default:
            // trigger exception
            ;
    }

    if (set_condition_flags) {
        set_cond_flags((uint32_t)result, signed_bit, overflow_flag, msr);
    }

    *gpr = (uint32_t)result;

    return (uint32_t)result;     
}

uint32_t or(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags) {
    uint64_t result;
    uint32_t signed_bit = 0;
    uint32_t overflow_flag = 0;

    switch (data_size) {
        case BYTE:
            result = (uint8_t)(op1 & 0x0ff) | (uint8_t)(op2 & 0x0ff);
            signed_bit = (result >> 7) & 0x01;
            overflow_flag = ((result >> 8) > 0);
            result &= 0x0ff;
            break;

        case HALFWORD:
            result = (uint16_t)(op1 & 0x0ffff) | (uint16_t)(op2 & 0x0ffff);
            signed_bit = (result >> 15) & 0x01;
            overflow_flag = ((result >> 16) > 0);
            result &= 0x0ffff;
            break;

        case WORD:
            result = (op1 & 0x0ffffffff) | (op2 & 0x0ffffffff); 
            signed_bit = (result >> 31) & 0x01;
            overflow_flag = ((result >> 32) > 0);
            result &= 0x0ffffffff;
            break;

        default:
            // trigger exception
            ;
    }

    if (set_condition_flags) {
        set_cond_flags((uint32_t)result, signed_bit, overflow_flag, msr);
    }

    *gpr = result;

    return (uint32_t)result;     
}

uint32_t shiftl(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags) {
    uint64_t result;
    uint32_t signed_bit = 0;
    uint32_t overflow_flag = 0;

    switch (data_size) {
        case BYTE:
            result = (uint8_t)(op1 & 0x0ff) << (uint8_t)(op2 & 0x0ff);
            signed_bit = (result >> 7) & 0x01;
            overflow_flag = ((result >> 8) > 0);
            result &= 0x0ff;
            break;

        case HALFWORD:
            result = (uint16_t)(op1 & 0x0ffff) << (uint16_t)(op2 & 0x0ffff);
            signed_bit = (result >> 15) & 0x01;
            overflow_flag = ((result >> 16) > 0);
            result &= 0x0ffff;
            break;

        case WORD:
            result = (op1 & 0x0ffffffff) << (op2 & 0x0ffffffff); 
            signed_bit = (result >> 31) & 0x01;
            overflow_flag = ((result >> 32) > 0);
            result &= 0x0ffffffff;
            break;

        default:
            // trigger exception
            ;
    }

    if (set_condition_flags) {
        set_cond_flags((uint32_t)result, signed_bit, overflow_flag, msr);
    }

    *gpr = (uint32_t)result;

    return (uint32_t)result;     
}

uint32_t shiftr(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags) {
    uint64_t result;
    uint32_t signed_bit = 0;
    uint32_t overflow_flag = 0;

    switch (data_size) {
        case BYTE:
            result = (uint8_t)(op1 & 0x0ff) >> (uint8_t)(op2 & 0x0ff);
            signed_bit = (result >> 7) & 0x01;
            overflow_flag = ((result >> 8) > 0);
            result &= 0x0ff;
            break;

        case HALFWORD:
            result = (uint16_t)(op1 & 0x0ffff) >> (uint16_t)(op2 & 0x0ffff);
            signed_bit = (result >> 15) & 0x01;
            overflow_flag = ((result >> 16) > 0);
            result &= 0x0ffff;
            break;

        case WORD:
            result = (op1 & 0x0ffffffff) >> (op2 & 0x0ffffffff); 
            signed_bit = (result >> 31) & 0x01;
            overflow_flag = ((result >> 32) > 0);
            result &= 0x0ffffffff;
            break;

        default:
            // trigger exception
            ;
    }

    if (set_condition_flags) {
        set_cond_flags((uint32_t)result, signed_bit, overflow_flag, msr);
    }

    *gpr = (uint32_t)result;

    return (uint32_t)result;     
}

uint32_t add(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags) {
    uint64_t sum;
    uint32_t signed_bit = 0;
    uint32_t overflow_flag = 0;

    switch (data_size) {
        case BYTE:
            sum = (uint8_t)(op1 & 0x0ff) + (uint8_t)(op2 & 0x0ff);
            signed_bit = (sum >> 7) & 0x01;
            overflow_flag = ((sum >> 8) > 0);
            sum &= 0x0ff;
            break;

        case HALFWORD:
            sum = (uint16_t)(op1 & 0x0ffff) + (uint16_t)(op2 & 0x0ffff);
            signed_bit = (sum >> 15) & 0x01;
            overflow_flag = ((sum >> 16) > 0);
            sum &= 0x0ffff;
            break;

        case WORD:
            sum = op1 + op2; 
            signed_bit = (sum >> 31) & 0x01;
            overflow_flag = ((sum >> 32) > 0);
            sum &= 0x0ffffffff;
            break;

        default:
            // trigger exception
            ;
    }

    if (set_condition_flags) {
        set_cond_flags((uint32_t)sum, signed_bit, overflow_flag, msr);
    }

    *gpr = (uint32_t)sum;

    return (uint32_t)sum;     
}

uint32_t multiply(uint32_t op1, uint32_t op2, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags) {
    uint64_t product;
    uint32_t signed_bit = 0;
    uint32_t overflow_flag = 0;

    switch (data_size) {
        case BYTE:
            product = (uint8_t)(op1 & 0x0ff) * (uint8_t)(op2 & 0x0ff);
            signed_bit = (product >> 7) & 0x01;
            overflow_flag = ((product >> 8) > 0);
            product &= 0x0ff;
            break;

        case HALFWORD:
            product = (uint16_t)(op1 & 0x0ffff) * (uint16_t)(op2 & 0x0ffff);
            signed_bit = (product >> 15) & 0x01;
            overflow_flag = ((product >> 16) > 0);
            product &= 0x0ffff;
            break;

        // check for overflow
        case WORD:
            product = op1 * op2; 
            signed_bit = (product >> 31) & 0x01;
            overflow_flag = ((product >> 32) > 0);
            product &= 0x0ffffffff;
            break;

        default:
            // trigger exception
            ;
    }

    if (set_condition_flags) {
        set_cond_flags((uint32_t)product, signed_bit, overflow_flag, msr);
    }

    *gpr = (uint32_t)product;

    return (uint32_t)product;     
}

uint32_t load(uint32_t address, uint8_t *mem, uint32_t *gpr, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags) {
    uint32_t data = 0;
    uint32_t signed_bit = 0;

    switch (data_size) {
        case BYTE:
            data = mem[address];
            signed_bit = data >> 7;
            break;

        case HALFWORD:
            data |= (mem[address] << 8);
            data |= mem[address + 1];
            signed_bit = data >> 15;
            break;

        case WORD:
            data |= (mem[address] << 24);
            data |= (mem[address + 1] << 16);
            data |= (mem[address + 2] << 8);
            data |= mem[address + 3];
            signed_bit = data >> 31;
            break;

        default:
            // trigger exception
            ;
    }
    
    if (set_condition_flags) {
        set_cond_flags(data, signed_bit, -1, msr);
    }

    printf("load msr: 0x%08x\n", *msr);

    *gpr = data; 

    return data;
}

void store(uint32_t address, uint8_t *mem, uint32_t data, uint32_t *msr, uint32_t data_size, uint32_t set_condition_flags) {
    uint32_t signed_bit = 0;

    switch (data_size) {
        case BYTE:
            mem[address] = (uint8_t)(data & 0x0ff);
            signed_bit = data >> 7;
            break;

        case HALFWORD:
            mem[address] = (uint8_t)((data >> 8) & 0x0ff);
            mem[address + 1] = (uint8_t)(data & 0x0ff);
            signed_bit = data >> 15;
            break;

        case WORD:
            mem[address] = (uint8_t)((data >> 24) & 0x0ff);
            mem[address + 1] = (uint8_t)((data >> 16) & 0x0ff);
            mem[address + 2] = (uint8_t)((data >> 8) & 0x0ff);
            mem[address + 3] = (uint8_t)(data & 0x0ff);
            signed_bit = data >> 31;
            break;

        default:
            // trigger exception
            ;
    }
    
    if (set_condition_flags) {
        set_cond_flags(data, signed_bit, -1, msr);
    }


    return; 
}

void branch(uint32_t condition_code, uint32_t address, uint32_t link, uint32_t ret, uint32_t priv_to_user, 
            uint32_t *msr, uint32_t *ip, uint32_t *lr, uint32_t *usp, uint32_t *pidr, 
            uint8_t *mem, uint32_t *gpr_file) {
    uint32_t eq_flag = (*msr >> (31 - 12)) & 0x01;
    uint32_t neg_flag = (*msr >> (31 - 13)) & 0x01;
    uint32_t pos_flag = (*msr >> (31 - 14)) & 0x01;
    uint32_t of_flag = (*msr >> (31 - 15)) & 0x01;

    if (link) {
       *lr = *ip; 
    }

    switch (condition_code) {
        case UC:
            if (ret) {
                *ip = *lr;
            } else {
                *ip = address;
            }
            break;
        case EQ:
            if (eq_flag && !(neg_flag || pos_flag || of_flag)) {
                if (ret) {
                    *ip = *lr;
                } else {
                    *ip = address;
                }
            }
            break;
        case NEQ:
            if (!eq_flag && (neg_flag || pos_flag || of_flag)) {
                if (ret) {
                    *ip = *lr;
                } else {
                    *ip = address;
                }
            }
            break;
        case GT:
            // result was positive, or the result was so positive that it became negative
            if ((pos_flag && !(eq_flag || neg_flag || of_flag)) 
            || ((neg_flag && of_flag) && !(eq_flag || pos_flag))) {
                if (ret) {
                    *ip = *lr;
                } else {
                    *ip = address;
                }
            }
            break;
        case LT:
            // result was negative, or the result was so negative that it became positive
            if ((neg_flag && !(eq_flag || pos_flag || of_flag)) 
            || ((pos_flag && of_flag) && !(eq_flag || neg_flag)) ) {
                if (ret) {
                    *ip = *lr;
                } else {
                    *ip = address;
                }
            }
            break;
        default:
            break;
            // invalid condition code error
    }

    // pop user-state off of user-stack
    if (priv_to_user) {
        if (((*msr >> (31 - 1)) & 0x1) == PRIV_MODE) {
            *usp += 4;
            *msr = READ_WORD(*usp);
            *usp += 4;
            *lr =  READ_WORD(*usp);
            *usp += 4;
            *pidr =  READ_WORD(*usp);
            
            for (int i = GPR_FILE_SIZE - 1; i >= 0; i--) {
                *usp += 4;
                gpr_file[i] =  READ_WORD(*usp);
            }

            // ensure msr is set to user mode
            *msr &= ~(0x3 << (31 - 1)); 
            *msr |= (USER_MODE << (31 - 1)); 
        } // TODO: else trigger exception
    }
}

void sys_call(uint32_t sc_vector, uint32_t *msr, uint32_t *ip, uint32_t *lr, uint32_t *usp, uint32_t *pidr, uint8_t *mem, uint32_t *gpr_file) {
    // each vector is an index into the sc vector table 
    uint32_t sc_vec_table_offset = sc_vector << 2;

    // push user-state to user-stack
    for (int i = 0; i < GPR_FILE_SIZE; i++){
        WRITE_WORD(*usp, gpr_file[i]);
        *usp -= 4;
    }
    WRITE_WORD(*usp, *pidr);
    *usp -= 4;
    WRITE_WORD(*usp, *lr);
    *usp -= 4;
    WRITE_WORD(*usp, *msr);
    *usp -= 4;
    
    // save return address
    *lr = *ip;
    // switch to supervisor mode
    *msr &= ~(0x3 << (31 - 1)); 
    *msr |= PRIV_MODE << (31 - 1);

    // the vector table contains the start address of the sc vector handler
    *ip = (uint32_t)(READ_WORD(SC_VEC_TBL_BASE + sc_vec_table_offset));
}


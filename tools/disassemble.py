#! /usr/bin/env python2.7

import sys, getopt, re

# indirect/register-offset instructions (opcode regA, regB, regC)
def disasm_form1(instr):
    opcode = instr >> (31 - 4)
    regA = (instr >> (31 - 11)) & 0x0f
    regB = (instr >> (31 - 19)) & 0x0f
    regC = (instr >> (31 - 27)) & 0x0f
    set_cond_flags = (instr >> (31 - 6)) & 0x01 
    data_size = (instr >> (31 - 15)) & 0x03 

    # print("opcode: 0x%02x" % opcode)
    asm_instr = ""
    if (opcode == 0x1):
        asm_instr += "ld." 
    elif (opcode == 0x3):
        asm_instr += "st." 
    elif (opcode == 0x5):
        asm_instr += "add."
    elif (opcode == 0x7):
        asm_instr += "mul." 
    elif (opcode == 0x9):
        asm_instr += "and." 
    elif (opcode == 0xb):
        asm_instr += "or." 
    elif (opcode == 0xd):
        asm_instr += "shr." 
    elif (opcode == 0xf):
        asm_instr += "shl." 

    if (data_size == 1):
        asm_instr += "b" 
    elif (data_size == 2):
        asm_instr += "h" 
    elif (data_size == 3):
        asm_instr += "w" 

    if (set_cond_flags == 1):
        asm_instr += "s" 
        
    asm_instr += (" r" + str(regA) + ", r" + str(regB) + ", r" + str(regC))

    return asm_instr

# immediate-offset instructions (opcode regA, regB, imm12)
def disasm_form2(instr):
    opcode = instr >> (31 - 4)
    regA = (instr >> (31 - 11)) & 0x0f
    regB = (instr >> (31 - 19)) & 0x0f
    imm12 = (instr >> (31 - 31)) & 0x0fff
    set_cond_flags = (instr >> (31 - 6)) & 0x01 
    data_size = (instr >> (31 - 15)) & 0x03 

    # print("opcode: 0x%02x" % opcode)
    asm_instr = ""
    if (opcode == 0x2):
        asm_instr += "ld." 
    elif (opcode == 0x4):
       asm_instr += "st." 
    elif (opcode == 0x6):
       asm_instr += "add." 
    elif (opcode == 0x8):
       asm_instr += "mul." 
    elif (opcode == 0xa):
       asm_instr += "and." 
    elif (opcode == 0xc):
       asm_instr += "or." 
    elif (opcode == 0xe):
       asm_instr += "shr." 
    elif (opcode == 0x10):
       asm_instr += "shl." 

    if (data_size == 1):
       asm_instr += "b" 
    elif (data_size == 2):
       asm_instr += "h" 
    elif (data_size == 3):
       asm_instr += "w" 

    if (set_cond_flags == 1):
       asm_instr += "s" 
        
    asm_instr += (" r" + str(regA) + ", r" + str(regB) + ", " + hex(imm12))

    return asm_instr

# branch
def disasm_form3(instr):
    opcode = instr >> (31 - 4)
    regA = (instr >> (31 - 11)) & 0x0f
    imm12 = (instr >> (31 - 31)) & 0x0fff
    uncond_br = (instr >> (31 - 6)) & 0x01 
    link = (instr >> (31 - 15)) & 0x01 
    cond_code = (instr >> (31 - 19)) & 0x0f 

    # print("opcode: 0x%02x" % opcode)
    asm_instr = ""
    if (opcode == 0x11):
        asm_instr += "b." 

    if (link == 1):
        asm_instr += "l"     

    if (cond_code == 0):
        asm_instr += "u" 
    elif (cond_code == 1):
        asm_instr += "e" 
    elif (cond_code == 2):
        asm_instr += "ne" 
    elif (cond_code == 3):
        asm_instr += "gt" 
    elif (cond_code == 4):
        asm_instr += "lt" 
        
    asm_instr += (" r" + str(regA) + ", " + hex(imm12))

    return asm_instr

# system-call
def disasm_form4(instr):
    opcode = instr >> (31 - 4)
    sc_vector = (instr >> (31 - 19)) & 0x0f 

    asm_instr = ""
    if (opcode == 0x12):
        asm_instr += "sc" 

    asm_instr += (" " + hex(sc_vector))

    return asm_instr

print("Basic Core Disassembler")

input_file = ''

# get args
try:
    opts, args = getopt.getopt(sys.argv[1:], "hi:")
except getopt.error as err:
    print(str(err))
    print("Usage: disassemble.py -i <input_file>")
    sys.exit()

if len(opts) == 0:
    print("Error: No args provided")
    print("Usage: disassemble.py -i <input_file>")
    sys.exit()

for opt, arg in opts:
    if opt == "-h":
        print("Usage: disassemble.py -i <input_file>")
        sys.exit()
    elif opt == "-i":
        input_file = arg


mc_file = open(input_file, 'r')
mach_code = mc_file.readlines()
mc_file.close()

mc_type = ''
mc_data = []

# first pass
# collect hints
i = 0
while i < len(mach_code):
    line = mach_code[i]

    if line[0] != '\n' or line != '':
        if "//" in line:
            line = re.sub("//\s*", "", line)
            line = re.sub("\n", "", line)
            mc_type = line
        else:
            try:
                line = re.sub("mem\[0x", "", line)
                line = re.sub("\] = 0x", " ", line)
                line = re.sub(";\n", "", line)
                addr, data = line.split()
                mc_data.append((mc_type, int(addr, 16), int(data, 16)))
            except ValueError:
                print("Bad line: \"%s\"" % mach_code[i])

    i += 1

#for data in mc_data:
#    print("%s, 0x%04x 0x%02x" % data) 

# pass 2
# process machine code and generate disassembly output
addr = 0
i = 0
while i < len(mc_data):
    if (mc_data[i][1] != addr):
        addr = mc_data[i][1] 

    if ("//" in mc_data[i][0]):
        pass
    elif ("byte" in mc_data[i][0]):
        byte = mc_data[i][2]
        print("0x%04x: .byte 0x%02x" % (addr, byte)) 
        i += 1
    elif ("hword" in mc_data[i][0]):
        hword = (mc_data[i][2] << 8) | mc_data[i + 1][2]
        print("0x%04x: .hword 0x%04x" % (addr, hword)) 
        i += 2
    elif ("word" in mc_data[i][0]):
        word = (mc_data[i][2] << 24) | (mc_data[i + 1][2] << 16) | (mc_data[i + 2][2] << 8) | mc_data[i + 3][2]
        print("0x%04x: .word 0x%08x" % (addr, word)) 
        i += 4
    elif ("instr" in mc_data[i][0]):
        instr = (mc_data[i][2] << 24) | (mc_data[i + 1][2] << 16) | (mc_data[i + 2][2] << 8) | mc_data[i + 3][2]
        opcode = instr >> (31 - 4)

        if (opcode == 18):
            asm_instr = disasm_form4(instr)            
        elif (opcode == 17):
            asm_instr = disasm_form3(instr)            
        elif ((opcode % 2) == 0 and (opcode >= 2 and opcode <= 16)):
            asm_instr = disasm_form2(instr)            
        elif ((opcode % 2) == 1 and (opcode >= 1 and opcode <= 15)):
            asm_instr = disasm_form1(instr)            

        print("0x%04x: %s (0x%08x)" % (addr, asm_instr, instr)) 
        i += 4
    

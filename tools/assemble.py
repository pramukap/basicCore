#! /usr/bin/python2.7

import sys, getopt, re

# indirect/register-offset instructions (opcode regA, regB, regC)
def assemble_form1(asm_tokens):
    mc_instr = 0x0

    opcode = asm_tokens[0].split('.')[0]
    data_size = asm_tokens[0].split('.')[1][0]    
    if ('s' in asm_tokens[0].split('.')[1]):
        set_cond = 1
    else:
        set_cond = 0
    regA = int(asm_tokens[1][1:-1], 10)
    regB = int(asm_tokens[2][1:-1], 10)
    regC = int(asm_tokens[3][1:], 10)

#    print("opcode: %s, data_size: %s, set_cond: %d, regA: %s, regB: %s, regC: %s" % (opcode, data_size, set_cond, regA, regB, regC))

    if ('ld' in opcode):
        mc_instr |= (0x0 << (31 - 4))
    elif ('st' in  opcode):
        mc_instr |= (0x2 << (31 - 4))
    elif ('add' in  opcode):
        mc_instr |= (0x4 << (31 - 4))
    elif ('mul' in  opcode):
        mc_instr |= (0x6 << (31 - 4))
    elif ('and' in  opcode):
        mc_instr |= (0x8 << (31 - 4))
    elif ('or' in  opcode):
        mc_instr |= (0xa << (31 - 4))
    elif ('shr' in  opcode):
        mc_instr |= (0xc << (31 - 4))
    elif ('shl' in  opcode):
        mc_instr |= (0xe << (31 - 4))

    if ('b' in data_size):
        mc_instr |= (0x1 << (31 - 15))
    elif ('h' in data_size):
        mc_instr |= (0x2 << (31 - 15))
    elif ('w' in data_size):
        mc_instr |= (0x3 << (31 - 15))
    
    if set_cond:
        mc_instr |= (0x1 << (31 - 6))

    mc_instr |= (regA << (31 - 11))
    mc_instr |= (regB << (31 - 19))
    mc_instr |= (regC << (31 - 27))

#    print("mc_instr: 0x%08x" % mc_instr)

    return mc_instr

# immediate-offset instructions (opcode regA, regB, imm12)
def assemble_form2(asm_tokens, curr_addr, label_table):
    mc_instr = 0x0

    opcode = asm_tokens[0].split('.')[0]
    data_size = asm_tokens[0].split('.')[1][0]    
    if ('s' in asm_tokens[0].split('.')[1]):
        set_cond = 1
    else:
        set_cond = 0
    regA = int(asm_tokens[1][1:-1], 10)

    # no-label pseudo-op
    if (len(asm_tokens) == 4):
        regB = int(asm_tokens[2][1:-1], 10)
        if (re.search('^-?0b', asm_tokens[3])):
            imm12 = int(re.sub("0b", "", asm_tokens[3]), 2)
        elif (re.search('^-?0d', asm_tokens[3])):
            imm12 = int(re.sub("0d", "", asm_tokens[3]), 10)
        elif (re.search('^-?0x', asm_tokens[3])):
            imm12 = int(re.sub("0x", "", asm_tokens[3]), 16)
        label = 'N/A'
    # label pseudo-op
    else:
        regB = 0
        label = asm_tokens[2]
        imm12 = 'N/A'

#    print("opcode: %s, data_size: %s, set_cond: %d, regA: %s, regB: %s, imm12: %s, label: %s" % (opcode, data_size, set_cond, regA, regB, imm12, label))

    if ('ld' in opcode):
        mc_instr |= (0x1 << (31 - 4))
    elif ('st' in  opcode):
        mc_instr |= (0x3 << (31 - 4))
    elif ('add' in  opcode):
        mc_instr |= (0x5 << (31 - 4))
    elif ('mul' in  opcode):
        mc_instr |= (0x7 << (31 - 4))
    elif ('and' in  opcode):
        mc_instr |= (0x9 << (31 - 4))
    elif ('or' in  opcode):
        mc_instr |= (0xb << (31 - 4))
    elif ('shr' in  opcode):
        mc_instr |= (0xd << (31 - 4))
    elif ('shl' in  opcode):
        mc_instr |= (0xf << (31 - 4))

    if ('b' in data_size):
        mc_instr |= (0x1 << (31 - 15))
    elif ('h' in data_size):
        mc_instr |= (0x2 << (31 - 15))
    elif ('w' in data_size):
        mc_instr |= (0x3 << (31 - 15))
    
    if set_cond:
        mc_instr |= (0x1 << (31 - 6))

    mc_instr |= (regA << (31 - 11))
    mc_instr |= (regB << (31 - 19))

    if label == 'N/A':
        mc_instr |= ((imm12 & 0x0fff) << (31 - 31))
    else:
        # check if valid label
        if (label_table[label] == None):
            # error
            print("check valid label error")
        else:
            label_addr = label_table[label]
            offset = label_addr - curr_addr
        
        # check if offset fits in 12 bits
        if (offset > 0):
            if (offset < 0x0800): 
                mc_instr |= (offset << (31 - 31))
            else:
                # error
                print("offset fit error")
        else:
            if (offset >= (0x800 * -1)): 
                offset &= 0xfff
                mc_instr |= (offset << (31 - 31))
            else:
                # error
                print("offset fit error")

#    print("mc_instr: 0x%08x" % mc_instr)

    return mc_instr

# branch
def assemble_form3(asm_tokens, curr_addr, label_table):
    mc_instr = 0x0

    opcode = asm_tokens[0].split('.')[0]
#    if (re.search('l[uengl]?', asm_tokens[0].split('.')[1])):
    if (re.search('l(?!t)', asm_tokens[0].split('.')[1])):
        link = 1
    else:
        link = 0
    
    if ('b' in opcode):
        mc_instr |= (0x10 << (31 - 4))

    if (len(asm_tokens) == 3):
        if (re.search('^-?0b', asm_tokens[2])):
            regA = int(asm_tokens[1][1:-1], 10)
            imm12 = int(re.sub("0b", "", asm_tokens[2]), 2)
            label = 'N/A'
        elif (re.search('^-?0d', asm_tokens[2])):
            regA = int(asm_tokens[1][1:-1], 10)
            imm12 = int(re.sub("0d", "", asm_tokens[2]), 10)
            label = 'N/A'
        elif (re.search('^-?0x', asm_tokens[2])):
            regA = int(asm_tokens[1][1:-1], 10)
            imm12 = int(re.sub("0x", "", asm_tokens[2]), 16)
            label = 'N/A'
    else:
        regA = 0
        label = asm_tokens[1]
        imm12 = 'N/A'

    link_and_cond = asm_tokens[0].split('.')[1] 

#    print("opcode: %s, link: %d, link_and_cond: %s, regA: %s, imm12: %s, label: %s" % (opcode, link, link_and_cond, regA, imm12, label))

    # unconditional
    if ('u' in asm_tokens[0].split('.')[1]):
        mc_instr |= (0x1 << (31-6))
        mc_instr &= ~(0x0f << (31-19))
    # unequal
    elif ('ne' in asm_tokens[0].split('.')[1]):
        mc_instr &= ~(0x1 << (31-6))
        mc_instr &= ~(0x0f << (31-19))
        mc_instr |= (0x2 << (31-19))
    # equal
    elif ('e' in asm_tokens[0].split('.')[1]):
        mc_instr &= ~(0x1 << (31-6))
        mc_instr &= ~(0x0f << (31-19))
        mc_instr |= (0x1 << (31-19))
    # greater than
    elif ('gt' in asm_tokens[0].split('.')[1]):
        mc_instr &= ~(0x1 << (31-6))
        mc_instr &= ~(0x0f << (31-19))
        mc_instr |= (0x3 << (31-19))
    # less than
    elif ('lt' in asm_tokens[0].split('.')[1]):
        mc_instr &= ~(0x1 << (31-6))
        mc_instr &= ~(0x0f << (31-19))
        mc_instr |= (0x4 << (31-19))

    if (link):
        mc_instr |= (0x1 << (31-15))
    else:
        mc_instr &= ~(0x1 << (31-15))

    mc_instr |= (regA << (31 - 11))

    if label == 'N/A':
        mc_instr |= ((imm12 & 0x0fff) << (31 - 31))
    else:
        # check if valid label
        if (label_table[label] == None):
            # error
            print("invalid label error")
        else:
            label_addr = label_table[label]
            offset = label_addr - curr_addr
        
        # check if offset fits in 12 bits
        if (offset > 0):
            if (offset < 0x0800): 
                mc_instr |= (offset << (31 - 31))
            else:
                # error
                print("offset fit error")
        else:
            if (offset >= (0x800 * -1)): 
                offset &= 0xfff
                mc_instr |= (offset << (31 - 31))
            else:
                # error
                print("offset fit error")

#    print("mc_instr: 0x%08x" % mc_instr)

    return mc_instr

# system-call
def assemble_form4(asm_tokens):
    mc_instr = 0x0

    opcode = asm_tokens[0].split('.')[0]

    if (re.search('^-?0b', asm_tokens[1])):
        imm4 = int(re.sub("0b", "", asm_tokens[1]), 2)
    elif (re.search('^-?0d', asm_tokens[1])):
        imm4 = int(re.sub("0d", "", asm_tokens[1]), 10)
    elif (re.search('^-?0x', asm_tokens[1])):
        imm4 = int(re.sub("0x", "", asm_tokens[1]), 16)

#    print("opcode: %s, imm4: %s" % (opcode, imm4))
    
    if ('sc' in opcode):
        mc_instr |= (0x11 << (31 - 4))

    if (imm4 < 0x10):
        mc_instr |= (imm4 << (31 - 19))
    else:
        print("sc vector fit error")

#    print("mc_instr: 0x%08x" % mc_instr)

    return mc_instr

def assemble_instr(asm_instr, curr_addr, label_table):
    mc_instr = 0x0
    asm_tokens = asm_instr.split()

    # form 4: <op imm4>
    if ((len(asm_tokens) == 2) and (re.search('^0[bdx]', asm_tokens[1]))):
        mc_instr = assemble_form4(asm_tokens)
    # form 3: <op regA, imm12>, <op label>
    elif ((len(asm_tokens) == 2) or (len(asm_tokens) == 3 and (re.search('-?^0[bdx]', asm_tokens[2])))):
        mc_instr = assemble_form3(asm_tokens, curr_addr + 4, label_table)
    # form 2: <op regA, regB, imm12>, <op regA, label>
    elif ((len(asm_tokens) == 3) or (len(asm_tokens) == 4 and (re.search('^-?0[bdx]', asm_tokens[3])))):
        mc_instr = assemble_form2(asm_tokens, curr_addr + 4, label_table)
    # form 1: <op regA, regB, regC>
    elif ((len(asm_tokens) == 4)):
        mc_instr = assemble_form1(asm_tokens)
        
    return mc_instr

print("// Basic Core Assembler")

input_file = ''

# Get args
try:
    opts, args = getopt.getopt(sys.argv[1:], "hi:")
except getopt.error as err:
    print(str(err))
    print("Usage: assemble.py -i <input_file>")
    sys.exit()

if len(opts) == 0:
    print("Error: No args provided")
    print("Usage: assemble.py -i <input_file>")
    sys.exit()

for opt, arg in opts:
    if opt == "-h":
        print("Usage: assemble.py -i <input_file>")
        sys.exit()
    elif opt == "-i":
        input_file = arg

#print("Input file: %s" % input_file)

asm_file = open(input_file, 'r')
asm_code = asm_file.readlines()
asm_file.close()

# first pass
# process macros: collect (labels, address) pairs, collect data values
addr = 0x0
labels = {}
program = []
i = 0
while i < len(asm_code):
    line = asm_code[i]
    data = 0
    instr = 0x0

    line.lower()
    line = re.sub("^\s*", "", line)
    tokens = line.split()

#    print("line: %s" % line.split('\n')[0])
#    print("addr: 0x%08d" % addr)
#    print(tokens)

    # macros
    # check for empty line
    if (line == "\n" or len(line) == 0):
        pass
#        print("empty line")
    # comments
    elif ('//' in line):
        pass
    # labels
    elif (':' in line):
        label = line.split(':')[0]
        labels.update({label: addr}); 
#        print("label: %s" % label)
    # addresses
    elif ('.addr' in line):
        if (re.search('^0b', tokens[1])):
            addr = int(re.sub("0b", "", tokens[1]), 2)
        elif (re.search('^0d', tokens[1])):
            addr = int(re.sub("0d", "", tokens[1]), 10)
        elif (re.search('^0x', tokens[1])):
            addr = int(re.sub("0x", "", tokens[1]), 16)
        else:
            # error - improperly defined data
            print("improperly defined addr error")

        labels.update({"addr_label_" + str(i): addr})
        asm_code[i] += "#addr_label_" + str(i)
#        print("addr: 0x%08x" % addr)
    # data
    elif (('.byte' in tokens[0]) or ('.hword' in tokens[0]) or ('.word' in tokens[0])):
        if (re.search('^0b', tokens[1])):
            data = int(re.sub("0b", "", tokens[1]), 2)
        elif (re.search('^0d', tokens[1])):
            data = int(re.sub("0d", "", tokens[1]), 10)
        elif (re.search('^0x', tokens[1])):
            data = int(re.sub("0x", "", tokens[1]), 16)
        else:
            # error - improperly defined data
            print("improperly defined data error")

#        print("data: %d" % data)
    
        if ('.byte' in tokens[0]):
            program.append(("// byte", 0)) # disassmbly hint
            program.append((addr, data))
            addr +=1
        elif ('.hword' in tokens[0]):
            program.append(("// hword", 0)) # disassembly hint
            program.append((addr, (data >> 8) & 0x0ff))
            program.append((addr + 1, data & 0x0ff))
            addr +=2
        elif ('.word' in tokens[0]):
            program.append(("// word", 0)) # disassembly hint
            program.append((addr, (data >> 24) & 0x0ff))
            program.append((addr + 1, (data >> 16) & 0x0ff))
            program.append((addr + 2, (data >> 8) & 0x0ff))
            program.append((addr + 3, data & 0x0ff))
            addr +=4
    # instruction
    else:
        addr += 4

    i += 1

# print(labels)

# second pass
# process instructions
addr = 0x0
for line in asm_code:
    dec_val = 0
    instr = 0x0

    line.lower()
    line = re.sub("^\s*", "", line)
    tokens = line.split()
#    print(tokens)

    # macros
    # check for empty line
    if (line == "\n" or len(line) == 0):
        pass
#        print("empty line")
    # comments
    elif ('//' in line):
        pass
    # labels
    elif (':' in line):
        label = line.split(':')[0]
        addr = labels[label]
    # addresses
    elif ('.addr' in line):
        label = line.split('#')[1]
        addr = labels[label]
    # data
    elif (('.byte' in tokens[0]) or ('.hword' in tokens[0]) or ('.word' in tokens[0])):
       if ('.byte' in tokens[0]):
           addr +=1
       elif ('.hword' in tokens[0]):
           addr +=2
       elif ('.word' in tokens[0]):
           addr +=4
    # instruction
    else:
        instr = assemble_instr(line, addr, labels)
        program.append(("// instr", 0)) # disassembly hint
        program.append((addr, (instr >> 24) & 0x0ff))
        program.append((addr + 1, (instr >> 16) & 0x0ff))
        program.append((addr + 2, (instr >> 8) & 0x0ff))
        program.append((addr + 3, instr & 0x0ff))
        addr += 4

# print resulting machine code as C array entry assignments
for byte_data in program:
    if ("//" in str(byte_data[0])):
        print("%s" % byte_data[0])
    else:
        print("mem[0x%04x] = 0x%02x;" % byte_data)

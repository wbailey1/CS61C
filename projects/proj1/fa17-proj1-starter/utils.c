#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


//sign extends a bitfield with given size
/* You may find implementing this function helpful */
int bitSigner( unsigned int field, unsigned int size){
    /* YOUR CODE HERE */
    unsigned int ret;
    unsigned mask = 0xffffffff << size;
    if ((field & (1 << (size - 1))) != 0){
        // number is negative
        ret = field | mask;
    }
    else {
        // number is positive
        ret = field & ~mask;
    }
    return (int) ret;
}

/* Remember that the offsets should return the offset in BYTES */

int get_branch_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    unsigned int mask1 = (1 << 4) - 1;
    unsigned int mask2  = (1 << 6) - 1;
    unsigned int first4bits = (instruction.sbtype.imm5 >> 1) & mask1;
    unsigned int bits4to9 = (instruction.sbtype.imm7) & mask2;
    unsigned int bit10 = instruction.sbtype.imm5 & 1;
    unsigned int bit11 = instruction.sbtype.imm7 >> 6 & 1;
    unsigned int offset = (bit11 << 1) | bit10;
    offset = (offset << 6) | bits4to9;
    offset = (offset << 4) | first4bits;
    offset = offset << 1;
    return bitSigner(offset, 12);
}

int get_jump_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    unsigned int mask1 = (1 << 10) - 1;
    unsigned int mask2 = (1 << 8) - 1;
    unsigned int first10bits = (instruction.ujtype.imm >> 9) & mask1;
    unsigned int next8bits = (instruction.ujtype.imm) & mask2;
    unsigned int bit11 = (instruction.ujtype.imm >> 8) & 1;
    unsigned int bit20 = (instruction.ujtype.imm >> 19) & 1;
    unsigned int offset = (bit20 << 8) | next8bits;
    offset = (offset << 1) | bit11;
    offset = (offset << 10) | first10bits;
    offset = offset << 1;
    return bitSigner(offset, 20);

    // unsigned short extract(unsigned short value, int begin, int end)
    // unsigned short mask = (1 << (end - begin)) - 1;
    // return (value >> begin) & mask;
}


int get_store_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    unsigned int offset = (instruction.stype.imm7 << 5) | instruction.stype.imm5;
    return bitSigner(offset, 12);
}

void handle_invalid_instruction(Instruction instruction) {
    printf("Invalid Instruction: 0x%08x\n", instruction.bits); 
}

void handle_invalid_read(Address address) {
    printf("Bad Read. Address: 0x%08x\n", address);
    exit(-1);
}

void handle_invalid_write(Address address) {
    printf("Bad Write. Address: 0x%08x\n", address);
    exit(-1);
}


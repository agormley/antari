#include "includes.h"

char StackPopByte(void){
  return  memmap->memory[( STACK_MASK | REG_SP++)];
}
ushort StackPopShort(void){
  ushort bytes = 0;
 ((char*)&bytes)[0] = memmap->memory[( STACK_MASK | REG_SP++)];
 ((char*)&bytes)[1] = memmap->memory[( STACK_MASK | REG_SP++)];
 return bytes;
}


void StackPushByte(unsigned char byte){
  memmap->memory[( STACK_MASK | --REG_SP)] = byte;
}
void StackPushShort(ushort bytes){
  memmap->memory[( STACK_MASK | --REG_SP)] = ((char*)&bytes)[1];
  memmap->memory[( STACK_MASK | --REG_SP)] = ((char*)&bytes)[0];
}

#define OPPRINTF(...) printf(__VA_ARGS__)
//#define OPPRINTF(...) 

int
CpuCycle(){
  // get the next opcode
  unsigned char opcode = 0;
    
  // helper vars for calculations
  unsigned char arg1 = 0;
  unsigned char arg2 = 0;
  ushort addr = 0;
  ushort write_addr = 0;
  int cycles = 0;

  if(--processor->cycles > 0){
    //printf("cpu taking cycles: %d\n", processor->cycles);
    return processor->cycles;
  }
      
  if(tia->wsync) {
    //printf("CPU PAUSED\n");
    return 0;
  }

  opcode = MemoryGetByteAt(REG_PC);
    
  // printf("PC: %#x\n", REG_PC);
  //    printf("OPCODE:%#x\n", opcode);
  OPPRINTF("row %d, column %d\n", tia->row, tia->column);
  
  switch(opcode){
  case OPCODE_ADC_IMM:
    arg1 = REG_A;
    arg2 = getImmediate(REG_PC+1 );
    
    OPPRINTF("ADC #$%.2x\n", arg2);
    
    REG_A = add(arg1, arg2, FLAG_CARRY(REG_ST));
    
    REG_PC += 2;
    cycles = 2;
    break;
  case OPCODE_ADC_ZERO:
    
    arg1 = REG_A;
    arg2  = getZero(REG_PC+1, &addr);

    OPPRINTF("ADC $%.2x\n", addr);

    REG_A = add(arg1, arg2, FLAG_CARRY(REG_ST));

    REG_PC += 2;
    cycles = 3;
    break;

  case OPCODE_ADC_ZERO_X:

    arg1 = REG_A;
    arg2  = getZeroX(REG_PC+1, &addr, &write_addr);

    OPPRINTF("ADC $%.2x,X\n", addr);

    REG_A =
      add(arg1, arg2, FLAG_CARRY(REG_ST));

    REG_PC += 2;
    cycles = 4;
    break;
  case OPCODE_ADC_ABS:

    arg1 = REG_A;
    arg2  = getAbsolute(REG_PC+1, &addr);

    OPPRINTF("ADC $%.4x\n", addr);
    
    REG_A =
      add(arg1, arg2, FLAG_CARRY(REG_ST)); 

    REG_PC += 3;
    cycles = 4;
    
    break;

  case OPCODE_ADC_ABS_X:

    arg1 = REG_A;
    arg2  = getAbsoluteX(REG_PC+1, &addr, NULL);
    
    OPPRINTF("ADC $%.4x,X\n", addr);
    
    REG_A =
      add(arg1, arg2, FLAG_CARRY(REG_ST)); 

    REG_PC += 3;

    cycles = 4;

    if (((addr  & 0xFF) + REG_X) > 0x100)
      cycles++;

    break;
      
  case OPCODE_ADC_ABS_Y:

    arg1 = REG_A;
    arg2  = getAbsoluteX(REG_PC+1, &addr, NULL);
    
    OPPRINTF("ADC $%.4x,Y\n", addr);
    
    REG_A =
      add(arg1, arg2, FLAG_CARRY(REG_ST)); 

    REG_PC += 3;

    cycles = 4;
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;
    break;
  case OPCODE_ADC_IND_X:
      
    arg1 = REG_A;
    arg2 = getIndirectX(REG_PC+1, &addr, NULL);

    OPPRINTF("ADC ($.2%x,X)\n", addr);
    
    REG_A =
      add(arg1, arg2, FLAG_CARRY(REG_ST)); 

    REG_PC += 2;
    cycles = 6;
    break;
      
  case OPCODE_ADC_IND_Y:
    assert(!FLAG_DECIMAL(REG_ST)); 
      
    arg1 = REG_A;
    arg2 = getIndirectX(REG_PC+1, &addr, NULL);
    OPPRINTF("ADC ($.2%x),Y\n", addr);
    
    REG_A =
      add(arg1, arg2, FLAG_CARRY(REG_ST)); 

    REG_PC += 2;

    cycles = 5;

    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;
  case OPCODE_AND_IMM:
    arg1 = REG_A;
    arg2 = getImmediate(REG_PC+1 );

    OPPRINTF("AND #$%.2x\n", arg2);

    REG_A = and(arg1, arg2);

    REG_PC += 2;

    cycles = 2;
    break;
    
  case OPCODE_AND_ZERO:
    arg1 = REG_A;
    arg2 = getZero(REG_PC+1, &addr);

    OPPRINTF("AND $%.2x\n", addr);

    REG_A = and(arg1, arg2);

    REG_PC += 2;

    cycles = 3;
    break;

  case OPCODE_AND_ZERO_X:
    arg1 = REG_A;
    arg2 = getZeroX(REG_PC+1, &addr, &write_addr);

    OPPRINTF("AND $%.2x,X\n", addr);

    REG_A = and(arg1, arg2);

    REG_PC += 2;
    cycles = 4;
    break;

  case OPCODE_AND_ABS:

    arg1 = REG_A;
    arg2 = getAbsolute(REG_PC+1, &addr);
    
    OPPRINTF("AND $%.4x\n", addr);

    REG_A = and(arg1, arg2);

    REG_PC += 3;
    cycles = 4;
    break;
      
  case OPCODE_AND_ABS_X:
    arg1 = REG_A;

    arg2 = getAbsoluteX(REG_PC+1, &addr, NULL);

    OPPRINTF("AND $%.4x,X\n", addr);

    REG_A = and(arg1, arg2);

    REG_PC += 3;
    cycles = 4;
    
    if (((addr  & 0xFF) + REG_X) > 0x100)
      cycles++;
    break;
  case OPCODE_AND_ABS_Y:
    arg1 = REG_A;

    arg2 = getAbsoluteY(REG_PC+1, &addr, NULL);

    OPPRINTF("AND $%.4x,Y\n", addr);

    REG_A = and(arg1, arg2);

    REG_PC += 3;
    cycles = 4;
    
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;
    
    break;

  case OPCODE_AND_IND_X:
    arg1 = REG_A;

    arg2 = getIndirectX(REG_PC+1, &addr, NULL);

    OPPRINTF("AND ($.2%x,X)\n", addr);

    REG_A = and(arg1, arg2);

    REG_PC += 2;
    cycles = 6;
    break;

      
  case OPCODE_AND_IND_Y:
     
    arg1 = REG_A;

    arg2 = getIndirectY(REG_PC+1, &addr, NULL);

    OPPRINTF("AND ($.2%x),Y\n", addr);

    REG_A = and(arg1, arg2);

    REG_PC += 2;
    cycles = 5;
    
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;
    break;

  case OPCODE_ASL_A:
    OPPRINTF("ASL A\n");
    arg1 = REG_A;

    REG_A = asl(arg1);

    REG_PC += 1;
    cycles = 2;
    break;

  case OPCODE_ASL_ZERO:
    
    arg1 = getZero(REG_PC+1, &addr);

    OPPRINTF("ASL: $%.2x\n", addr);

    MemorySetByteAt(addr, asl(arg1));

    REG_PC += 2;
    cycles = 5;
    break;

  case OPCODE_ASL_ZERO_X:
    arg1 = getZeroX(REG_PC+1, &addr, &write_addr);

    OPPRINTF("ASL $%.2x,X\n", addr);

    MemorySetByteAt(write_addr, asl(arg1));

    REG_PC += 2;
    cycles = 6;
    break;

  case OPCODE_ASL_ABS:

    arg1 = getAbsolute(REG_PC+1, &addr);

    OPPRINTF("ASL $%.4x\n", addr);

    // write back to memory
    MemorySetByteAt(addr, asl(arg1));

    // advance pc
    REG_PC += 3;
    cycles = 6;
    break;
      
  case OPCODE_ASL_ABS_X:
    
    arg1 = getAbsoluteX(REG_PC+1, &addr, &write_addr);

    OPPRINTF("ASL $%.4x,X\n", addr);
    
    // write back to memory
    MemorySetByteAt(write_addr, asl(arg1));

    // advance pc
    REG_PC += 3;
    cycles = 7;
    break;
  case OPCODE_BIT_ZERO:
    arg1 = REG_A;
    arg2 = getZero(REG_PC+1, &addr);
    
    OPPRINTF("BIT $%.2x\n", addr);

    // No return, just sets flags
    bit(arg1, arg2);

    REG_PC += 2;
    cycles = 3;
    break;
      
  case OPCODE_BIT_ABS:
    arg1 = REG_A;
    arg2 = getAbsolute(REG_PC+1, &addr);

    // No return, just sets flags
    bit(arg1, arg2);

    OPPRINTF("BIT $%.4x\n", addr);

    REG_PC += 3;
    cycles = 4;
    break;

  case OPCODE_BPL:
    OPPRINTF("BPL\n");

    cycles = 2;
    // Note address if branch not taken
    addr = REG_PC + 2;
      
    if(!FLAG_NEG(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((addr & 0xF00) != (REG_PC & 0xF00))
	cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BMI:
    OPPRINTF("BMI\n");
    cycles = 2;

    addr = REG_PC + 2;

    if(FLAG_NEG(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((addr & 0xF00) != (REG_PC & 0xF00))
	cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BVC:
    OPPRINTF("BVC\n");
    cycles = 2;
    // Note address if branch not taken
    addr = REG_PC + 2;

    if(!FLAG_OVER(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((addr & 0xF00) != (REG_PC & 0xF00))
	cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BVS:
    OPPRINTF("BVS\n");
    cycles = 2;
    // Note address if branch not taken
    addr = REG_PC + 2;

    if(FLAG_OVER(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((addr & 0xF00) != (REG_PC & 0xF00))
	cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BCC:
    OPPRINTF("BCC\n");
    cycles = 2;
    // Note address if branch not taken
    addr = REG_PC + 2;

    if(!FLAG_CARRY(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((addr & 0xF00) != (REG_PC & 0xF00))
	cycles++;

    }
    REG_PC += 2;					    
    break;
  case OPCODE_BCS:
    OPPRINTF("BCS\n");
    cycles = 2;
    // Note address if branch not taken
    addr = REG_PC + 2;

    if(FLAG_CARRY(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      // Does branch cross page boundary?
      if((addr & 0xF00) != (REG_PC & 0xF00))
	cycles++;
      cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BNE:

    OPPRINTF("BNE %.4x\n",
	     REG_PC +
	     (signed char)(MemoryGetByteAt(REG_PC + 1)));

    cycles = 2;
    // Note address if branch not taken
    addr = REG_PC + 2;
    if(!FLAG_ZERO(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));

      cycles++;
      // Does branch cross page boundary?
      if((addr & 0xF00) != (REG_PC & 0xF00))
	cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BEQ:
    OPPRINTF("BEQ\n");
    cycles = 2;
    // Note address if branch not taken
    addr = REG_PC + 2;

    if(FLAG_ZERO(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((addr & 0xF00) != (REG_PC & 0xF00))
	cycles++;

    }
	 
    REG_PC += 2;					    
    break;
  case OPCODE_BRK:
    OPPRINTF("BRK\n");
    OPPRINTF("BREAK INSTRUCTION: Not Implemented!\n");
    //    assert(true == false);
    //programrunning = false;
    REG_PC++;
    cycles = 7;

    break;
  case OPCODE_CMP_IMM:
    arg1 = REG_A;
    arg2 = getImmediate(REG_PC+1);

    cmp(arg1, arg2);

    OPPRINTF("CMP #$%.2x\n", arg2);

    REG_PC += 2;
    cycles = 2;

    break;
  case OPCODE_CMP_ZERO:
    arg1 = REG_A;
    arg2 = getZero(REG_PC+1, &addr);
    OPPRINTF("CMP $%.2x\n", addr);

    cmp(arg1, arg2);
    REG_PC += 2;
    cycles = 3;
	    
    break;
  case OPCODE_CMP_ZERO_X:
    arg1 = REG_A;
    arg2 = getZeroX(REG_PC+1, &addr, &write_addr);
    OPPRINTF("CMP $%.2x,X\n", addr);

    cmp(arg1, arg2);

    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_CMP_ABS:
    arg1 = REG_A;
    arg2 = getAbsolute(REG_PC+1, &addr);

    cmp(arg1, arg2);

    OPPRINTF("CMP $%.4x\n", addr);

    REG_PC += 3;
    cycles = 4;

    break;
  case OPCODE_CMP_ABS_X:
    arg1 = REG_A;
    arg2 = getAbsoluteX(REG_PC+1, &addr, NULL);

    cmp(arg1, arg2);
    OPPRINTF("CMP $%.4x,X\n", addr);

    REG_PC += 3;
    cycles = 4;
    
    if (((addr  & 0xFF) + REG_X) > 0x100)
      cycles++;

    break;
  case OPCODE_CMP_ABS_Y:
    arg1 = REG_A;
    arg2 = getAbsoluteY(REG_PC+1, &addr, NULL);

    cmp(arg1, arg2);
    OPPRINTF("CMP $%.4x,Y\n", addr);

    REG_PC += 3;
    cycles = 4;

    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;
    break;

  case OPCODE_CMP_IND_X:
    arg1 = REG_A;
    arg2 = getIndirectX(REG_PC+1, &addr, NULL);

    cmp(arg1, arg2);
    OPPRINTF("CMP ($x%.2x,X)\n", addr);

    REG_PC += 2;
    cycles = 6;

    break;
     
  case OPCODE_CMP_IND_Y:
    arg1 = REG_A;
    arg2 = getIndirectY(REG_PC+1, &addr, NULL);

    cmp(arg1, arg2);
    OPPRINTF("CMP ($%.2x),Y\n", addr);

    REG_PC += 2;
    cycles = 5;

    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;
    break;

  case OPCODE_CPX_IMM:
    arg1 = REG_X;
    arg2 = getImmediate(REG_PC+1);

    cmp(arg1, arg2);

    OPPRINTF("CPX #$%.2x\n", arg2);

    REG_PC += 2;
    cycles = 2;

    break;
  case OPCODE_CPX_ZERO:
    arg1 = REG_X;
    arg2 = getZero(REG_PC+1, &addr);
    OPPRINTF("CPX $%.2x\n", addr);

    cmp(arg1, arg2);
    REG_PC += 2;
    cycles = 3;
	    
    break;

  case OPCODE_CPX_ABS:
    arg1 = REG_X;
    arg2 = getAbsolute(REG_PC+1, &addr);

    cmp(arg1, arg2);

    OPPRINTF("CPX $%.4x\n", addr);

    REG_PC += 3;
    cycles = 4;

    break;


  case OPCODE_CPY_IMM:
    arg1 = REG_Y;
    arg2 = getImmediate(REG_PC+1);

    cmp(arg1, arg2);

    OPPRINTF("CPY #$%.2x\n", arg2);

    REG_PC += 2;
    cycles = 2;

    break;
 case OPCODE_CPY_ZERO:
    arg1 = REG_Y;
    arg2 = getZero(REG_PC+1, &addr);
    OPPRINTF("CMP $%.2x\n", addr);

    cmp(arg1, arg2);
    REG_PC += 2;
    cycles = 3;
	    
    break;
  case OPCODE_CPY_ABS:
    arg1 = REG_Y;
    arg2 = getAbsolute(REG_PC+1, &addr);

    cmp(arg1, arg2);

    OPPRINTF("CPY $%.4x\n", addr);

    REG_PC += 3;
    cycles = 4;

    break;
  case OPCODE_DEC_ZERO:
    arg1 = getZero(REG_PC+1, &addr);
 
    OPPRINTF("DEC $%.2x\n", addr);

    dec(arg1, addr);
    
    REG_PC += 2;
    cycles = 5;

    break;
     
  case OPCODE_DEC_ZERO_X:
    arg1 = getZeroX(REG_PC+1, &addr, &write_addr);
 
    dec(arg1, addr);

    OPPRINTF("DEC $%.2x,X\n", addr);

    REG_PC += 2;
    cycles = 6;

    break;

  case OPCODE_DEC_ABS:
    arg1 = getAbsolute(REG_PC+1, &addr);
 
    dec(arg1, addr);
    OPPRINTF("DEC $%.4x\n", addr);

    REG_PC += 3;
    cycles = 6;

    break;

  case OPCODE_DEC_ABS_X:
    arg1 = getAbsoluteX(REG_PC+1, &addr, NULL);
 
    dec(arg1, addr);

    OPPRINTF("DEC $%.4x,X\n", addr);

    REG_PC += 3;
    cycles = 7;

    break;

  case OPCODE_EOR_IMM:
    arg1 = REG_A;
    arg2 = getImmediate(REG_PC+1 );
    
    REG_A = eor(arg1, arg2);
    OPPRINTF("EOR #$%.2x\n", arg2);

    REG_PC += 2;
    cycles = 2;

    break;

  case OPCODE_EOR_ZERO:
    arg1 = REG_A;
    arg2 = getZero(REG_PC+1, &addr );
    
    REG_A = eor(arg1, arg2);
    OPPRINTF("EOR $%.2x\n", addr);

    REG_PC += 2;
    cycles = 3;

    break;

  case OPCODE_EOR_ZERO_X:
    arg1 = REG_A;
    arg2 = getZeroX(REG_PC+1, &addr, NULL);
    
    REG_A = eor(arg1, arg2);
    OPPRINTF("EOR $%.2x,X\n", addr);

    REG_PC += 2;
    cycles = 4;

    break;
      
  case OPCODE_EOR_ABS:
    arg1 = REG_A;
    arg2 = getAbsolute(REG_PC+1, &addr );
    
    REG_A = eor(arg1, arg2);
    OPPRINTF("EOR $%.4x\n", addr);
 
    REG_PC += 3;
    cycles = 4;

    break;
      
  case OPCODE_EOR_ABS_X:
    arg1 = REG_A;

    arg2 = getAbsoluteX(REG_PC+1, &addr, NULL);
    
    REG_A = eor(arg1, arg2);

    OPPRINTF("EOR $%.4x,X\n", addr);

    REG_PC += 3;
    cycles = 4;

    if (((addr  & 0xFF) + REG_X) > 0x100)
      cycles++;

    break;
      
  case OPCODE_EOR_ABS_Y:
    arg1 = REG_A;

    arg2 = getAbsoluteY(REG_PC+1, &addr, NULL);
    
    REG_A = eor(arg1, arg2);

    OPPRINTF("EOR $%.4x,Y\n", addr);

    REG_PC += 3;
    
    cycles = 4;
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;

  case OPCODE_EOR_IND_X:
    arg1 = REG_A;

    arg2 = getIndirectX(REG_PC+1, &addr, NULL);
    
    REG_A = eor(arg1, arg2);

    OPPRINTF("EOR $%.4x,Y\n", addr);

    REG_PC += 2;
    cycles = 6;
            
    break;
  case OPCODE_EOR_IND_Y:
    arg1 = REG_A;

    arg2 = getIndirectY(REG_PC+1, &addr, NULL);
    
    REG_A = eor(arg1, arg2);

    OPPRINTF("EOR ($.2%x),Y\n", addr);

    REG_PC += 2;

    cycles = 5;
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;

  case OPCODE_CLC:
    OPPRINTF("CLC\n");
    FLAG_CARRY_CLEAR(REG_ST);
    REG_PC++;
    cycles = 2;

    break;
  case OPCODE_SEC:
    OPPRINTF("SEC\n");
    FLAG_CARRY_SET(REG_ST);
    REG_PC++;
    cycles = 2;

    break;
  case OPCODE_CLI:
    OPPRINTF("CLI\n");
    FLAG_INTERRUPT_CLEAR(REG_ST);
    REG_PC++;
    cycles = 2;

    break;
  case OPCODE_SEI:
    OPPRINTF("SEI\n");
    FLAG_INTERRUPT_SET(REG_ST);
    REG_PC++;
    cycles = 2;

    break;
  case OPCODE_CLV:
    OPPRINTF("CLV\n");
    FLAG_OVER_CLEAR(REG_ST);
    REG_PC++;
    cycles = 2;

    break;
  case OPCODE_CLD:
    OPPRINTF("CLD\n");
    FLAG_DECIMAL_CLEAR(REG_ST);
    REG_PC++;
    cycles = 2;

    break;
  case OPCODE_SED:
    OPPRINTF("SED\n");
    FLAG_DECIMAL_SET(REG_ST);
    REG_PC++;
    cycles = 2;

    break;
  case OPCODE_INC_ZERO:
    arg1 = getZero(REG_PC+1, &addr);

    OPPRINTF("INC $%.2x\n", arg1);

    inc(arg1, addr);
    
    REG_PC += 2;
    cycles = 5;

    break;
     
  case OPCODE_INC_ZERO_X:
    arg1 = getZeroX(REG_PC+1, &addr, &write_addr);
 
    inc(arg1, write_addr);

    OPPRINTF("INC $%.2x,X\n", addr);

    REG_PC += 2;
    cycles = 6;

    break;

  case OPCODE_INC_ABS:
    arg1 = getAbsolute(REG_PC+1, &addr);
 
    inc(arg1, addr);
    OPPRINTF("INC $%.4x\n", addr);

    REG_PC += 3;
    cycles = 6;

    break;

  case OPCODE_INC_ABS_X:
    arg1 = getAbsoluteX(REG_PC+1, &addr, &write_addr);
 
    inc(arg1, write_addr);

    OPPRINTF("INC $%.4x,X\n", addr);

    REG_PC += 3;
    cycles = 7;

    break;


  case OPCODE_JMP_ABS:
    getAbsolute(REG_PC+1, &addr);

    OPPRINTF("JMP $%.4x\n", addr);

    REG_PC = addr;
    cycles = 3;

    break;
  case OPCODE_JMP_IND:
    getIndirect(REG_PC+1, &addr);
    OPPRINTF("JMP ($%.4x)\n", addr);

    REG_PC = addr;
    cycles = 5;

    break;
  case OPCODE_JSR_ABS:
    getAbsolute(REG_PC+1, &addr);
    OPPRINTF("JSR $%.4x\n", addr);

    StackPushShort(REG_PC + 2);
    REG_PC = addr;
      
    cycles = 6;
	    
    break;
  case OPCODE_LDA_IMM:
    arg2 = getImmediate(REG_PC+1);

    lda(arg2);
    OPPRINTF("LDA #$%.2x\n", arg2);

    REG_PC += 2;
    cycles = 2;

    break;
  case OPCODE_LDA_ZERO:
    arg2 = getZero(REG_PC+1, &addr);

    OPPRINTF("LDA $%.2x\n", addr);

    lda(arg2);

    REG_PC += 2;
    cycles = 3;

    break;
  case OPCODE_LDA_ZERO_X:
    arg2 = getZeroX(REG_PC+1, &addr, &write_addr);
    OPPRINTF("LDA $%.2x,X\n", addr);
    lda(arg2);
    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_LDA_ABS:
    arg2 = getAbsolute(REG_PC+1, &addr);
    OPPRINTF("LDA $%.4x\n", addr);
    lda(arg2);
    REG_PC += 3;
    cycles = 4;

    break;
  case OPCODE_LDA_ABS_X:
    arg2 = getAbsoluteX(REG_PC+1, &addr, NULL);
    OPPRINTF("LDA $%.4x,X\n", addr);
    lda(arg2);

    REG_PC += 3;
    cycles = 4;
    if (((addr  & 0xFF) + REG_X) > 0x100)
      cycles++;
    break;
  case OPCODE_LDA_ABS_Y:
    arg2 = getAbsoluteY(REG_PC+1, &addr, NULL);
    OPPRINTF("LDA $%.4x,Y\n", addr);

    lda(arg2);
    REG_PC = REG_PC+3;
    cycles = 4;
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;
  case OPCODE_LDA_IND_X:
    arg2 = getIndirectX(REG_PC+1, &addr, NULL);

    OPPRINTF("LDA ($%.2x,X)\n", addr);

    lda(arg2);

    REG_PC += 2;
    cycles = 6;

    break;
  case OPCODE_LDA_IND_Y:
    arg2 = getIndirectY(REG_PC+1, &addr, NULL);

    OPPRINTF("LDA ($%.2x),Y\n", addr);

    lda(arg2);
    REG_PC += 2;
    cycles = 5;

    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;
  case OPCODE_LDX_IMM:

    arg2 = getImmediate(REG_PC+1);

    OPPRINTF("LDX #$%.2x\n", arg2);
    ldx(arg2);
    REG_PC += 2;
    cycles = 2;

    break;
  case OPCODE_LDX_ZERO:
    arg2 = getZero(REG_PC+1, &addr);

    OPPRINTF("LDX $%.2x\n", addr);
    ldx(arg2);
    REG_PC += 2;
    cycles = 3;

    break;

  case OPCODE_LDX_ZERO_Y:
    arg2 = getZeroY(REG_PC+1, &addr, NULL);
    
    OPPRINTF("LDX $%.2x,Y\n", addr);
    ldx(arg2);
    REG_PC += 2;
    cycles = 4;

    break;

  case OPCODE_LDX_ABS:
    arg2 = getAbsolute(REG_PC+1, &addr);
	
    OPPRINTF("LDX $%.4x\n", addr);
    ldx(arg2);

    REG_PC = REG_PC+3;
    cycles = 4;

    break;
      
  case OPCODE_LDX_ABS_Y:
    arg2 = getAbsoluteY(REG_PC+1, &addr, NULL);

    OPPRINTF("LDX $%.4x,Y\n", addr);
    ldx(arg2);

    REG_PC = REG_PC+3;
    cycles = 4;
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;
  case OPCODE_LDY_IMM:
    arg2 = getImmediate(REG_PC+1);

    OPPRINTF("LDY #$%.2x\n", arg2);

    ldy(arg2);

    REG_PC += 2;
    cycles = 2;

    break;
  case OPCODE_LDY_ZERO:
    arg2 = getZero(REG_PC+1, &addr);

    OPPRINTF("LDY $%.2x\n", addr);
    ldy(arg2);
    REG_PC += 2;
    cycles = 3;

    break;

  case OPCODE_LDY_ZERO_X:
    arg2 = getZeroX(REG_PC+1, &addr, &write_addr);

    OPPRINTF("LDY $%.2x,X\n", addr);
    ldy(arg2);
    REG_PC += 2;
    cycles = 4;

    break;

  case OPCODE_LDY_ABS:
    arg2 = getAbsolute(REG_PC+1, &addr);
    OPPRINTF("LDY $%.4x\n", addr);

    ldy(arg2);

    REG_PC = REG_PC+3;
    cycles = 4;

    break;
  case OPCODE_LDY_ABS_X:
    arg2 = getAbsoluteX(REG_PC+1, &addr, NULL);

    OPPRINTF("LDY $%.4x,X\n", addr);
    ldy(arg2);

    REG_PC = REG_PC+3;
    cycles = 4;
    if (((addr  & 0xFF) + REG_X) > 0x100)
      cycles++;

    break;
  case OPCODE_LSR_A:
    OPPRINTF("LSR A\n");

    lsr(&REG_A); 
    REG_PC += 1;
    cycles = 2;

    break;
   
  case OPCODE_LSR_ZERO:
    arg2 = getZero(REG_PC+1, &addr);
    OPPRINTF("LSR $%.2x\n", addr);

    lsr(&arg2);

    MemorySetByteAt(addr, arg2);
    REG_PC += 2;
    cycles = 5;

    break;
      
  case OPCODE_LSR_ZERO_X:
    // calculate addr
    arg2 = getZeroX(REG_PC+1, &addr, &write_addr);

    lsr(&arg2);

    MemorySetByteAt(addr, arg2);


    OPPRINTF("LSR $%.2x,X\n", addr);

    // advance pc past used bytes
    REG_PC += 2;
    cycles = 6;

    break;

  case OPCODE_LSR_ABS:
    // get addr
    arg2 = getAbsolute(REG_PC+1, &addr);

    lsr(&arg2);

    MemorySetByteAt(addr, arg2);

    OPPRINTF("LSR $%.4x\n", addr);

    // advance pc
    REG_PC += 3;
    cycles = 6;

    break;
      
  case OPCODE_LSR_ABS_X:
    arg2 = getAbsoluteX(REG_PC+1, &addr, NULL);

    lsr(&arg2);

    MemorySetByteAt(addr, arg2);

    OPPRINTF("LSR $%.4x,X\n", addr);

    // advance pc
    REG_PC += 3;
    cycles = 7;

    break;

  case OPCODE_NOP:
    OPPRINTF("NOP\n");
    // No op!
      
    // advance pc
    REG_PC += 1;
    cycles = 2;

    break;
  case OPCODE_ORA_IMM:
    arg2 = getImmediate(REG_PC+1);

    OPPRINTF("ORA #$%.2x\n", arg2);
    ora(arg2);

    REG_PC += 2;
    cycles = 2;
            
    break;
  case OPCODE_ORA_ZERO:
    arg2 = getZero(REG_PC+1, &addr);

    OPPRINTF("ORA $%.2x\n", addr);

    ora(arg2);

    REG_PC += 2;
    cycles = 3;

    break;
  case OPCODE_ORA_ZERO_X:
    arg2 = getZeroX(REG_PC+1, &addr, &write_addr);

    OPPRINTF("ORA $%.2x,X\n", addr);

    ora(arg2);

    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_ORA_ABS:
    
    arg2 = getAbsolute(REG_PC+1, &addr);
    OPPRINTF("ORA $%.4x\n", addr);

    ora(arg2);

    REG_PC += 3;
    cycles = 4;

    break;
      
  case OPCODE_ORA_ABS_X:
    arg2 = getAbsoluteX(REG_PC+1, &addr, NULL);

    OPPRINTF("ORA $%.4x,X\n", addr);

    ora(arg2);
    REG_PC += 3;
    cycles = 4;
    if (((addr  & 0xFF) + REG_X) > 0x100)
      cycles++;
    break;

  case OPCODE_ORA_ABS_Y:
    arg2 = getAbsoluteY(REG_PC+1, &addr, NULL);

    OPPRINTF("ORA $%.4x,Y\n", addr);

    ora(arg2);
    
    REG_PC += 3;
    cycles = 4;
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;

  case OPCODE_ORA_IND_X:
    arg2 = getIndirectX(REG_PC+1, &addr, NULL);

    OPPRINTF("ORA ($.2%x,X)\n", addr);

    ora(arg2);

    REG_PC += 2;
    cycles = 6;

    break;

  case OPCODE_ORA_IND_Y:
    arg2 = getIndirectY(REG_PC+1, &addr, NULL);

    OPPRINTF("ORA ($.2%x),Y\n", addr);

    ora(arg2);

    REG_PC += 2;
    cycles = 5;
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;

  case OPCODE_TAX:
    OPPRINTF("TAX\n");
    REG_X = REG_A;
    REG_PC++;
    SETSIGN(REG_X);
    SETZERO(REG_X);
    cycles = 2;
	    
    break;
  case OPCODE_TXA:
    OPPRINTF("TXA\n");
    REG_A = REG_X;
    REG_PC++;
    SETSIGN(REG_A);
    SETZERO(REG_A);
    cycles = 2;

    break;
  case OPCODE_DEX:
    OPPRINTF("DEX\n");
    REG_X--;
    REG_PC++;
    SETSIGN(REG_X);
    SETZERO(REG_X);
    cycles = 2;

    break;
  case OPCODE_INX:
    OPPRINTF("INX\n");
    REG_X++;
    //    OPPRINTF("x: %#x\n", REG_X);
    REG_PC++;
    SETSIGN(REG_X);
    SETZERO(REG_X);
    cycles = 2;

    break;
  case OPCODE_TAY:
    OPPRINTF("TAY\n");
    REG_Y = REG_A;
    REG_PC++;
    SETSIGN(REG_Y);
    SETZERO(REG_Y);
    cycles = 2;

    break;
  case OPCODE_TYA:
    OPPRINTF("TYA\n");
    REG_A = REG_Y;
    REG_PC++;
    SETSIGN(REG_A);
    SETZERO(REG_A);
    cycles = 2;

    break;
  case OPCODE_DEY:
    OPPRINTF("DEY\n");
    REG_Y--;
    REG_PC++;
    SETSIGN(REG_Y);
    SETZERO(REG_Y);
    cycles = 2;

    break;
  case OPCODE_INY:
    OPPRINTF("INY\n");
    REG_Y++;
    REG_PC++;
    SETSIGN(REG_Y);
    SETZERO(REG_Y);
    cycles = 2;

    break;
  case OPCODE_ROL_A:
    OPPRINTF("ROL A\n");

    rol(&REG_A);

    REG_PC += 1;
    cycles = 2;

    break;
      
  case OPCODE_ROL_ZERO:
    arg2 = getZero(REG_PC+1, &addr);

    OPPRINTF("ROL $%.2x\n", addr);

    rol(&arg2);

    // write back value
    MemorySetByteAt(addr, arg2);

    // increment pc
    REG_PC += 2;
    cycles = 5;

    break;
  case OPCODE_ROL_ZERO_X:
    arg2 = getZeroX(REG_PC+1, &addr, &write_addr);

    OPPRINTF("ROL $%.2x,X\n", addr);

    rol(&arg2);

    // write back value
    MemorySetByteAt(addr, arg2);

    // increment pc
    REG_PC += 2;
    cycles = 6;

    break;

  case OPCODE_ROL_ABS:
    arg2 = getAbsolute(REG_PC+1, &addr);

    rol(&arg2);

    // write back value
    MemorySetByteAt(addr, arg2);

    OPPRINTF("ROL $%.4x\n", addr);

    // advance pc
    REG_PC += 3;
    cycles = 6;

    break;

  case OPCODE_ROL_ABS_X:
    arg2 = getAbsoluteX(REG_PC+1, &addr, NULL);

    rol(&arg2);

    // write back value
    MemorySetByteAt(addr, arg2);

    OPPRINTF("ROL $%.4x,X\n", addr);

    // advance pc
    REG_PC += 3;
    cycles = 7;

    break;

  case OPCODE_ROR_A:
    OPPRINTF("ROR A\n");

    ror(&REG_A);

    // increment pc
    REG_PC += 1;
    cycles = 2;

    break;

  case OPCODE_ROR_ZERO:
    arg2 = getZero(REG_PC+1, &addr);

    ror(&arg2);

    // write back value
    MemorySetByteAt(addr, arg2);

    OPPRINTF("ROR $%.2x\n", addr);

    // increment pc
    REG_PC += 2;
    cycles = 5;

    break;
  case OPCODE_ROR_ZERO_X:
    arg2 = getZeroX(REG_PC+1, &addr, &write_addr);

    ror(&arg2);

    // write back value
    MemorySetByteAt(addr, arg2);

    OPPRINTF("ROR $%.2x,X\n", addr);

    // increment pc
    REG_PC += 2;
    cycles = 6;

    break;

  case OPCODE_ROR_ABS:
    arg2 = getAbsolute(REG_PC+1, &addr);

    ror(&arg2);

    // write back value
    MemorySetByteAt(addr, arg2);

    OPPRINTF("ROR $%.4x\n", addr);

    // advance pc
    REG_PC += 3;
    cycles = 6;
    break;

  case OPCODE_ROR_ABS_X:
    arg2 = getAbsoluteX(REG_PC+1, &addr, NULL);

    ror(&arg2);

    // write back value
    MemorySetByteAt(addr, arg2);

    OPPRINTF("ROR $%.4x,X\n", addr);

    // advance pc
    REG_PC += 3;
    cycles = 7;
    break;

  case OPCODE_RTI:
    OPPRINTF("RTI\n");
    /* RTI retrieves the Processor Status Word (flags) and the Program Counter from the stack in that order (interrupts push the PC first and then the PSW).  */
    REG_ST = StackPopByte();
    REG_PC = StackPopShort();
    cycles = 6;

    break;
  case OPCODE_RTS:
    OPPRINTF("RTS\n");
    REG_PC = StackPopShort() + 1;
    cycles = 6;

    break;
  case OPCODE_SBC_IMM:
    // Get subtrahend
    arg2 = getImmediate(REG_PC+1);

    REG_A = sbc(REG_A, arg2, FLAG_CARRY(REG_ST));

    OPPRINTF("SBC #$%.2x\n", arg2);


    REG_PC += 2;
    cycles = 2;
    break;
  case OPCODE_SBC_ZERO:
    // Get subtrahend
    arg2 = getZero(REG_PC+1, &addr);

    REG_A = sbc(REG_A, arg2, FLAG_CARRY(REG_ST));

    OPPRINTF("SBC $%.2x\n", addr);

    REG_PC += 2;
    cycles = 3;
    break;

  case OPCODE_SBC_ZERO_X:
    // Get subtrahend
    arg2 = getZeroX(REG_PC+1, &addr, &write_addr);

    REG_A = sbc(REG_A, arg2, FLAG_CARRY(REG_ST));

    OPPRINTF("SBC $%.2x,X\n", addr);
    
    REG_PC += 2;
    cycles = 4;
	    
    break;
 
  case OPCODE_SBC_ABS:
    // Get subtrahend
    arg2 = getAbsolute(REG_PC+1, &addr);
    
    REG_A = sbc(REG_A, arg2, FLAG_CARRY(REG_ST));

    OPPRINTF("SBC $%.4x\n", addr);
    
    REG_PC += 3;
    cycles = 4;

    break;

  case OPCODE_SBC_ABS_X:
    // Get subtrahend
    arg2 = getAbsoluteX(REG_PC+1, &addr, NULL);

    REG_A = sbc(REG_A, arg2, FLAG_CARRY(REG_ST));
    
    OPPRINTF("SBC $%.4x,X\n", addr);


    REG_PC += 3;
    cycles = 4;
    if (((addr  & 0xFF) + REG_X) > 0x100)
      cycles++;

    break;

  case OPCODE_SBC_ABS_Y:
    // Get subtrahend
    arg2 = getAbsoluteY(REG_PC+1, &addr, NULL);

    REG_A = sbc(REG_A, arg2, FLAG_CARRY(REG_ST));
    
    OPPRINTF("SBC $%.4x,Y\n", addr);


    REG_PC += 3;
    cycles = 4;
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;

  case OPCODE_SBC_IND_X:
    // Get subtrahend
    arg2 = getIndirectX(REG_PC+1, &addr, NULL);

    REG_A = sbc(REG_A, arg2, FLAG_CARRY(REG_ST));
    
    OPPRINTF("SBC ($.2%x,X)\n", addr);
    
    REG_PC += 2;
    cycles = 6;

    break;
      
  case OPCODE_SBC_IND_Y:
    // Get subtrahend
    arg2 = getIndirectY(REG_PC+1, &addr, NULL);

    REG_A = sbc(REG_A, arg2, FLAG_CARRY(REG_ST));
    
    OPPRINTF("SBC ($.2%x),Y\n", addr);

    REG_PC += 2;
    cycles = 5;
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;

  case OPCODE_STA_ZERO:
    getZero(REG_PC+1, &write_addr);

    sta(write_addr);
    
    OPPRINTF("STA $%.2x\n", write_addr);
    
    REG_PC += 2;
    cycles = 3;

    break;
  case OPCODE_STA_ZERO_X:
    getZeroX(REG_PC+1, &addr, &write_addr);

    sta(write_addr);
    
    OPPRINTF("STA $%.2x,X\n", addr);

    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_STA_ABS:
    getAbsolute(REG_PC+1, &addr);

    sta(addr);

    OPPRINTF("STA $%.4x\n", addr);
    REG_PC = REG_PC+3;
    cycles = 4;

    break;
  case OPCODE_STA_ABS_X:
    getAbsoluteX(REG_PC+1, &addr, &write_addr);

    sta(write_addr);

    OPPRINTF("STA $%.4x,X\n", addr);

    REG_PC = REG_PC+3;
    cycles = 5;

    break;
  case OPCODE_STA_ABS_Y:
    getAbsoluteY(REG_PC+1, &addr, &write_addr);

    sta(write_addr);

    OPPRINTF("STA $%.4x,Y\n", addr);

    REG_PC = REG_PC+3;
    cycles = 5;

    break;
  case OPCODE_STA_IND_X:
    getIndirectX(REG_PC+1, &addr, &write_addr);

    sta(write_addr);

    OPPRINTF("STA ($.2%x,X)\n", addr);

    REG_PC += 2;
    cycles = 6;

    break;
  case OPCODE_STA_IND_Y:
    getIndirectY(REG_PC+1, &addr, &write_addr);

    sta(write_addr);

    OPPRINTF("STA ($.2%x),Y\n", addr);

    REG_PC += 2;
    cycles = 6;

    break;
  case OPCODE_TXS:
    OPPRINTF("TXS\n");
    REG_SP = REG_X;
    REG_PC++;
    cycles = 2;

    break;
  case OPCODE_TSX:
    OPPRINTF("TSX\n");
    REG_X = REG_SP;
    REG_PC++;
    cycles = 2;

    break;
  case OPCODE_PHA:
    OPPRINTF("PHA\n");
    StackPushByte(REG_A);
    REG_PC++;
    cycles = 3;

    break;      
      
  case OPCODE_PLA:
    OPPRINTF("PLA\n");
    REG_A = StackPopByte();
    REG_PC++;
    cycles = 4;

    break;      

  case OPCODE_PHP:
    OPPRINTF("PHP\n");
    StackPushByte(REG_ST);
    REG_PC++;
    cycles = 3;

    break;      

  case OPCODE_PLP:
    OPPRINTF("PLP\n");
    REG_ST = StackPopByte();
    REG_PC++;
    cycles = 4;

    break;      
  case OPCODE_STX_ZERO:
    getZero(REG_PC+1, &write_addr);

    stx(write_addr);

    OPPRINTF("STX $%.2x\n", write_addr);

    REG_PC += 2;
    cycles = 3;

    break;
  case OPCODE_STX_ZERO_Y:
    getZeroY(REG_PC+1, &addr, &write_addr);

    stx(write_addr);

    OPPRINTF("LDX $%.2x,Y\n", addr);

    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_STX_ABS:
    getAbsolute(REG_PC+1, &write_addr);

    stx(write_addr);

    OPPRINTF("STX $%.4x\n", addr);

    REG_PC = REG_PC+3;
    cycles = 4;

    break;
  case OPCODE_STY_ZERO:
    getZero(REG_PC+1, &write_addr);

    sty(write_addr);

    OPPRINTF("STY $%.2x\n", write_addr);

    REG_PC += 2;
    cycles = 3;
           
    break;
  case OPCODE_STY_ZERO_X:
    getZeroX(REG_PC+1, &addr, &write_addr);

    sty(write_addr);

    OPPRINTF("STY $%.2x,X\n", addr);

    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_STY_ABS:
    getZeroX(REG_PC+1, &addr, &write_addr);

    sty(write_addr);

    OPPRINTF("STY $%.4x\n", addr);

    REG_PC = REG_PC+3;
    cycles = 4;
	    
    break;
  case 0x0C:
  case 0x1C:
  case 0x3C:
  case 0x5C:
  case 0x7C:
  case 0xDC:
  case 0xFC:
    // SKW    ***
    // SKW skips next word (two bytes).
    REG_PC = REG_PC+2;
    cycles = 4;
    break;
    
  default:
    printf("ERROR: UNKNOWN OPCODE:%d\n", opcode);

    break;
  }
  assert(cycles != 0);

  processor->cycles = cycles;

  return 0;
}



int
CpuCreate()
{
  processor = (Cpu*)calloc(1, sizeof(Cpu));

  if ( !processor )
    printf("%s: failed", __FUNCTION__);

  // initial regs
  REG_SP = 0xFF;

  // Start with the reset vector
  REG_PC = MemoryGetTwoBytesAt(RESET_VECTOR_ADDR);
  
  return 0;
}



void
CpuPrintRegs()
{
  printf("A: %.2x\n"
	 "X: %.2x\n"
	 "Y: %.2x\n"
	 "Flags: %.2x\n"
	 "Stack: %.2x\n",
	 REG_A,
	 REG_X,
	 REG_Y,
	 REG_ST,
	 REG_SP);
}

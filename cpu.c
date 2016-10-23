#include "includes.h"

char StackPopByte(void){
  return  memmap->memory[( STACK_MASK | processor->regs.sp++)];
}
ushort StackPopShort(void){
  ushort bytes = 0;
 ((char*)&bytes)[1] = memmap->memory[( STACK_MASK | processor->regs.sp++)];
 ((char*)&bytes)[0] = memmap->memory[( STACK_MASK | processor->regs.sp++)];
 return bytes;
}


void StackPushByte(unsigned char byte){
  memmap->memory[( STACK_MASK | --processor->regs.sp)] = byte;
}
void StackPushShort(ushort bytes){
  memmap->memory[( STACK_MASK | --processor->regs.sp)] = ((char*)&bytes)[0];
  memmap->memory[( STACK_MASK | --processor->regs.sp)] = ((char*)&bytes)[1];
}

#define OPPRINTF(...) printf(__VA_ARGS__)
// #define OPPRINTF(...) 

int
CpuCycle(){
  // get the next opcode
  unsigned char opcode = MemoryGetByteAt(REG_PC);
    
  // helper vars for calculations
  ushort tmp = 0;
  unsigned char arg1 = 0;
  unsigned char arg2 = 0;
  ushort addr = 0;
  ushort result = 0;
  int cycles = 0;


  if(--processor->cycles > 0){
    //printf("cpu taking cycles: %d\n", processor->cycles);
    return processor->cycles;
  }
      
  if(Wsync) {
    //printf("CPU PAUSED\n");
    return 0;
  }
    
  // printf("PC: %#x\n", REG_PC);
  //    printf("OPCODE:%#x\n", opcode);
    
  switch(opcode){
  case OPCODE_ADC_IMM:
    arg1 = processor->regs.accumulator;
    arg2 = getImmediate(REG_PC+1 );
    
    OPPRINTF("ADC #$%.2x\n", arg2);
    
    processor->regs.accumulator = add(arg1, arg2, FLAG_CARRY(REG_ST));
    
    REG_PC += 2;
    cycles = 2;
    break;
  case OPCODE_ADC_ZERO:
    
    arg1 = processor->regs.accumulator;
    arg2  = getZero(REG_PC+1, &tmp);

    OPPRINTF("ADC $%.2x\n", tmp);

    processor->regs.accumulator = add(arg1, arg2, FLAG_CARRY(REG_ST));

    REG_PC += 2;
    cycles = 3;
    break;

  case OPCODE_ADC_ZERO_X:

    arg1 = processor->regs.accumulator;
    arg2  = getZeroX(REG_PC+1, &tmp);

    OPPRINTF("ADC $%.2x,X\n", tmp);

    processor->regs.accumulator =
      add(arg1, arg2, FLAG_CARRY(REG_ST));

    REG_PC += 2;
    cycles = 4;
    break;
  case OPCODE_ADC_ABS:

    arg1 = processor->regs.accumulator;
    arg2  = getAbsolute(REG_PC+1, &tmp);

    OPPRINTF("ADC $%.4x\n", tmp);
    
    processor->regs.accumulator =
      add(arg1, arg2, FLAG_CARRY(REG_ST)); 

    REG_PC += 3;
    cycles = 4;
    
    break;

  case OPCODE_ADC_ABS_X:

    arg1 = processor->regs.accumulator;
    arg2  = getAbsoluteX(REG_PC+1, &tmp);
    
    OPPRINTF("ADC $%.4x,X\n", tmp);
    
    processor->regs.accumulator =
      add(arg1, arg2, FLAG_CARRY(REG_ST)); 

    REG_PC += 3;

    cycles = 4;

    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;
      
  case OPCODE_ADC_ABS_Y:

    arg1 = processor->regs.accumulator;
    arg2  = getAbsoluteX(REG_PC+1, &tmp);
    
    OPPRINTF("ADC $%.4x,Y\n", tmp);
    
    processor->regs.accumulator =
      add(arg1, arg2, FLAG_CARRY(REG_ST)); 

    REG_PC += 3;

    cycles = 4;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;
    break;
  case OPCODE_ADC_IND_X:
      
    arg1 = processor->regs.accumulator;
    arg2 = getIndirectX(REG_PC+1, &tmp);

    OPPRINTF("ADC ($.2%x,X)\n", tmp);
    
    processor->regs.accumulator =
      add(arg1, arg2, FLAG_CARRY(REG_ST)); 

    REG_PC += 2;
    cycles = 6;
    break;
      
  case OPCODE_ADC_IND_Y:
    assert(!FLAG_DECIMAL(REG_ST)); 
      
    arg1 = processor->regs.accumulator;
    arg2 = getIndirectX(REG_PC+1, &tmp);
    OPPRINTF("ADC ($.2%x),Y\n", tmp);
    
    processor->regs.accumulator =
      add(arg1, arg2, FLAG_CARRY(REG_ST)); 

    REG_PC += 2;

    cycles = 5;

    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;
  case OPCODE_AND_IMM:
    arg1 = processor->regs.accumulator;
    arg2 = getImmediate(REG_PC+1 );

    OPPRINTF("AND #$%.2x\n", arg2);

    processor->regs.accumulator = and(arg1, arg2);

    REG_PC += 2;

    cycles = 2;
    break;
    
  case OPCODE_AND_ZERO:
    arg1 = processor->regs.accumulator;
    arg2 = getZero(REG_PC+1, &tmp );

    OPPRINTF("AND $%.2x\n", tmp);

    processor->regs.accumulator = and(arg1, arg2);

    REG_PC += 2;

    cycles = 3;
    break;

  case OPCODE_AND_ZERO_X:
    arg1 = processor->regs.accumulator;
    arg2 = getZeroX(REG_PC+1, &tmp);

    OPPRINTF("AND $%.2x,X\n", tmp);

    processor->regs.accumulator = and(arg1, arg2);

    REG_PC += 2;
    cycles = 4;
    break;

  case OPCODE_AND_ABS:

    arg1 = processor->regs.accumulator;
    arg2 = getAbsolute(REG_PC+1, &tmp);
    
    OPPRINTF("AND $%.4x\n", addr);

    processor->regs.accumulator = and(arg1, arg2);

    REG_PC += 3;
    cycles = 4;
    break;
      
  case OPCODE_AND_ABS_X:
    arg1 = processor->regs.accumulator;

    arg2 = getAbsoluteX(REG_PC+1, &tmp);

    OPPRINTF("AND $%.4x,X\n", addr);

    processor->regs.accumulator = and(arg1, arg2);

    REG_PC += 3;
    cycles = 4;
    
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;
    break;
  case OPCODE_AND_ABS_Y:
    arg1 = processor->regs.accumulator;

    arg2 = getAbsoluteY(REG_PC+1, &tmp);

    OPPRINTF("AND $%.4x,Y\n", addr);

    processor->regs.accumulator = and(arg1, arg2);

    REG_PC += 3;
    cycles = 4;
    
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;
    
    break;

  case OPCODE_AND_IND_X:
    arg1 = processor->regs.accumulator;

    arg2 = getIndirectX(REG_PC+1, &tmp);

    OPPRINTF("AND ($.2%x,X)\n", addr);

    processor->regs.accumulator = and(arg1, arg2);

    REG_PC += 2;
    cycles = 6;
    break;

      
  case OPCODE_AND_IND_Y:
     
    arg1 = processor->regs.accumulator;

    arg2 = getIndirectY(REG_PC+1, &tmp);

    OPPRINTF("AND ($.2%x),Y\n", addr);

    processor->regs.accumulator = and(arg1, arg2);

    REG_PC += 2;
    cycles = 5;
    
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;
    break;

  case OPCODE_ASL_A:
    OPPRINTF("ASL A\n");
    arg1 = processor->regs.accumulator;

    processor->regs.accumulator = asl(arg1);

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
    arg1 = getZeroX(REG_PC+1, &addr);

    OPPRINTF("ASL $%.2x,X\n", addr);

    MemorySetByteAt(addr, asl(arg1));

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
    
    arg1 = getAbsoluteX(REG_PC+1, &addr);

    OPPRINTF("ASL $%.4x,X\n", addr);
    
    // write back to memory
    MemorySetByteAt(addr, asl(arg1));


    // advance pc
    REG_PC += 3;
    cycles = 7;
    break;
  case OPCODE_BIT_ZERO:
    arg1 = processor->regs.accumulator;
    arg2 = getZero(REG_PC+1, &addr);
    
    OPPRINTF("BIT $%.2x\n", addr);

    bit(arg1, arg2);

    REG_PC += 2;
    cycles = 3;
    break;
      
  case OPCODE_BIT_ABS:
    arg1 = processor->regs.accumulator;
    arg2 = getAbsolute(REG_PC+1, &addr);
    bit(arg1, arg2);

    OPPRINTF("BIT $%.4x\n", addr);

    REG_PC += 3;
    cycles = 4;
    break;

  case OPCODE_BPL:
    OPPRINTF("BPL\n");

    cycles = 2;
    // Note address if branch not taken
    tmp = REG_PC + 2;
      
    if(!FLAG_NEG(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((tmp & 0xF00) != (REG_PC & 0xF00))
	cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BMI:
    OPPRINTF("BMI\n");
    cycles = 2;

    if(FLAG_NEG(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((tmp & 0xF00) != (REG_PC & 0xF00))
	cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BVC:
    OPPRINTF("BVC\n");
    cycles = 2;
    // Note address if branch not taken
    tmp = REG_PC + 2;

    if(!FLAG_OVER(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((tmp & 0xF00) != (REG_PC & 0xF00))
	cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BVS:
    OPPRINTF("BVS\n");
    cycles = 2;
    // Note address if branch not taken
    tmp = REG_PC + 2;

    if(FLAG_OVER(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((tmp & 0xF00) != (REG_PC & 0xF00))
	cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BCC:
    OPPRINTF("BCC\n");
    cycles = 2;
    // Note address if branch not taken
    tmp = REG_PC + 2;

    if(!FLAG_CARRY(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((tmp & 0xF00) != (REG_PC & 0xF00))
	cycles++;

    }
    REG_PC += 2;					    
    break;
  case OPCODE_BCS:
    OPPRINTF("BCS\n");
    cycles = 2;
    // Note address if branch not taken
    tmp = REG_PC + 2;

    if(FLAG_CARRY(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      // Does branch cross page boundary?
      if((tmp & 0xF00) != (REG_PC & 0xF00))
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
    tmp = REG_PC + 2;
    if(!FLAG_ZERO(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));

      cycles++;
      // Does branch cross page boundary?
      if((tmp & 0xF00) != (REG_PC & 0xF00))
	cycles++;
    }
    REG_PC += 2;					    
    break;
  case OPCODE_BEQ:
    OPPRINTF("BEQ\n");
    cycles = 2;
    // Note address if branch not taken
    tmp = REG_PC + 2;

    if(FLAG_ZERO(REG_ST)) {
      REG_PC +=
	(signed char)(MemoryGetByteAt(REG_PC + 1));
      cycles++;
      // Does branch cross page boundary?
      if((tmp & 0xF00) != (REG_PC & 0xF00))
	cycles++;

    }
	 
    REG_PC += 2;					    
    break;
  case OPCODE_BRK:
    OPPRINTF("BRK\n");
    OPPRINTF("BREAK INSTRUCTION: Not Implemented!\n");
    //    assert(true == false);
    //programrunning = false;
    REG_PC = REG_PC+1;
    cycles = 7;

    break;
  case OPCODE_CMP_IMM:
    arg1 = processor->regs.accumulator;
    arg2 = getImmediate(REG_PC+1);

    cmp(arg1, arg2);

    OPPRINTF("CMP #$%.2x\n", arg2);

    REG_PC += 2;
    cycles = 2;

    break;
  case OPCODE_CMP_ZERO:
    arg1 = processor->regs.accumulator;
    arg2 = getZero(REG_PC+1, &addr);
    OPPRINTF("CMP $%.2x\n", addr);

    cmp(arg1, arg2);
    REG_PC += 2;
    cycles = 3;
	    
    break;
  case OPCODE_CMP_ZERO_X:
    arg1 = processor->regs.accumulator;
    arg2 = getZeroX(REG_PC+1, &addr);
    OPPRINTF("CMP $%.2x,X\n", tmp);

    cmp(arg1, arg2);

    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_CMP_ABS:
    arg1 = processor->regs.accumulator;
    arg2 = getAbsolute(REG_PC+1, &addr);

    cmp(arg1, arg2);

    OPPRINTF("CMP $%.4x\n", addr);

    REG_PC += 3;
    cycles = 4;

    break;
  case OPCODE_CMP_ABS_X:
    arg1 = processor->regs.accumulator;
    arg2 = getAbsoluteX(REG_PC+1, &addr);

    cmp(arg1, arg2);
    OPPRINTF("CMP $%.4x,X\n", addr);

    REG_PC += 3;
    cycles = 4;
    
    if (((addr  & 0xFF) + processor->regs.x) > 0x100)
      cycles++;

    break;
  case OPCODE_CMP_ABS_Y:
    arg1 = processor->regs.accumulator;
    arg2 = getAbsoluteY(REG_PC+1, &addr);

    cmp(arg1, arg2);
    OPPRINTF("CMP $%.4x,Y\n", addr);

    REG_PC += 3;
    cycles = 4;

    if (((addr  & 0xFF) + processor->regs.y) > 0x100)
      cycles++;
    break;

  case OPCODE_CMP_IND_X:
    arg1 = processor->regs.accumulator;
    arg2 = getIndirectX(REG_PC+1, &addr);

    cmp(arg1, arg2);
    OPPRINTF("CMP ($.2%x,X)\n", addr);

    REG_PC += 2;
    cycles = 6;

    break;
     
  case OPCODE_CMP_IND_Y:
    arg1 = processor->regs.accumulator;
    arg2 = getIndirectY(REG_PC+1, &addr);

    cmp(arg1, arg2);
    OPPRINTF("CMP ($.2%x),Y\n", addr);

    REG_PC += 2;
    cycles = 5;

    if (((addr  & 0xFF) + processor->regs.y) > 0x100)
      cycles++;
    break;

  case OPCODE_CPX_IMM:
    arg1 = processor->regs.x;
    arg2 = getImmediate(REG_PC+1);

    cmp(arg1, arg2);

    OPPRINTF("CPX #$%.2x\n", arg2);

    REG_PC += 2;
    cycles = 2;

    break;
  case OPCODE_CPX_ZERO:
    arg1 = processor->regs.x;
    arg2 = getZero(REG_PC+1, &addr);
    OPPRINTF("CPX $%.2x\n", addr);

    cmp(arg1, arg2);
    REG_PC += 2;
    cycles = 3;
	    
    break;

  case OPCODE_CPX_ABS:
    arg1 = processor->regs.x;
    arg2 = getAbsolute(REG_PC+1, &addr);

    cmp(arg1, arg2);

    OPPRINTF("CPX $%.4x\n", addr);

    REG_PC += 3;
    cycles = 4;

    break;


  case OPCODE_CPY_IMM:
    arg1 = processor->regs.y;
    arg2 = getImmediate(REG_PC+1);

    cmp(arg1, arg2);

    OPPRINTF("CPY #$%.2x\n", arg2);

    REG_PC += 2;
    cycles = 2;

    break;
 case OPCODE_CPY_ZERO:
    arg1 = processor->regs.y;
    arg2 = getZero(REG_PC+1, &addr);
    OPPRINTF("CMP $%.2x\n", addr);

    cmp(arg1, arg2);
    REG_PC += 2;
    cycles = 3;
	    
    break;
  case OPCODE_CPY_ABS:
    arg1 = processor->regs.y;
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
    arg1 = getZeroX(REG_PC+1, &addr);
 
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
    arg1 = getAbsoluteX(REG_PC+1, &addr);
 
    dec(arg1, addr);

    OPPRINTF("DEC $%.4x,X\n", addr);

    REG_PC += 3;
    cycles = 7;

    break;

  case OPCODE_EOR_IMM:
    arg1 = processor->regs.accumulator;
    arg2 = getImmediate(REG_PC+1 );
    
    processor->regs.accumulator = eor(arg1, arg2);
    OPPRINTF("EOR #$%.2x\n", arg2);

    REG_PC += 2;
    cycles = 2;

    break;

  case OPCODE_EOR_ZERO:
    arg1 = processor->regs.accumulator;
    arg2 = getZero(REG_PC+1, &addr );
    
    processor->regs.accumulator = eor(arg1, arg2);
    OPPRINTF("EOR $%.2x\n", addr);

    REG_PC += 2;
    cycles = 3;

    break;

  case OPCODE_EOR_ZERO_X:
    arg1 = processor->regs.accumulator;
    arg2 = getZeroX(REG_PC+1, &addr );
    
    processor->regs.accumulator = eor(arg1, arg2);
    OPPRINTF("EOR $%.2x,X\n", addr);

    REG_PC += 2;
    cycles = 4;

    break;
      
  case OPCODE_EOR_ABS:
    arg1 = processor->regs.accumulator;
    arg2 = getAbsolute(REG_PC+1, &addr );
    
    processor->regs.accumulator = eor(arg1, arg2);
    OPPRINTF("EOR $%.4x\n", addr);
 
    REG_PC += 3;
    cycles = 4;

    break;
      
  case OPCODE_EOR_ABS_X:
    arg1 = processor->regs.accumulator;

    arg2 = getAbsoluteX(REG_PC+1, &addr );
    
    processor->regs.accumulator = eor(arg1, arg2);

    OPPRINTF("EOR $%.4x,X\n", addr);

    REG_PC += 3;
    cycles = 4;

    if (((addr  & 0xFF) + REG_X) > 0x100)
      cycles++;

    break;
      
  case OPCODE_EOR_ABS_Y:
    arg1 = REG_A;

    arg2 = getAbsoluteY(REG_PC+1, &addr );
    
    REG_A = eor(arg1, arg2);

    OPPRINTF("EOR $%.4x,Y\n", addr);

    REG_PC += 3;
    
    cycles = 4;
    if (((addr  & 0xFF) + REG_Y) > 0x100)
      cycles++;

    break;

  case OPCODE_EOR_IND_X:
    arg1 = processor->regs.accumulator;

    arg2 = getIndirectX(REG_PC+1, &addr );
    
    REG_A = eor(arg1, arg2);

    OPPRINTF("EOR $%.4x,Y\n", addr);

    REG_PC += 2;
    cycles = 6;
            
    break;
  case OPCODE_EOR_IND_Y:
    arg1 = processor->regs.accumulator;

    arg2 = getIndirectY(REG_PC+1, &addr );
    
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
    arg1 = getZeroX(REG_PC+1, &addr);
 
    inc(arg1, addr);

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
    arg1 = getAbsoluteX(REG_PC+1, &addr);
 
    inc(arg1, addr);

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
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("JSR $%.4x\n", addr);

    StackPushShort(REG_PC + 2);
    REG_PC = addr;
      
    cycles = 6;
	    
    break;
  case OPCODE_LDA_IMM:
    arg2 = MemoryGetByteAt(REG_PC+1 );
    OPPRINTF("LDA #$%.2x\n", arg2);

    processor->regs.accumulator = arg2;
    REG_PC += 2;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    cycles = 2;

    break;
  case OPCODE_LDA_ZERO:
    tmp = MemoryGetByteAt(REG_PC+1 );

    OPPRINTF("LDA $%.2x\n", tmp);

    processor->regs.accumulator = MemoryGetByteAt(tmp);
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 3;

    break;
  case OPCODE_LDA_ZERO_X:
    tmp = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("LDA $%.2x,X\n", tmp);

    tmp += processor->regs.x;
    tmp &= 0xFF;
    processor->regs.accumulator = MemoryGetByteAt(tmp);
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_LDA_ABS:
    tmp = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("LDA $%.4x\n", tmp);

    processor->regs.accumulator = MemoryGetByteAt(tmp );
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC = REG_PC+3;
    cycles = 4;

    break;
  case OPCODE_LDA_ABS_X:
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("LDA $%.4x,X\n", addr);

    //    addr += processor->regs.x;
    tmp = processor->regs.x;
    processor->regs.accumulator = MemoryGetByteAt( addr + tmp );
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC = REG_PC+3;
    cycles = 4;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;
    break;
  case OPCODE_LDA_ABS_Y:
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("LDA $%.4x,Y\n", addr);

    tmp = processor->regs.y;
    processor->regs.accumulator = MemoryGetByteAt(tmp + addr);
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC = REG_PC+3;
    cycles = 4;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;
  case OPCODE_LDA_IND_X:
    tmp = MemoryGetByteAt(REG_PC+1 );
    OPPRINTF("LDA ($.2%x,X)\n", tmp);

    tmp += processor->regs.x;
    processor->regs.accumulator = MemoryGetByteAt(MemoryGetTwoBytesAt(tmp) );
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 6;

    break;
  case OPCODE_LDA_IND_Y:
    addr =  MemoryGetByteAt(REG_PC+1);
    OPPRINTF("LDA ($.2%x),Y\n", addr);

    addr = MemoryGetTwoBytesAt(addr);
    tmp  = processor->regs.y;
    processor->regs.accumulator = MemoryGetByteAt(tmp + addr);
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 5;

    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;
  case OPCODE_LDX_IMM:
    arg2 = MemoryGetByteAt(REG_PC+1);
    MemoryGetByteAt(REG_PC+1 );
    processor->regs.x = arg2;
    OPPRINTF("LDX #$%.2x\n", arg2);

    //    OPPRINTF("x: %#x\n", processor->regs.x);
    SETSIGN(processor->regs.x);
    SETZERO(processor->regs.x);
    REG_PC += 2;
    cycles = 2;

    break;
  case OPCODE_LDX_ZERO:
    tmp = MemoryGetByteAt(REG_PC+1 );
    OPPRINTF("LDX $%.2x\n", tmp);
    processor->regs.x = MemoryGetByteAt(tmp);
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 3;

    break;

  case OPCODE_LDX_ZERO_Y:
    tmp = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("LDX $%.2x,Y\n", tmp);

    tmp += processor->regs.y;
    processor->regs.x = MemoryGetByteAt(tmp);
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 4;

    break;

  case OPCODE_LDX_ABS:
    tmp = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("LDX $%.4x\n", tmp);

    processor->regs.x = MemoryGetByteAt(tmp );
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC = REG_PC+3;
    cycles = 4;

    break;
      
  case OPCODE_LDX_ABS_Y:

    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("LDX $%.4x,Y\n", addr);

    tmp = processor->regs.y;

    // get byte
    arg2 = MemoryGetByteAt(addr + tmp);

    processor->regs.x = arg2;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC = REG_PC+3;
    cycles = 4;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;
  case OPCODE_LDY_IMM:
    arg2 = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("LDY #$%.2x\n", arg2);
    
    processor->regs.y = arg2;
    SETSIGN(processor->regs.y);
    SETZERO(processor->regs.y);
    REG_PC += 2;
    cycles = 2;

    break;
  case OPCODE_LDY_ZERO:
    tmp = MemoryGetByteAt(REG_PC+1 );

    OPPRINTF("LDY $%.2x\n", tmp);

    processor->regs.y = MemoryGetByteAt(tmp);
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 3;

    break;

  case OPCODE_LDY_ZERO_X:
    tmp = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("LDY $%.2x,X\n", tmp);
	
    tmp += processor->regs.x;
    tmp &= 0xFF;
    processor->regs.y = MemoryGetByteAt(tmp);
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 4;

    break;

  case OPCODE_LDY_ABS:
    tmp = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("LDY $%.4x\n", tmp);

    processor->regs.y = MemoryGetByteAt(tmp );
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC = REG_PC+3;
    cycles = 4;

    break;
  case OPCODE_LDY_ABS_X:
    // calculate address
    addr  = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("LDY $%.4x,X\n", addr);

    tmp = processor->regs.x;

    // get byte
    arg2 = MemoryGetByteAt(tmp + addr);

    processor->regs.y = arg2;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC = REG_PC+3;
    cycles = 4;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;
  case OPCODE_LSR_A:
    OPPRINTF("LSR A\n");
    arg1 = processor->regs.accumulator;
    processor->regs.accumulator = 0x7F & (arg1 >> 1);
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    if(arg1 & 0xFE)
      FLAG_CARRY_SET(REG_ST);
    else
      FLAG_CARRY_CLEAR(REG_ST);
    REG_PC += 1;
    cycles = 2;

    break;
   
  case OPCODE_LSR_ZERO:
    addr = MemoryGetByteAt(REG_PC+1 );

    OPPRINTF("LSR $%.2x\n", addr);
    
    arg1 = MemoryGetByteAt(addr);
    result = 0x7F & (arg1 >> 1);
    MemorySetByteAt(addr,  (char)result);
    SETSIGN((char)result);
    SETZERO((char)result);
    if(arg1 & 0xFE)
      FLAG_CARRY_SET(REG_ST);
    else
      FLAG_CARRY_CLEAR(REG_ST);
    REG_PC += 2;
    cycles = 5;

    break;
      
  case OPCODE_LSR_ZERO_X:
    // calculate addr
    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("LSR $%.2x,X\n", addr);

    addr += processor->regs.x;
    addr &= 0xFF;
    // get byte
    arg1 = MemoryGetByteAt(addr);

    // shift byte
    result = 0x7F & (arg1 >> 1);

    // write back to memory
    MemorySetByteAt(addr, (char)result);

    // set flags
    SETSIGN((char)result);
    SETZERO((char)result);
    if(arg1 & 0xFE)
      FLAG_CARRY_SET(REG_ST);
    else
      FLAG_CARRY_CLEAR(REG_ST);

    // advance pc past used bytes
    REG_PC += 2;
    cycles = 6;

    break;

  case OPCODE_LSR_ABS:
    // get addr
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("LSR $%.4x\n", addr);

    // get byte
    arg1 = MemoryGetByteAt(addr);

    // operate on byte
    result = 0x7F & (arg1 >> 1);

    // write back to memory
    MemorySetByteAt(addr, (char)result);

    // set signs
    SETSIGN((char)result);
    SETZERO((char)result);
    if(arg1 & 0xFE)
      FLAG_CARRY_SET(REG_ST);
    else
      FLAG_CARRY_CLEAR(REG_ST);

    // advance pc
    REG_PC += 3;
    cycles = 6;

    break;
      
  case OPCODE_LSR_ABS_X:
    // calculate addr
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("LSR $%.4x,X\n", addr);

    addr += processor->regs.x;
      
    // get byte
    arg1 = MemoryGetByteAt(addr);

    // operate on byte
    result = 0x7F & (arg1 >> 1);

    // write back to memory
    MemorySetByteAt(addr, (char)result);

    // set signs
    SETSIGN((char)result);
    SETZERO((char)result);
    if(arg1 & 0xFE)
      FLAG_CARRY_SET(REG_ST);
    else
      FLAG_CARRY_CLEAR(REG_ST);

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
    arg2 = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("ORA #$%.2x\n", arg2);

    processor->regs.accumulator |= MemoryGetByteAt(REG_PC+1);
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 2;
            
    break;
  case OPCODE_ORA_ZERO:
    addr = MemoryGetByteAt(REG_PC+1 );
    OPPRINTF("ORA $%.2x\n", addr);

    
    arg1 = processor->regs.accumulator;
    arg2 = MemoryGetByteAt(addr);
    result = arg1 | arg2;
    processor->regs.accumulator = result;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 3;

    break;
  case OPCODE_ORA_ZERO_X:
    tmp = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("ORA $%.2x,X\n", tmp);

    tmp += processor->regs.x;
    tmp &= 0xFF;
    arg1 = processor->regs.accumulator;
    arg2 = MemoryGetByteAt(tmp);
    result = arg1 | arg2;
    processor->regs.accumulator = result;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_ORA_ABS:
    arg1 = processor->regs.accumulator;

    // get address
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("ORA $%.4x\n", addr);

    // get byte
    arg2 = MemoryGetByteAt(addr);

    result = arg1 | arg2;
      
    processor->regs.accumulator = result;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 3;
    cycles = 4;

    break;
      
  case OPCODE_ORA_ABS_X:
    arg1 = processor->regs.accumulator;

    // Get base address
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("ORA $%.4x,X\n", addr);

    tmp = processor->regs.x;

    // get byte
    arg2 = MemoryGetByteAt(addr + tmp);

    result = arg1 | arg2;
    processor->regs.accumulator = result;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 3;
    cycles = 4;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;

  case OPCODE_ORA_ABS_Y:
    arg1 = processor->regs.accumulator;

    // Get base address
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("ORA $%.4x,Y\n", addr);

    tmp = processor->regs.y;

    // get byte
    arg2 = MemoryGetByteAt(addr + tmp);

    result = arg1 | arg2;
      
    processor->regs.accumulator = result;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 3;
    cycles = 4;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;

  case OPCODE_ORA_IND_X:

    arg1 = processor->regs.accumulator;

    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("ORA ($.2%x,X)\n", addr);

    addr += processor->regs.x;
    addr = MemoryGetTwoBytesAt(addr);
    
    arg2 = MemoryGetByteAt(addr);
   
    result = arg1 | arg2;
    processor->regs.accumulator = result;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 6;

    break;

  case OPCODE_ORA_IND_Y:
    arg1 = processor->regs.accumulator;

    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("ORA ($.2%x),Y\n", addr);

    addr = MemoryGetTwoBytesAt(addr);
    tmp = processor->regs.y;
    
    arg2 = MemoryGetByteAt(addr + tmp);

    result = arg1 | arg2;
    processor->regs.accumulator = result;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 5;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;

  case OPCODE_TAX:
    OPPRINTF("TAX\n");
    processor->regs.x = processor->regs.accumulator;
    REG_PC = REG_PC+1;
    SETSIGN(processor->regs.x);
    SETZERO(processor->regs.x);
    cycles = 2;
	    
    break;
  case OPCODE_TXA:
    OPPRINTF("TXA\n");
    processor->regs.accumulator = processor->regs.x;
    REG_PC = REG_PC+1;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    cycles = 2;

    break;
  case OPCODE_DEX:
    OPPRINTF("DEX\n");
    processor->regs.x--;
    REG_PC = REG_PC+1;
    SETSIGN(processor->regs.x);
    SETZERO(processor->regs.x);
    cycles = 2;

    break;
  case OPCODE_INX:
    OPPRINTF("INX\n");
    processor->regs.x++;
    //    OPPRINTF("x: %#x\n", processor->regs.x);
    REG_PC = REG_PC+1;
    SETSIGN(processor->regs.x);
    SETZERO(processor->regs.x);
    cycles = 2;

    break;
  case OPCODE_TAY:
    OPPRINTF("TAY\n");
    processor->regs.y = processor->regs.accumulator;
    REG_PC = REG_PC+1;
    SETSIGN(processor->regs.y);
    SETZERO(processor->regs.y);
    cycles = 2;

    break;
  case OPCODE_TYA:
    OPPRINTF("TYA\n");
    processor->regs.accumulator = processor->regs.y;
    REG_PC = REG_PC+1;
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    cycles = 2;

    break;
  case OPCODE_DEY:
    OPPRINTF("DEY\n");
    processor->regs.y--;
    REG_PC = REG_PC+1;
    SETSIGN(processor->regs.y);
    SETZERO(processor->regs.y);
    cycles = 2;

    break;
  case OPCODE_INY:
    OPPRINTF("INY\n");
    processor->regs.y++;
    REG_PC = REG_PC+1;
    SETSIGN(processor->regs.y);
    SETZERO(processor->regs.y);
    cycles = 2;

    break;
  case OPCODE_ROL_A:
    OPPRINTF("ROL A\n");
    // get byte from memory
    result = processor->regs.accumulator;
    // Calculate new value
    result = result << 1;
    // Set lowest bit according to carry flag
    if(FLAG_CARRY(REG_ST))
      result |= 0x1;
    else
      result &= 0xFE;

    // write back value
    processor->regs.accumulator = (char)result;

    // set flags
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    SETCARRY(result);
    // increment pc
    REG_PC += 1;
    cycles = 2;

    break;
      
  case OPCODE_ROL_ZERO:
    // get byte from memory
    addr = MemoryGetByteAt(REG_PC+1 );

    OPPRINTF("ROL $%.2x\n", addr);

    arg1 = MemoryGetByteAt(addr);

    // Calculate new value
    result = result << 1;
    // Set lowest bit according to carry flag
    if(FLAG_CARRY(REG_ST))
      result |= 0x1;
    else
      result &= 0xFE;

    // write back value
    MemorySetByteAt(addr, (char)result);

    // set flags
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    SETCARRY(result);
    // increment pc
    REG_PC += 2;
    cycles = 5;

    break;
  case OPCODE_ROL_ZERO_X:
    // calculate addr
    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("ROL $%.2x,X\n", addr);

    addr += processor->regs.x;
    addr &= 0xFF;
    // get byte from memory
    arg1 = MemoryGetByteAt(addr);

    // Calculate new value
    result = result << 1;
    // Set lowest bit according to carry flag
    if(FLAG_CARRY(REG_ST))
      result |= 0x1;
    else
      result &= 0xFE;

    // write back value
    MemorySetByteAt(addr, (char)result);

    // set flags
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    SETCARRY(result);
    // increment pc
    REG_PC += 2;
    cycles = 6;

    break;

  case OPCODE_ROL_ABS:
    // get addr
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("ROL $%.4x\n", addr);

    // get byte
    arg1 = MemoryGetByteAt(addr);

    // operate on byte
    result = arg1 << 1;

    // Set lowest bit according to carry flag
    if(FLAG_CARRY(REG_ST))
      result |= 0x1;
    else
      result &= 0xFE;

    // write back to memory
    MemorySetByteAt(addr, (char)result);

    // set signs
    SETSIGN((char)result);
    SETZERO((char)result);
    SETCARRY(result);

    // advance pc
    REG_PC += 3;
    cycles = 6;

    break;

  case OPCODE_ROL_ABS_X:
    // calculate addr
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("ROL $%.4x,X\n", addr);

    addr += processor->regs.x;
      
    // get byte
    arg1 = MemoryGetByteAt(addr);

    // operate on byte
    result = arg1 << 1;

    // Set lowest bit according to carry flag
    if(FLAG_CARRY(REG_ST))
      result |= 0x1;
    else
      result &= 0xFE;

    // write back to memory
    MemorySetByteAt(addr, (char)result);

    // set signs
    SETSIGN((char)result);
    SETZERO((char)result);
    SETCARRY(result);

    // advance pc
    REG_PC += 3;
    cycles = 7;

    break;

  case OPCODE_ROR_A:
    OPPRINTF("ROR A\n");

    // get byte from memory
    arg1 = processor->regs.accumulator;

    // Calculate new value
    result = arg1 >> 1;

    // Set lowest bit according to carry flag
    if(FLAG_CARRY(REG_ST))
      result |= 0x80;
    else
      result &= 0x7F;

    // write back value
    processor->regs.accumulator = (char)result;

    // set flags
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    if(arg1 & 0xFE)
      FLAG_CARRY_SET(REG_ST);
    else
      FLAG_CARRY_CLEAR(REG_ST);

    // increment pc
    REG_PC += 1;
    cycles = 2;

    break;

  case OPCODE_ROR_ZERO:
    // get byte from memory
    addr = MemoryGetByteAt(REG_PC+1 );
    OPPRINTF("ROR $%.2x\n", addr);

    
    arg1 = MemoryGetByteAt(addr);

    // Calculate new value
    result = arg1 >> 1;

    // Set lowest bit according to carry flag
    if(FLAG_CARRY(REG_ST))
      result |= 0x80;
    else
      result &= 0x7F;

    // write back value
    MemorySetByteAt(addr, (char)result);

    // set flags
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    SETCARRY(result);
    // increment pc
    REG_PC += 2;
    cycles = 5;

    break;
  case OPCODE_ROR_ZERO_X:
    // calculate addr
    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("ROR $%.2x,X\n", addr);

    addr += processor->regs.x;
    addr &= 0xFF;
    // get byte from memory
    arg1 = MemoryGetByteAt(addr);

    // Calculate new value
    result = arg1 >> 1;

    // Set lowest bit according to carry flag
    if(FLAG_CARRY(REG_ST))
      result |= 0x80;
    else
      result &= 0x7F;

    // write back value
    MemorySetByteAt(addr, (char)result);

    // set flags
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    SETCARRY(result);
    // increment pc
    REG_PC += 2;
    cycles = 6;

    break;

  case OPCODE_ROR_ABS:
    // get addr
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("ROR $%.4x\n", addr);

    // get byte
    arg1 = MemoryGetByteAt(addr);

    // operate on byte
    result = arg1 >> 1;

    // Set lowest bit according to carry flag
    if(FLAG_CARRY(REG_ST))
      result |= 0x80;
    else
      result &= 0x7F;

    // write back to memory
    MemorySetByteAt(addr, (char)result);

    // set signs
    SETSIGN((char)result);
    SETZERO((char)result);
    SETCARRY(result);

    // advance pc
    REG_PC += 3;
    cycles = 6;
    break;

  case OPCODE_ROR_ABS_X:
    // calculate addr
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("ROR $%.4x,X\n", addr);

    addr += processor->regs.x;
      
    // get byte
    arg1 = MemoryGetByteAt(addr);

    // operate on byte
    result = arg1 >> 1;

    // Set lowest bit according to carry flag
    if(FLAG_CARRY(REG_ST))
      result |= 0x80;
    else
      result &= 0x7F;

    // write back to memory
    MemorySetByteAt(addr, (char)result);

    // set signs
    SETSIGN((char)result);
    SETZERO((char)result);
    SETCARRY(result);

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
    assert(!FLAG_DECIMAL(REG_ST)); 
      
    // Get minuend 
    arg1 = processor->regs.accumulator;

    // Get subtrahend
    arg2 = MemoryGetByteAt(REG_PC+1);

    OPPRINTF("SBC #$%.2x\n", arg2);

    // Get its twos compliment
    // Negate the bits
    arg2 = ~arg2;

    // Add one unless (not) carry indicates that borrow occurred
    // thus the difference will be one less.
    if (FLAG_CARRY(REG_ST)){
      tmp = 1;
    }
    else {
      tmp = 0;
    }

    // Subraction of two's comp is really just an add.
    result = arg1 + arg2 + tmp;

    processor->regs.accumulator = result;

    // Carry is reverse borrow.
    if( result > 255 )
      FLAG_CARRY_CLEAR(REG_ST);
    else
      FLAG_CARRY_SET(REG_ST);

    // Magic makes overflow logic work as if it's an add.
    SETOVER(arg1, arg2, result);
           
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 2;
    break;
  case OPCODE_SBC_ZERO:
    assert(!FLAG_DECIMAL(REG_ST)); 

    arg1 = processor->regs.accumulator;

    addr = MemoryGetByteAt(REG_PC+1 );
    OPPRINTF("SBC $%.2x\n", addr);
    
    arg2 = MemoryGetByteAt(addr);

    // Get its twos compliment
    // Negate the bits
    arg2 = ~arg2;

    // Add one unless (not) carry indicates that borrow occurred
    // thus the difference will be one less.
    if (FLAG_CARRY(REG_ST)){
      tmp = 1;
    }
    else {
      tmp = 0;
    }

    // Subraction of two's comp is really just an add.
    result = arg1 + arg2 + tmp;

    processor->regs.accumulator = result;

    // Carry is reverse borrow.
    if( result > 255 )
      FLAG_CARRY_CLEAR(REG_ST);
    else
      FLAG_CARRY_SET(REG_ST);

    // Magic makes overflow logic work as if it's an add.
    SETOVER(arg1, arg2, result);
      
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 3;
    break;

  case OPCODE_SBC_ZERO_X:
    assert(!FLAG_DECIMAL(REG_ST)); 

    arg1 = processor->regs.accumulator;
    
    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("SBC $%.2x,X\n", addr);

    addr += processor->regs.x;
    addr &= 0xFF;
    // get byte from memory
    arg2 = MemoryGetByteAt(addr);

    // Get its twos compliment
    // Negate the bits
    arg2 = ~arg2;

    // Add one unless (not) carry indicates that borrow occurred
    // thus the difference will be one less.
    if (FLAG_CARRY(REG_ST)){
      tmp = 1;
    }
    else {
      tmp = 0;
    }

    // Subraction of two's comp is really just an add.
    result = arg1 + arg2 + tmp;

    processor->regs.accumulator = result;

    // Carry is reverse borrow.
    if( result > 255 )
      FLAG_CARRY_CLEAR(REG_ST);
    else
      FLAG_CARRY_SET(REG_ST);

    // Magic makes overflow logic work as if it's an add.
    SETOVER(arg1, arg2, result);
           
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 4;
	    
    break;
   
  case OPCODE_SBC_ABS:
    assert(!FLAG_DECIMAL(REG_ST)); 

    arg1 = processor->regs.accumulator;

    // get addr
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("SBC $%.4x\n", addr);

    // get byte
    arg2 = MemoryGetByteAt(addr);

    // Get its twos compliment
    // Negate the bits
    arg2 = ~arg2;

    // Add one unless (not) carry indicates that borrow occurred
    // thus the difference will be one less.
    if (FLAG_CARRY(REG_ST)){
      tmp = 1;
    }
    else {
      tmp = 0;
    }

    // Subraction of two's comp is really just an add.
    result = arg1 + arg2 + tmp;

    processor->regs.accumulator = result;

    // Carry is reverse borrow.
    if( result > 255 )
      FLAG_CARRY_CLEAR(REG_ST);
    else
      FLAG_CARRY_SET(REG_ST);

    // Magic makes overflow logic work as if it's an add.
    SETOVER(arg1, arg2, result);

    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 3;
    cycles = 4;

    break;

  case OPCODE_SBC_ABS_X:
    assert(!FLAG_DECIMAL(REG_ST)); 

    arg1 = processor->regs.accumulator;
	    
    // calculate addr
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("SBC $%.4x,X\n", addr);

    tmp = processor->regs.x;
      
    // get byte
    arg2 = MemoryGetByteAt(addr + tmp);

    // Get its twos compliment
    // Negate the bits
    arg2 = ~arg2;

    // Add one unless (not) carry indicates that borrow occurred
    // thus the difference will be one less.
    if (FLAG_CARRY(REG_ST)){
      tmp = 1;
    }
    else {
      tmp = 0;
    }

    // Subraction of two's comp is really just an add.
    result = arg1 + arg2 + tmp;

    processor->regs.accumulator = result;

    // Carry is reverse borrow.
    if( result > 255 )
      FLAG_CARRY_CLEAR(REG_ST);
    else
      FLAG_CARRY_SET(REG_ST);

    // Magic makes overflow logic work as if it's an add.
    SETOVER(arg1, arg2, result);
           
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 3;
    cycles = 4;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;

  case OPCODE_SBC_ABS_Y:
    assert(!FLAG_DECIMAL(REG_ST)); 

    arg1 = processor->regs.accumulator;
	    
    // calculate addr
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("SBC $%.4x,Y\n", addr);

    tmp = processor->regs.y;

    // get byte
    arg2 = MemoryGetByteAt(addr + tmp);

    // Get its twos compliment
    // Negate the bits
    arg2 = ~arg2;

    // Add one unless (not) carry indicates that borrow occurred
    // thus the difference will be one less.
    if (FLAG_CARRY(REG_ST)){
      tmp = 1;
    }
    else {
      tmp = 0;
    }

    // Subraction of two's comp is really just an add.
    result = arg1 + arg2 + tmp;

    processor->regs.accumulator = result;

    // Carry is reverse borrow.
    if( result > 255 )
      FLAG_CARRY_CLEAR(REG_ST);
    else
      FLAG_CARRY_SET(REG_ST);

    // Magic makes overflow logic work as if it's an add.
    SETOVER(arg1, arg2, result);
           
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 3;
    cycles = 4;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;

  case OPCODE_SBC_IND_X:
    assert(!FLAG_DECIMAL(REG_ST)); 

      
    arg1 = processor->regs.accumulator;
	    
    // calculate addr
      
    // get the operand
    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("SBC ($.2%x,X)\n", addr);

    // add value of x to it
    addr += processor->regs.x;

    // retrieve address 
    addr = MemoryGetTwoBytesAt(addr);

    // get byte
    arg2 = MemoryGetByteAt(addr);

    // Get its twos compliment
    // Negate the bits
    arg2 = ~arg2;

    // Add one unless (not) carry indicates that borrow occurred
    // thus the difference will be one less.
    if (FLAG_CARRY(REG_ST)){
      tmp = 1;
    }
    else {
      tmp = 0;
    }

    // Subraction of two's comp is really just an add.
    result = arg1 + arg2 + tmp;

    processor->regs.accumulator = result;

    // Carry is reverse borrow.
    if( result > 255 )
      FLAG_CARRY_CLEAR(REG_ST);
    else
      FLAG_CARRY_SET(REG_ST);

    // Magic makes overflow logic work as if it's an add.
    SETOVER(arg1, arg2, result);
           
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 6;

    break;
      
  case OPCODE_SBC_IND_Y:
    assert(!FLAG_DECIMAL(REG_ST)); 

    arg1 = processor->regs.accumulator;
	    
    // calculate addr

    // Get the operand
    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("SBC ($.2%x),Y\n", addr);


    // Get the base memory address
    addr = MemoryGetTwoBytesAt(addr);

    // Add y to the addr
    tmp = processor->regs.y;

    // get byte
    arg2 = MemoryGetByteAt(addr + tmp);

    // Get its twos compliment
    // Negate the bits
    arg2 = ~arg2;

    // Add one unless (not) carry indicates that borrow occurred
    // thus the difference will be one less.
    if (FLAG_CARRY(REG_ST)){
      tmp = 1;
    }
    else {
      tmp = 0;
    }

    // Subraction of two's comp is really just an add.
    result = arg1 + arg2 + tmp;

    processor->regs.accumulator = result;

    // Carry is reverse borrow.
    if( result > 255 )
      FLAG_CARRY_CLEAR(REG_ST);
    else
      FLAG_CARRY_SET(REG_ST);

    // Magic makes overflow logic work as if it's an add.
    SETOVER(arg1, arg2, result);
           
    SETSIGN(processor->regs.accumulator);
    SETZERO(processor->regs.accumulator);
    REG_PC += 2;
    cycles = 5;
    if (((addr  & 0xFF) + tmp) > 0x100)
      cycles++;

    break;

  case OPCODE_STA_ZERO:
    arg1 = processor->regs.accumulator;
    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("STA $%.2x\n", addr);
    
    MemorySetByteAt(addr, arg1);
    // memmap->memory[addr] = arg1;
    REG_PC += 2;
    cycles = 3;

    break;
  case OPCODE_STA_ZERO_X:
    arg1 = processor->regs.accumulator;
    arg2 = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("STA $%.2x,X\n", arg2);

    addr = arg2 + processor->regs.x;
    addr &= 0xFF;
    //printf("arg1: %#2x, arg2: %#2x\n", arg1, arg2);
    // memmap->memory[addr] = arg1;
    MemorySetByteAt(addr, arg1);
    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_STA_ABS:
    arg1 = processor->regs.accumulator;
    addr =  MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("STA $%.4x\n", addr);

    MemorySetByteAt(addr, arg1);
    REG_PC = REG_PC+3;
    cycles = 4;

    break;
  case OPCODE_STA_ABS_X:
    arg1 = processor->regs.accumulator;
    addr = MemoryGetTwoBytesAt(REG_PC+1);

    OPPRINTF("STA $%.4x,X\n", addr);

    addr += processor->regs.x;
    MemorySetByteAt(addr, arg1);
    REG_PC = REG_PC+3;
    cycles = 5;

    break;
  case OPCODE_STA_ABS_Y:
    arg1 = processor->regs.accumulator;
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("STA $%.4x,Y\n", addr);

    addr  +=   processor->regs.y;
    MemorySetByteAt(addr, arg1);
    REG_PC = REG_PC+3;
    cycles = 5;

    break;
  case OPCODE_STA_IND_X:
    arg1 = processor->regs.accumulator;
    tmp = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("STA ($.2%x,X)\n", tmp);

    tmp += processor->regs.x;
    addr = MemoryGetTwoBytesAt(tmp);
    MemorySetByteAt(addr, arg1);
    REG_PC += 2;
    cycles = 6;

    break;
  case OPCODE_STA_IND_Y:
    arg1 = processor->regs.accumulator;
    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("STA ($.2%x),Y\n", addr);
    addr = MemoryGetTwoBytesAt(addr);
    addr += processor->regs.y;
      
    MemorySetByteAt(addr, arg1);

    REG_PC += 2;
    cycles = 6;

    break;
  case OPCODE_TXS:
    OPPRINTF("TXS\n");
    processor->regs.sp = processor->regs.x;
    REG_PC = REG_PC+1;
    cycles = 2;

    break;
  case OPCODE_TSX:
    OPPRINTF("TSX\n");
    processor->regs.x = processor->regs.sp;
    REG_PC = REG_PC+1;
    cycles = 2;

    break;
  case OPCODE_PHA:
    OPPRINTF("PHA\n");
    StackPushByte(processor->regs.accumulator);
    REG_PC = REG_PC+1;
    cycles = 3;

    break;      
      
  case OPCODE_PLA:
    OPPRINTF("PLA\n");
    processor->regs.accumulator = StackPopByte();
    REG_PC = REG_PC+1;
    cycles = 4;

    break;      

  case OPCODE_PHP:
    OPPRINTF("PHP\n");
    StackPushByte(REG_ST);
    REG_PC = REG_PC+1;
    cycles = 3;

    break;      

  case OPCODE_PLP:
    OPPRINTF("PLP\n");
    REG_ST = StackPopByte();
    REG_PC = REG_PC+1;
    cycles = 4;

    break;      
  case OPCODE_STX_ZERO:
    arg1 = processor->regs.x;
    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("STX $%.2x\n", addr);

    MemorySetByteAt(addr, arg1);
    REG_PC += 2;
    cycles = 3;

    break;
  case OPCODE_STX_ZERO_Y:
    tmp = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("LDX $%.2x,Y\n", tmp);

    arg1 = processor->regs.x;
    addr = tmp  + processor->regs.y; 
    MemorySetByteAt(addr, arg1);
    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_STX_ABS:
    arg1 = processor->regs.x;
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("STX $%.4x\n", addr);

    MemorySetByteAt(addr, arg1);
    REG_PC = REG_PC+3;
    cycles = 4;

    break;
  case OPCODE_STY_ZERO:
    arg1 = processor->regs.y;
    addr = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("STY $%.2x\n", addr);

    MemorySetByteAt(addr, arg1);
    REG_PC += 2;
    cycles = 3;
           
    break;
  case OPCODE_STY_ZERO_X:
    arg1 = processor->regs.y;
    arg2 = MemoryGetByteAt(REG_PC+1);
    OPPRINTF("STY $%.2x,X\n", arg2);

    addr = arg2 + processor->regs.x;
    addr &= 0xFF;
    MemorySetByteAt(addr, arg1);
    REG_PC += 2;
    cycles = 4;

    break;
  case OPCODE_STY_ABS:
    arg1 = processor->regs.y;
    addr = MemoryGetTwoBytesAt(REG_PC+1);
    OPPRINTF("STY $%.4x\n", addr);

    MemorySetByteAt(addr, arg1);
    REG_PC = REG_PC+3;
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
  processor->regs.sp = 0xFF;

  // Start with the reset vector
  REG_PC = MemoryGetTwoBytesAt(RESET_VECTOR_ADDR);
  
  return 0;
}


 

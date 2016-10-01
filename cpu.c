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

/* void* */
/* CpuLoopThd(void *threadid) */
/* { */
/*   long tid = (long)threadid; */
  

/*   // Start with the reset vector */
/*   processor->regs.pc = MemoryGetTwoBytesAt(RESET_VECTOR_ADDR); */
/*   // printf("%s: reset vector %#x", __FUNCTION__, processor->regs.pc); */

  
/* } */
// #define OPPRINTF(...) printf(__VA_ARGS__)
#define OPPRINTF(...) 

int
CpuCycle(){
  //    bool programrunning = true;
    // get the next opcode
    unsigned char opcode = MemoryGetByteAt(processor->regs.pc);
    
    // helper vars for calculations
    ushort tmp = 0;
    char arg1 = 0;
    char arg2 = 0;
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
    
    // printf("PC: %#x\n", processor->regs.pc);
    //    printf("OPCODE:%#x\n", opcode);
    
    switch(opcode){
    case OPCODE_ADC_IMM:
      OPPRINTF("OPCODE_ADC_IMM:\n");

      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(processor->regs.pc+1 );
      result = arg1 + arg2 + FLAG_CARRY(processor->regs.flags);
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 2;
      break;
    case OPCODE_ADC_ZERO:
      OPPRINTF("OPCODE_ADC_ZERO:\n");
      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      arg1 = processor->regs.accumulator;
      tmp = MemoryGetByteAt(processor->regs.pc+1 );
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 + arg2 + FLAG_CARRY(processor->regs.flags);
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 3;
      break;

    case OPCODE_ADC_ZERO_X:
      OPPRINTF("OPCODE_ADC_ZERO_X:\n");
      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      
      tmp = MemoryGetByteAt(processor->regs.pc+1);
      tmp += processor->regs.x;
      tmp &= 0xFF;
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 + arg2 + FLAG_CARRY(processor->regs.flags);
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 4;
      break;
    case OPCODE_ADC_ABS:
      OPPRINTF("OPCODE_ADC_ABS:\n");
      assert(!FLAG_DECIMAL(processor->regs.flags)); 
      
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      result = arg1 + arg2 + FLAG_CARRY(processor->regs.flags);
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      SETCARRY(result);
      processor->regs.pc += 3;
      cycles = 4;
      
      break;

    case OPCODE_ADC_ABS_X:
      OPPRINTF("OPCODE_ADC_ABS_X:\n");

      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      
      arg1 = processor->regs.accumulator;
	    
      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg2 += processor->regs.x;
      result = arg1 + arg2 + FLAG_CARRY(processor->regs.flags);
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      SETCARRY(result);
      processor->regs.pc += 3;

      cycles = 4;
      break;
      
    case OPCODE_ADC_ABS_Y:
      OPPRINTF("OPCODE_ADC_ABS_Y:\n");

      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      arg1 = processor->regs.accumulator;
	    
      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg2 += processor->regs.y;
      result = arg1 + arg2 + FLAG_CARRY(processor->regs.flags);
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      SETCARRY(result);
      processor->regs.pc += 3;

      cycles = 4;
      break;
    case OPCODE_ADC_IND_X:
      OPPRINTF("OPCODE_ADC_IND_X:\n");

      assert(!FLAG_DECIMAL(processor->regs.flags)); 
      
      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetByteAt(processor->regs.pc+1);
      arg2 += processor->regs.x;
      arg2 = MemoryGetByteAt(MemoryGetTwoBytesAt(arg2));
      result = arg1 + arg2 + FLAG_CARRY(processor->regs.flags);
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 6;
      break;

      
    case OPCODE_ADC_IND_Y:
      OPPRINTF("OPCODE_ADC_IND_Y:\n");
      assert(!FLAG_DECIMAL(processor->regs.flags)); 
      
      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetTwoBytesAt(MemoryGetByteAt(processor->regs.pc+1));
      arg2 += processor->regs.y;
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 + arg2 + FLAG_CARRY(processor->regs.flags);
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 5;
      break;
    case OPCODE_AND_IMM:
      OPPRINTF("OPCODE_AND_IMM:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(processor->regs.pc+1 );
      result = arg1 & arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;

      cycles = 2;
      break;
    case OPCODE_AND_ZERO:
      OPPRINTF("OPCODE_AND_ZERO:\n");
      arg1 = processor->regs.accumulator;
      tmp = MemoryGetByteAt(processor->regs.pc+1 );
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 & arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;

      cycles = 3;
      break;

    case OPCODE_AND_ZERO_X:
      OPPRINTF("OPCODE_AND_ZERO_X:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1);
      tmp += processor->regs.x;
      tmp &= 0xFF;
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 & arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 4;
      break;

    case OPCODE_AND_ABS:
      OPPRINTF("OPCODE_AND_ABS:\n");

      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      result = arg1 & arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;
      break;
      
    case OPCODE_AND_ABS_X:
      OPPRINTF("OPCODE_AND_ABS_X:\n");
      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg2 += processor->regs.x;
      result = arg1 & arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;
      break;
    case OPCODE_AND_ABS_Y:
      OPPRINTF("OPCODE_AND_ABS_Y:\n");
      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg2 += processor->regs.y;
      result = arg1 & arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;
      break;

    case OPCODE_AND_IND_X:
      OPPRINTF("OPCODE_AND_IND_X:\n");
      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetByteAt(processor->regs.pc+1);
      arg2 += processor->regs.x;
      arg2 = MemoryGetByteAt(MemoryGetTwoBytesAt(arg2));
      result = arg1 & arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 6;
      break;

      
    case OPCODE_AND_IND_Y:
      OPPRINTF("OPCODE_AND_IND_Y:\n");
      
      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetTwoBytesAt(MemoryGetByteAt(processor->regs.pc+1));
      arg2 += processor->regs.y;
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 & arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 5;
      break;

    case OPCODE_ASL_A:
      OPPRINTF("OPCODE_ASL_A:\n");
      result = processor->regs.accumulator;
      processor->regs.accumulator = (char)result << 1;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      SETCARRY(result);
      processor->regs.pc += 1;
      cycles = 2;
      break;

    case OPCODE_ASL_ZERO:
      OPPRINTF("OPCODE_ASL_ZERO:\n");
      addr = MemoryGetByteAt(processor->regs.pc+1 );
      arg1 = MemoryGetByteAt(addr);
      result = arg1 << 1;
      MemorySetByteAt(addr, (char)result);
      SETSIGN((char)result);
      SETZERO((char)result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 5;
      break;

    case OPCODE_ASL_ZERO_X:
      OPPRINTF("OPCODE_ASL_ZERO_X:\n");
      addr = MemoryGetByteAt(processor->regs.pc+1);
      addr += processor->regs.x;
      addr &= 0xFF;
      arg1 = MemoryGetByteAt(addr);
      result = arg1 << 1;
      MemorySetByteAt(addr, (char)result);
      SETSIGN((char)result);
      SETZERO((char)result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 6;
      break;

    case OPCODE_ASL_ABS:
      OPPRINTF("OPCODE_ASL_ABS:\n");
      
      // get addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);

      // get byte
      arg1 = MemoryGetByteAt(addr);

      // operate on byte
      result = arg1 << 1;

      // write back to memory
      MemorySetByteAt(addr, (char)result);

      // set signs
      SETSIGN((char)result);
      SETZERO((char)result);
      SETCARRY(result);

      // advance pc
      processor->regs.pc += 3;
      cycles = 6;
      break;
      
    case OPCODE_ASL_ABS_X:
      OPPRINTF("OPCODE_ASL_ABS_X:\n");

      // calculate addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);
      addr += processor->regs.x;
      
      // get byte
      arg1 = MemoryGetByteAt(addr);

      // operate on byte
      result = arg1 << 1;

      // write back to memory
      MemorySetByteAt(addr, (char)result);

      // set signs
      SETSIGN((char)result);
      SETZERO((char)result);
      SETCARRY(result);

      // advance pc
      processor->regs.pc += 3;
      cycles = 7;
      break;
    case OPCODE_BIT_ZERO:
      OPPRINTF("OPCODE_BIT_ZERO:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(MemoryGetByteAt(processor->regs.pc+1));
      result = arg1 & arg2;
      processor->regs.accumulator = (char)result;
      SETZERO(result);

      if(FLAG_OVER(arg2))
	FLAG_OVER_SET(processor->regs.flags);
      else 
	FLAG_OVER_CLEAR(processor->regs.flags);

      if(FLAG_NEG(arg2))
	FLAG_NEG_SET(processor->regs.flags);
      else 
	FLAG_NEG_CLEAR(processor->regs.flags);

      processor->regs.pc += 2;
      cycles = 3;
      break;
      
    case OPCODE_BIT_ABS:
      OPPRINTF("OPCODE_BIT_ABS:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      result = arg1 & arg2;
      processor->regs.accumulator = (char)result;
      SETZERO(result);

      if(FLAG_OVER(arg2))
	FLAG_OVER_SET(processor->regs.flags);
      else 
	FLAG_OVER_CLEAR(processor->regs.flags);

      if(FLAG_NEG(arg2))
	FLAG_NEG_SET(processor->regs.flags);
      else 
	FLAG_NEG_CLEAR(processor->regs.flags);

      processor->regs.pc += 3;
      cycles = 4;
      break;

    case OPCODE_BPL:
      OPPRINTF("OPCODE_BPL:\n");

      cycles = 2;
      // Note address if branch not taken
      tmp = processor->regs.pc + 2;
      
      if(!FLAG_NEG(processor->regs.flags)) {
	processor->regs.pc +=
	  (signed char)(MemoryGetByteAt(processor->regs.pc + 1));
	cycles++;
      	// Does branch cross page boundary?
	if((tmp & 0xFF) != (processor->regs.pc & 0xFF))
	  cycles++;
      }
      processor->regs.pc += 2;					    
      break;
    case OPCODE_BMI:
      OPPRINTF("OPCODE_BMI:\n");
      cycles = 2;

      if(FLAG_NEG(processor->regs.flags)) {
	processor->regs.pc +=
	  (signed char)(MemoryGetByteAt(processor->regs.pc + 1));
	cycles++;
      	// Does branch cross page boundary?
	if((tmp & 0xFF) != (processor->regs.pc & 0xFF))
	  cycles++;
      }
      processor->regs.pc += 2;					    
      break;
    case OPCODE_BVC:
      OPPRINTF("OPCODE_BVC:\n");
      cycles = 2;
      // Note address if branch not taken
      tmp = processor->regs.pc + 2;

      if(!FLAG_OVER(processor->regs.flags)) {
	processor->regs.pc +=
	  (signed char)(MemoryGetByteAt(processor->regs.pc + 1));
	cycles++;
      	// Does branch cross page boundary?
	if((tmp & 0xFF) != (processor->regs.pc & 0xFF))
	  cycles++;
      }
      processor->regs.pc += 2;					    
      break;
    case OPCODE_BVS:
      OPPRINTF("OPCODE_BVS:\n");
      cycles = 2;
      // Note address if branch not taken
      tmp = processor->regs.pc + 2;

      if(FLAG_OVER(processor->regs.flags)) {
	processor->regs.pc +=
	  (signed char)(MemoryGetByteAt(processor->regs.pc + 1));
	cycles++;
      	// Does branch cross page boundary?
	if((tmp & 0xFF) != (processor->regs.pc & 0xFF))
	  cycles++;
      }
      processor->regs.pc += 2;					    
      break;
    case OPCODE_BCC:
      OPPRINTF("OPCODE_BCC:\n");
      cycles = 2;
      // Note address if branch not taken
      tmp = processor->regs.pc + 2;

      if(!FLAG_CARRY(processor->regs.flags)) {
	processor->regs.pc +=
	  (signed char)(MemoryGetByteAt(processor->regs.pc + 1));
	cycles++;
	// Does branch cross page boundary?
	if((tmp & 0xFF) != (processor->regs.pc & 0xFF))
	  cycles++;

      }
      processor->regs.pc += 2;					    
      break;
    case OPCODE_BCS:
      OPPRINTF("OPCODE_BCS:\n");
      cycles = 2;
      // Note address if branch not taken
      tmp = processor->regs.pc + 2;

      if(FLAG_CARRY(processor->regs.flags)) {
	processor->regs.pc +=
	  (signed char)(MemoryGetByteAt(processor->regs.pc + 1));
	// Does branch cross page boundary?
	if((tmp & 0xFF) != (processor->regs.pc & 0xFF))
	  cycles++;
	cycles++;
      }
      processor->regs.pc += 2;					    
      break;
    case OPCODE_BNE:
      OPPRINTF("OPCODE_BNE:\n");
      cycles = 2;
      // Note address if branch not taken
      tmp = processor->regs.pc + 2;
      if(!FLAG_ZERO(processor->regs.flags)) {
	processor->regs.pc +=
	  (signed char)(MemoryGetByteAt(processor->regs.pc + 1));
	cycles++;
	// Does branch cross page boundary?
	if((tmp & 0xFF) != (processor->regs.pc & 0xFF))
	  cycles++;
      }
      processor->regs.pc += 2;					    
      break;
    case OPCODE_BEQ:
      OPPRINTF("OPCODE_BEQ:\n");
      cycles = 2;
      // Note address if branch not taken
      tmp = processor->regs.pc + 2;

      if(FLAG_ZERO(processor->regs.flags)) {
	processor->regs.pc +=
	  (signed char)(MemoryGetByteAt(processor->regs.pc + 1));
	cycles++;
	// Does branch cross page boundary?
	if((tmp & 0xFF) != (processor->regs.pc & 0xFF))
	  cycles++;

      }
	 
      processor->regs.pc += 2;					    
      break;
    case OPCODE_BRK:
      OPPRINTF("OPCODE_BRK:\n");
      OPPRINTF("BREAK INSTRUCTION: Not Implemented!\n");
      assert(true == false);
      //programrunning = false;
      processor->regs.pc = processor->regs.pc+1;
      cycles = 7;

      break;
    case OPCODE_CMP_IMM:
      OPPRINTF("OPCODE_CMP_IMM:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(processor->regs.pc+1 );
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 2;

      break;
    case OPCODE_CMP_ZERO:
      OPPRINTF("OPCODE_CMP_ZERO:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(MemoryGetByteAt(processor->regs.pc+1));
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 3;
	    
      break;
    case OPCODE_CMP_ZERO_X:
      OPPRINTF("OPCODE_CMP_ZERO_X:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1);
      tmp += processor->regs.x;
      tmp &= 0xFF;
      arg2 = MemoryGetByteAt(tmp);
      arg1 = processor->regs.accumulator;
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 4;

      break;
    case OPCODE_CMP_ABS:
      OPPRINTF("OPCODE_CMP_ABS:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 3;
      cycles = 4;

      break;
    case OPCODE_CMP_ABS_X:
      OPPRINTF("OPCODE_CMP_ABS_X:\n");
      arg1 = processor->regs.accumulator;
      tmp = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg2 += processor->regs.x;
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 3;
      cycles = 4;

      break;
    case OPCODE_CMP_ABS_Y:
      OPPRINTF("OPCODE_CMP_ABS_Y:\n");
      arg1 = processor->regs.accumulator;
      tmp = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg2 += processor->regs.y;
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 3;
      cycles = 4;

      break;

    case OPCODE_CMP_IND_X:
      OPPRINTF("OPCODE_CMP_IND_X:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(processor->regs.pc+1);
      arg2 += processor->regs.x;
      arg2 = MemoryGetByteAt(MemoryGetTwoBytesAt(arg2));

      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 6;

      break;
     
    case OPCODE_CMP_IND_Y:
      OPPRINTF("OPCODE_CMP_IND_Y:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetTwoBytesAt(MemoryGetByteAt(processor->regs.pc+1));
      arg2 += processor->regs.y;
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 5;

      break;

    case OPCODE_CPX_IMM:
      OPPRINTF("OPCODE_CPX_IMM:\n");
      arg1 = processor->regs.x;
      arg2 = MemoryGetByteAt(processor->regs.pc+1 );
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 2;

      break;
    case OPCODE_CPX_ZERO:
      OPPRINTF("OPCODE_CPX_ZERO:\n");
      arg1 = processor->regs.x;
      arg2 = MemoryGetByteAt(MemoryGetByteAt(processor->regs.pc+1));
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 3;

      break;
      
    case OPCODE_CPX_ABS:
      OPPRINTF("OPCODE_CPX_ABS:\n");
      arg1 = processor->regs.x;
      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 3;
      cycles = 4;

      break;

    case OPCODE_CPY_IMM:
      OPPRINTF("OPCODE_CPY_IMM:\n");
      arg1 = processor->regs.y;
      arg2 = MemoryGetByteAt(processor->regs.pc+1 );
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 2;

      break;

    case OPCODE_CPY_ZERO:
      OPPRINTF("OPCODE_CPY_ZERO:\n");
      arg1 = processor->regs.y;
      arg2 = MemoryGetByteAt(MemoryGetByteAt(processor->regs.pc+1));
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 2;
      cycles = 2;

      break;
    case OPCODE_CPY_ABS:
      OPPRINTF("OPCODE_CPY_ABS:\n");
      arg1 = processor->regs.y;
      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      result = arg1 - arg2;
      result==0?FLAG_NEG_CLEAR(processor->regs.flags):
	SETSIGN(arg1);
      SETZERO(result);
      SETOVER(arg1, arg2, result);
      SETCARRY(result);
      processor->regs.pc += 3;
      cycles = 2;

      break;
      
    case OPCODE_DEC_ZERO:
      OPPRINTF("OPCODE_DEC_ZERO:\n");
      arg1 = MemoryGetByteAt(processor->regs.pc+1);
      arg2 = MemoryGetByteAt(arg1);
      arg2--;
      MemorySetByteAt(arg1, arg2);
      SETSIGN(arg2);
      SETZERO(arg2);
      processor->regs.pc += 2;
      cycles = 5;

      break;
     
    case OPCODE_DEC_ZERO_X:
      OPPRINTF("OPCODE_DEC_ZERO_X:\n");
      arg1 = MemoryGetByteAt(processor->regs.pc+1);
      arg1 += processor->regs.x;
      arg2 = MemoryGetByteAt(arg1);
      arg2--;
      MemorySetByteAt(arg1, arg2);
      SETSIGN(arg2);
      SETZERO(arg2);
      processor->regs.pc += 2;
      cycles = 6;

      break;

    case OPCODE_DEC_ABS:
      OPPRINTF("OPCODE_DEC_ABS:\n");

      arg1 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg2 = MemoryGetByteAt(arg1);
      arg2--;
      MemorySetByteAt(arg1, arg2);
      SETSIGN(arg2);
      SETZERO(arg2);
      processor->regs.pc += 3;
      cycles = 6;

      break;

    case OPCODE_DEC_ABS_X:
      OPPRINTF("OPCODE_DEC_ABS_X:\n");
      arg1 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg1 += processor->regs.x;
      arg2 = MemoryGetByteAt(arg1);
      arg2--;
      MemorySetByteAt(arg1, arg2);
      SETSIGN(arg2);
      SETZERO(arg2);
      processor->regs.pc += 3;
      cycles = 7;

      break;

    case OPCODE_EOR_IMM:
      OPPRINTF("OPCODE_EOR_IMM:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(processor->regs.pc+1 );
      result = arg1 ^ arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 2;

      break;

    case OPCODE_EOR_ZERO:
      OPPRINTF("OPCODE_EOR_ZERO:\n");
      arg1 = processor->regs.accumulator;
      tmp = MemoryGetByteAt(processor->regs.pc+1 );
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 ^ arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 3;

      break;

    case OPCODE_EOR_ZERO_X:
      OPPRINTF("OPCODE_EOR_ZERO_X:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1);
      tmp += processor->regs.x;
      tmp &= 0xFF;
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 ^ arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 4;

      break;
      
    case OPCODE_EOR_ABS:
      OPPRINTF("OPCODE_EOR_ABS:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      result = arg1 ^ arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;

      break;
      
    case OPCODE_EOR_ABS_X:
      OPPRINTF("OPCODE_EOR_ABS_X:\n");
      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg2 += processor->regs.x;
      result = arg1 ^ arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;

      break;
      
    case OPCODE_EOR_ABS_Y:
      OPPRINTF("OPCODE_EOR_ABS_Y:\n");
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg2 += processor->regs.y;
      result = arg1 ^ arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;

      break;

    case OPCODE_EOR_IND_X:
      OPPRINTF("OPCODE_EOR_IND_X:\n");

      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetByteAt(processor->regs.pc+1);
      arg2 += processor->regs.x;
      arg2 = MemoryGetByteAt(MemoryGetTwoBytesAt(arg2));
      result = arg1 ^ arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 6;
            
      break;
    case OPCODE_EOR_IND_Y:
      OPPRINTF("OPCODE_EOR_IND_Y:\n");
      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetTwoBytesAt(MemoryGetByteAt(processor->regs.pc+1));
      arg2 += processor->regs.y;
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 ^ arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 5;

      break;

    case OPCODE_CLC:
      OPPRINTF("OPCODE_CLC:\n");
      FLAG_CARRY_CLEAR(processor->regs.flags);
      processor->regs.pc++;
      cycles = 2;

      break;
    case OPCODE_SEC:
      OPPRINTF("OPCODE_SEC:\n");
      FLAG_CARRY_SET(processor->regs.flags);
      processor->regs.pc++;
      cycles = 2;

      break;
    case OPCODE_CLI:
      OPPRINTF("OPCODE_CLI:\n");
      FLAG_INTERRUPT_CLEAR(processor->regs.flags);
      processor->regs.pc++;
      cycles = 2;

      break;
    case OPCODE_SEI:
      OPPRINTF("OPCODE_SEI:\n");
      FLAG_INTERRUPT_SET(processor->regs.flags);
      processor->regs.pc++;
      cycles = 2;

      break;
    case OPCODE_CLV:
      OPPRINTF("OPCODE_CLV:\n");
      FLAG_OVER_CLEAR(processor->regs.flags);
      processor->regs.pc++;
      cycles = 2;

      break;
    case OPCODE_CLD:
      OPPRINTF("OPCODE_CLD:\n");
      FLAG_DECIMAL_CLEAR(processor->regs.flags);
      processor->regs.pc++;
      cycles = 2;

      break;
    case OPCODE_SED:
      OPPRINTF("OPCODE_SED:\n");
      FLAG_DECIMAL_SET(processor->regs.flags);
      processor->regs.pc++;
      cycles = 2;

      break;
    case OPCODE_INC_ZERO:
      OPPRINTF("OPCODE_INC_ZERO:\n");
      arg1 = MemoryGetByteAt(processor->regs.pc+1);
      arg2 = MemoryGetByteAt(arg1);
      arg2++;
      MemorySetByteAt(arg1, arg2);
      SETSIGN(arg2);
      SETZERO(arg2);
      processor->regs.pc += 2;
      cycles = 5;

      break;
    case OPCODE_INC_ZERO_X:
      OPPRINTF("OPCODE_INC_ZERO_X:\n");
      arg1 = MemoryGetByteAt(processor->regs.pc+1);
      arg1 += processor->regs.x;
      arg2 = MemoryGetByteAt(arg1);
      arg2++;
      MemorySetByteAt(arg1, arg2);
      SETSIGN(arg2);
      SETZERO(arg2);
      processor->regs.pc += 2;
      cycles = 6;

      break;

    case OPCODE_INC_ABS:
      OPPRINTF("OPCODE_INC_ABS:\n");
      arg1 = MemoryGetTwoBytesAt(processor->regs.pc+1);
      arg2 = MemoryGetByteAt(arg1);
      arg2++;
      MemorySetByteAt(arg1, arg2);
      SETSIGN(arg2);
      SETZERO(arg2);
      processor->regs.pc += 3;
      cycles = 6;

      break;

    case OPCODE_INC_ABS_X:
      OPPRINTF("OPCODE_INC_ABS_X:\n");

      // Get base address
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);

      // Offset addr by x
      addr += processor->regs.x;

      // Get byte
      arg1 = MemoryGetByteAt(addr);

      // increment byte
      arg1++;

      // store back in memory
      MemorySetByteAt(addr, arg1);

      // flags
      SETSIGN(arg2);
      SETZERO(arg2);
      processor->regs.pc += 3;
      cycles = 7;

      break;

    case OPCODE_JMP_ABS:
      OPPRINTF("OPCODE_JMP_ABS:\n");
      processor->regs.pc = MemoryGetTwoBytesAt(processor->regs.pc+1);
      cycles = 3;

      break;
    case OPCODE_JMP_IND:
      OPPRINTF("OPCODE_JMP_IND:\n");
      processor->regs.pc =
	MemoryGetByteAt(MemoryGetTwoBytesAt(processor->regs.pc+1));
      cycles = 5;

      break;
    case OPCODE_JSR_ABS:
      OPPRINTF("OPCODE_JSR_ABS:\n");
      // push address -1(?) of the next operation on to the stack
      StackPushShort(processor->regs.pc + 2);
      processor->regs.pc =
	MemoryGetTwoBytesAt(processor->regs.pc+1);
      cycles = 6;
	    
      break;
    case OPCODE_LDA_IMM:
      OPPRINTF("OPCODE_LDA_IMM:\n");
      processor->regs.accumulator = MemoryGetByteAt(processor->regs.pc+1 );
       processor->regs.pc += 2;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      cycles = 2;

      break;
    case OPCODE_LDA_ZERO:
      OPPRINTF("OPCODE_LDA_ZERO:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1 );
      processor->regs.accumulator = MemoryGetByteAt(tmp);
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
       processor->regs.pc += 2;
       cycles = 3;

      break;
    case OPCODE_LDA_ZERO_X:
      OPPRINTF("OPCODE_LDA_ZERO_X:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1);
      tmp += processor->regs.x;
      tmp &= 0xFF;
      processor->regs.accumulator = MemoryGetByteAt(tmp);
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 4;

      break;
    case OPCODE_LDA_ABS:
      OPPRINTF("OPCODE_LDA_ABS:\n");
      tmp = MemoryGetTwoBytesAt(processor->regs.pc+1);
      processor->regs.accumulator = MemoryGetByteAt(tmp );
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 4;

      break;
    case OPCODE_LDA_ABS_X:
      OPPRINTF("OPCODE_LDA_ABS_X:\n");
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);
      addr += processor->regs.x;
      
      processor->regs.accumulator = MemoryGetByteAt( addr );
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 4;
      if (((addr + processor->regs.x) & 0xFF) > 0x100)
	cycles++;
      break;
    case OPCODE_LDA_ABS_Y:
      OPPRINTF("OPCODE_LDA_ABS_Y:\n");
      tmp = MemoryGetTwoBytesAt(processor->regs.pc+1);
      tmp += processor->regs.y;
      processor->regs.accumulator = MemoryGetByteAt(tmp );
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 4;

      break;
    case OPCODE_LDA_IND_X:
      OPPRINTF("OPCODE_LDA_IND_X:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1 );
      tmp += processor->regs.x;
      processor->regs.accumulator = MemoryGetByteAt(MemoryGetTwoBytesAt(tmp) );
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 6;

      break;
    case OPCODE_LDA_IND_Y:
      OPPRINTF("OPCODE_LDA_IND_Y:\n");
      tmp = MemoryGetTwoBytesAt(MemoryGetByteAt(processor->regs.pc+1));
      tmp += processor->regs.y;
      processor->regs.accumulator = MemoryGetByteAt(tmp);
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 5;

      break;
    case OPCODE_LDX_IMM:
      OPPRINTF("OPCODE_LDX_IMM:\n");
      processor->regs.x = MemoryGetByteAt(processor->regs.pc+1);
      OPPRINTF("x: %#x\n", processor->regs.x);
      SETSIGN(processor->regs.x);
      SETZERO(processor->regs.x);
      processor->regs.pc += 2;
      cycles = 2;

      break;
    case OPCODE_LDX_ZERO:
      OPPRINTF("OPCODE_LDX_ZERO:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1 );
      processor->regs.x = MemoryGetByteAt(tmp);
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 3;

      break;

    case OPCODE_LDX_ZERO_Y:
      OPPRINTF("OPCODE_LDX_ZERO_Y:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1);
      tmp += processor->regs.y;
      processor->regs.x = MemoryGetByteAt(tmp);
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 4;

      break;

    case OPCODE_LDX_ABS:
      OPPRINTF("OPCODE_LDX_ABS:\n");
      tmp = MemoryGetTwoBytesAt(processor->regs.pc+1);
      processor->regs.x = MemoryGetByteAt(tmp );
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 4;

      break;
      
    case OPCODE_LDX_ABS_Y:
      OPPRINTF("OPCODE_LDX_ABS_Y:\n");
      tmp = MemoryGetTwoBytesAt(processor->regs.pc+1);
      tmp += processor->regs.y;
      processor->regs.x = MemoryGetByteAt(tmp );
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 4;

      break;
    case OPCODE_LDY_IMM:
      OPPRINTF("OPCODE_LDY_IMM:\n");
      processor->regs.y = MemoryGetByteAt(processor->regs.pc+1);
      SETSIGN(processor->regs.y);
      SETZERO(processor->regs.y);
      processor->regs.pc += 2;
      cycles = 2;

      break;
    case OPCODE_LDY_ZERO:
      OPPRINTF("OPCODE_LDY_ZERO:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1 );
      processor->regs.y = MemoryGetByteAt(tmp);
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 3;

      break;

    case OPCODE_LDY_ZERO_X:
      OPPRINTF("OPCODE_LDY_ZERO_X:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1);
      tmp += processor->regs.x;
      tmp &= 0xFF;
      processor->regs.y = MemoryGetByteAt(tmp);
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 4;

      break;

    case OPCODE_LDY_ABS:
      OPPRINTF("OPCODE_LDY_ABS:\n");
      tmp = MemoryGetTwoBytesAt(processor->regs.pc+1);
      processor->regs.y = MemoryGetByteAt(tmp );
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 4;

      break;
    case OPCODE_LDY_ABS_X:
      OPPRINTF("OPCODE_LDY_ABS_X:\n");
      tmp = MemoryGetTwoBytesAt(processor->regs.pc+1);
      tmp += processor->regs.x;
      processor->regs.y = MemoryGetByteAt(tmp );
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 4;

      break;
    case OPCODE_LSR_A:
      OPPRINTF("OPCODE_LSR_A:\n");
      arg1 = processor->regs.accumulator;
      processor->regs.accumulator = 0x7F & (arg1 >> 1);
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      if(arg1 & 0xFE)
	FLAG_CARRY_SET(processor->regs.flags);
      else
	FLAG_CARRY_CLEAR(processor->regs.flags);
      processor->regs.pc += 1;
      cycles = 2;

      break;
   
    case OPCODE_LSR_ZERO:
      OPPRINTF("OPCODE_LSR_ZERO:\n");
      addr = MemoryGetByteAt(processor->regs.pc+1 );
      arg1 = MemoryGetByteAt(addr);
      result = 0x7F & (arg1 >> 1);
      MemorySetByteAt(addr,  (char)result);
      SETSIGN((char)result);
      SETZERO((char)result);
      if(arg1 & 0xFE)
	FLAG_CARRY_SET(processor->regs.flags);
      else
	FLAG_CARRY_CLEAR(processor->regs.flags);
      processor->regs.pc += 2;
      cycles = 5;

      break;
      
    case OPCODE_LSR_ZERO_X:
      OPPRINTF("OPCODE_LSR_ZERO_X:\n");
      // calculate addr
      addr = MemoryGetByteAt(processor->regs.pc+1);
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
	FLAG_CARRY_SET(processor->regs.flags);
      else
	FLAG_CARRY_CLEAR(processor->regs.flags);

      // advance pc past used bytes
      processor->regs.pc += 2;
      cycles = 6;

      break;

    case OPCODE_LSR_ABS:
      OPPRINTF("OPCODE_LSR_ABS:\n");

      // get addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);

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
	FLAG_CARRY_SET(processor->regs.flags);
      else
	FLAG_CARRY_CLEAR(processor->regs.flags);

      // advance pc
      processor->regs.pc += 3;
      cycles = 6;

      break;
      
    case OPCODE_LSR_ABS_X:
      OPPRINTF("OPCODE_LSR_ABS_X:\n");

      // calculate addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);
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
	FLAG_CARRY_SET(processor->regs.flags);
      else
	FLAG_CARRY_CLEAR(processor->regs.flags);

      // advance pc
      processor->regs.pc += 3;
      cycles = 7;

      break;

    case OPCODE_NOP:
      OPPRINTF("OPCODE_NOP:\n");
      // No op!
      
      // advance pc
      processor->regs.pc += 1;
      cycles = 2;

      break;
    case OPCODE_ORA_IMM:
      OPPRINTF("OPCODE_ORA_IMM:\n");
      processor->regs.accumulator |= MemoryGetByteAt(processor->regs.pc+1);
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 2;
            
      break;
    case OPCODE_ORA_ZERO:
      OPPRINTF("OPCODE_ORA_ZERO:\n");
      addr = MemoryGetByteAt(processor->regs.pc+1 );
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(addr);
      result = arg1 | arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 3;

      break;
    case OPCODE_ORA_ZERO_X:
      OPPRINTF("OPCODE_ORA_ZERO_X:\n");
      tmp = MemoryGetByteAt(processor->regs.pc+1);
      tmp += processor->regs.x;
      tmp &= 0xFF;
      arg1 = processor->regs.accumulator;
      arg2 = MemoryGetByteAt(tmp);
      result = arg1 | arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 4;

      break;
    case OPCODE_ORA_ABS:
      OPPRINTF("OPCODE_ORA_ABS:\n");
      arg1 = processor->regs.accumulator;

      // get address
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);

      // get byte
      arg2 = MemoryGetByteAt(addr);

      result = arg1 | arg2;
      
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;

      break;
      
    case OPCODE_ORA_ABS_X:
      OPPRINTF("OPCODE_ORA_ABS_X:\n");
      arg1 = processor->regs.accumulator;

      // Get base address
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);

      // Address + x
      addr += processor->regs.x;

      // get byte
      arg2 = MemoryGetByteAt(addr);

      result = arg1 | arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;

      break;

    case OPCODE_ORA_ABS_Y:
      OPPRINTF("OPCODE_ORA_ABS_Y:\n");

      arg1 = processor->regs.accumulator;

      // Get base address
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);

      // Address + y
      addr += processor->regs.y;

      // get byte
      arg2 = MemoryGetByteAt(addr);

      result = arg1 | arg2;
      
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;

      break;

    case OPCODE_ORA_IND_X:
      OPPRINTF("OPCODE_ORA_IND_X:\n");

      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetByteAt(processor->regs.pc+1);
      arg2 += processor->regs.x;
      arg2 = MemoryGetByteAt(MemoryGetTwoBytesAt(arg2));
      result = arg1 | arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 6;

      break;

    case OPCODE_ORA_IND_Y:
      OPPRINTF("OPCODE_ORA_IND_Y:\n");
      arg1 = processor->regs.accumulator;

      arg2 = MemoryGetTwoBytesAt(MemoryGetByteAt(processor->regs.pc+1));
      arg2 += processor->regs.y;
      arg2 = MemoryGetByteAt(arg2);
      result = arg1 | arg2;
      processor->regs.accumulator = result;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 5;

      break;

    case OPCODE_TAX:
      OPPRINTF("OPCODE_TAX:\n");
      processor->regs.x = processor->regs.accumulator;
      processor->regs.pc = processor->regs.pc+1;
      SETSIGN(processor->regs.x);
      SETZERO(processor->regs.x);
      cycles = 2;
	    
      break;
    case OPCODE_TXA:
      OPPRINTF("OPCODE_TXA:\n");
      processor->regs.accumulator = processor->regs.x;
      processor->regs.pc = processor->regs.pc+1;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      cycles = 2;

      break;
    case OPCODE_DEX:
      OPPRINTF("OPCODE_DEX:\n");
      processor->regs.x--;
      processor->regs.pc = processor->regs.pc+1;
      SETSIGN(processor->regs.x);
      SETZERO(processor->regs.x);
      cycles = 2;

      break;
    case OPCODE_INX:
      OPPRINTF("OPCODE_INX:\n");
      processor->regs.x++;
      OPPRINTF("x: %#x\n", processor->regs.x);
      processor->regs.pc = processor->regs.pc+1;
      SETSIGN(processor->regs.x);
      SETZERO(processor->regs.x);
      cycles = 2;

      break;
    case OPCODE_TAY:
      OPPRINTF("OPCODE_TAY:\n");
      processor->regs.y = processor->regs.accumulator;
      processor->regs.pc = processor->regs.pc+1;
      SETSIGN(processor->regs.y);
      SETZERO(processor->regs.y);
      cycles = 2;

      break;
    case OPCODE_TYA:
      OPPRINTF("OPCODE_TYA:\n");
      processor->regs.accumulator = processor->regs.y;
      processor->regs.pc = processor->regs.pc+1;
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      cycles = 2;

      break;
    case OPCODE_DEY:
      OPPRINTF("OPCODE_DEY:\n");
      processor->regs.y--;
      processor->regs.pc = processor->regs.pc+1;
      SETSIGN(processor->regs.y);
      SETZERO(processor->regs.y);
      cycles = 2;

      break;
    case OPCODE_INY:
      OPPRINTF("OPCODE_INY:\n");
      processor->regs.y++;
      processor->regs.pc = processor->regs.pc+1;
      SETSIGN(processor->regs.y);
      SETZERO(processor->regs.y);
      cycles = 2;

      break;
    case OPCODE_ROL_A:
      OPPRINTF("OPCODE_ROL_A:\n");
      // get byte from memory
      result = processor->regs.accumulator;
      // Calculate new value
      result = result << 1;
      // Set lowest bit according to carry flag
      if(FLAG_CARRY(processor->regs.flags))
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
      processor->regs.pc += 1;
      cycles = 2;

      break;
      
    case OPCODE_ROL_ZERO:
      OPPRINTF("OPCODE_ROL_ZERO:\n");
      // get byte from memory
      addr = MemoryGetByteAt(processor->regs.pc+1 );
      arg1 = MemoryGetByteAt(addr);

      // Calculate new value
      result = result << 1;
      // Set lowest bit according to carry flag
      if(FLAG_CARRY(processor->regs.flags))
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
      processor->regs.pc += 2;
      cycles = 5;

      break;
    case OPCODE_ROL_ZERO_X:
      OPPRINTF("OPCODE_ROL_ZERO_X:\n");
      // calculate addr
      addr = MemoryGetByteAt(processor->regs.pc+1);
      addr += processor->regs.x;
      addr &= 0xFF;
      // get byte from memory
      arg1 = MemoryGetByteAt(addr);

      // Calculate new value
      result = result << 1;
      // Set lowest bit according to carry flag
      if(FLAG_CARRY(processor->regs.flags))
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
      processor->regs.pc += 2;
      cycles = 6;

      break;

    case OPCODE_ROL_ABS:
      OPPRINTF("OPCODE_ROL_ABS:\n");
            
      // get addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);

      // get byte
      arg1 = MemoryGetByteAt(addr);

      // operate on byte
      result = arg1 << 1;

      // Set lowest bit according to carry flag
      if(FLAG_CARRY(processor->regs.flags))
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
      processor->regs.pc += 3;
      cycles = 6;

      break;

    case OPCODE_ROL_ABS_X:
      OPPRINTF("OPCODE_ROL_ABS_X:\n");

      // calculate addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);
      addr += processor->regs.x;
      
      // get byte
      arg1 = MemoryGetByteAt(addr);

      // operate on byte
      result = arg1 << 1;

      // Set lowest bit according to carry flag
      if(FLAG_CARRY(processor->regs.flags))
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
      processor->regs.pc += 3;
      cycles = 7;

      break;

    case OPCODE_ROR_A:
      OPPRINTF("OPCODE_ROR_A:\n");

      // get byte from memory
      arg1 = processor->regs.accumulator;

      // Calculate new value
      result = arg1 >> 1;

      // Set lowest bit according to carry flag
      if(FLAG_CARRY(processor->regs.flags))
	result |= 0x80;
      else
	result &= 0x7F;

      // write back value
      processor->regs.accumulator = (char)result;

      // set flags
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      if(arg1 & 0xFE)
	FLAG_CARRY_SET(processor->regs.flags);
      else
	FLAG_CARRY_CLEAR(processor->regs.flags);

      // increment pc
      processor->regs.pc += 1;
      cycles = 2;

      break;

    case OPCODE_ROR_ZERO:
      OPPRINTF("OPCODE_ROR_ZERO:\n");
        // get byte from memory
      addr = MemoryGetByteAt(processor->regs.pc+1 );
      arg1 = MemoryGetByteAt(addr);

      // Calculate new value
      result = arg1 >> 1;

      // Set lowest bit according to carry flag
      if(FLAG_CARRY(processor->regs.flags))
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
      processor->regs.pc += 2;
      cycles = 5;

      break;
    case OPCODE_ROR_ZERO_X:
      OPPRINTF("OPCODE_ROR_ZERO_X:\n");
            // calculate addr
      addr = MemoryGetByteAt(processor->regs.pc+1);
      addr += processor->regs.x;
      addr &= 0xFF;
      // get byte from memory
      arg1 = MemoryGetByteAt(addr);

      // Calculate new value
      result = arg1 >> 1;

      // Set lowest bit according to carry flag
      if(FLAG_CARRY(processor->regs.flags))
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
      processor->regs.pc += 2;
      cycles = 6;

      break;

    case OPCODE_ROR_ABS:
      OPPRINTF("OPCODE_ROR_ABS:\n");
                  
      // get addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);

      // get byte
      arg1 = MemoryGetByteAt(addr);

      // operate on byte
      result = arg1 >> 1;

      // Set lowest bit according to carry flag
      if(FLAG_CARRY(processor->regs.flags))
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
      processor->regs.pc += 3;
      cycles = 6;
      break;

    case OPCODE_ROR_ABS_X:
      OPPRINTF("OPCODE_ROR_ABS_X:\n");

      // calculate addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);
      addr += processor->regs.x;
      
      // get byte
      arg1 = MemoryGetByteAt(addr);

      // operate on byte
      result = arg1 >> 1;

      // Set lowest bit according to carry flag
      if(FLAG_CARRY(processor->regs.flags))
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
      processor->regs.pc += 3;
      cycles = 7;
      break;

    case OPCODE_RTI:
      OPPRINTF("OPCODE_RTI:\n");
      /* RTI retrieves the Processor Status Word (flags) and the Program Counter from the stack in that order (interrupts push the PC first and then the PSW).  */
      processor->regs.flags = StackPopByte();
      processor->regs.pc = StackPopShort();
      cycles = 6;

      break;
    case OPCODE_RTS:
      OPPRINTF("OPCODE_RTS:\n");
      processor->regs.pc = StackPopShort() + 1;
      cycles = 6;

      break;
    case OPCODE_SBC_IMM:
      OPPRINTF("OPCODE_SBC_IMM:\n");

      assert(!FLAG_DECIMAL(processor->regs.flags)); 
      
      // Get minuend 
      arg1 = processor->regs.accumulator;

      // Get subtrahend
      arg2 = MemoryGetByteAt(processor->regs.pc+1);

      // Get its twos compliment
      // Negate the bits
      arg2 = ~arg2;

      // Add one unless (not) carry indicates that borrow occurred
      // thus the difference will be one less.
      if (FLAG_CARRY(processor->regs.flags)){
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
	FLAG_CARRY_CLEAR(processor->regs.flags);
      else
	FLAG_CARRY_SET(processor->regs.flags);

      // Magic makes overflow logic work as if it's an add.
      SETOVER(arg1, arg2, result);
           
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 2;
      break;
    case OPCODE_SBC_ZERO:
      OPPRINTF("OPCODE_SBC_ZERO:\n");
      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      arg1 = processor->regs.accumulator;

      addr = MemoryGetByteAt(processor->regs.pc+1 );
      arg2 = MemoryGetByteAt(addr);

      // Get its twos compliment
      // Negate the bits
      arg2 = ~arg2;

      // Add one unless (not) carry indicates that borrow occurred
      // thus the difference will be one less.
      if (FLAG_CARRY(processor->regs.flags)){
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
	FLAG_CARRY_CLEAR(processor->regs.flags);
      else
	FLAG_CARRY_SET(processor->regs.flags);

      // Magic makes overflow logic work as if it's an add.
      SETOVER(arg1, arg2, result);
      
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 3;
      break;

    case OPCODE_SBC_ZERO_X:
      OPPRINTF("OPCODE_SBC_ZERO_X:\n");
      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      arg1 = processor->regs.accumulator;

      addr = MemoryGetByteAt(processor->regs.pc+1);
      addr += processor->regs.x;
      addr &= 0xFF;
      // get byte from memory
      arg2 = MemoryGetByteAt(addr);

      // Get its twos compliment
      // Negate the bits
      arg2 = ~arg2;

      // Add one unless (not) carry indicates that borrow occurred
      // thus the difference will be one less.
      if (FLAG_CARRY(processor->regs.flags)){
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
	FLAG_CARRY_CLEAR(processor->regs.flags);
      else
	FLAG_CARRY_SET(processor->regs.flags);

      // Magic makes overflow logic work as if it's an add.
      SETOVER(arg1, arg2, result);
           
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 4;
	    
      break;
   
    case OPCODE_SBC_ABS:
      OPPRINTF("OPCODE_SBC_ABS:\n");
      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      arg1 = processor->regs.accumulator;

      // get addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);

      // get byte
      arg2 = MemoryGetByteAt(addr);

      // Get its twos compliment
      // Negate the bits
      arg2 = ~arg2;

      // Add one unless (not) carry indicates that borrow occurred
      // thus the difference will be one less.
      if (FLAG_CARRY(processor->regs.flags)){
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
	FLAG_CARRY_CLEAR(processor->regs.flags);
      else
	FLAG_CARRY_SET(processor->regs.flags);

      // Magic makes overflow logic work as if it's an add.
      SETOVER(arg1, arg2, result);

      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;

      break;

    case OPCODE_SBC_ABS_X:
      OPPRINTF("OPCODE_SBC_ABS_X:\n");
      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      arg1 = processor->regs.accumulator;
	    
      // calculate addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);
      addr += processor->regs.x;
      
      // get byte
      arg2 = MemoryGetByteAt(addr);

      // Get its twos compliment
      // Negate the bits
      arg2 = ~arg2;

      // Add one unless (not) carry indicates that borrow occurred
      // thus the difference will be one less.
      if (FLAG_CARRY(processor->regs.flags)){
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
	FLAG_CARRY_CLEAR(processor->regs.flags);
      else
	FLAG_CARRY_SET(processor->regs.flags);

      // Magic makes overflow logic work as if it's an add.
      SETOVER(arg1, arg2, result);
           
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;
      break;

    case OPCODE_SBC_ABS_Y:
      OPPRINTF("OPCODE_SBC_ABS_Y:\n");
      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      arg1 = processor->regs.accumulator;
	    
      // calculate addr
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);
      addr += processor->regs.y;

      // get byte
      arg2 = MemoryGetByteAt(addr);

      // Get its twos compliment
      // Negate the bits
      arg2 = ~arg2;

      // Add one unless (not) carry indicates that borrow occurred
      // thus the difference will be one less.
      if (FLAG_CARRY(processor->regs.flags)){
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
	FLAG_CARRY_CLEAR(processor->regs.flags);
      else
	FLAG_CARRY_SET(processor->regs.flags);

      // Magic makes overflow logic work as if it's an add.
      SETOVER(arg1, arg2, result);
           
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 3;
      cycles = 4;

      break;

    case OPCODE_SBC_IND_X:
      OPPRINTF("OPCODE_SBC_IND_X:\n");
      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      
      arg1 = processor->regs.accumulator;
	    
      // calculate addr
      
      // get the operand
      addr = MemoryGetByteAt(processor->regs.pc+1);
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
      if (FLAG_CARRY(processor->regs.flags)){
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
	FLAG_CARRY_CLEAR(processor->regs.flags);
      else
	FLAG_CARRY_SET(processor->regs.flags);

      // Magic makes overflow logic work as if it's an add.
      SETOVER(arg1, arg2, result);
           
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 6;

      break;
      
    case OPCODE_SBC_IND_Y:
      OPPRINTF("OPCODE_SBC_IND_Y:\n");

      assert(!FLAG_DECIMAL(processor->regs.flags)); 

      arg1 = processor->regs.accumulator;
	    
      // calculate addr

      // Get the operand
      addr = MemoryGetByteAt(processor->regs.pc+1);

      // Get the base memory address
      addr = MemoryGetTwoBytesAt(addr);

      // Add y to the addr
      addr += processor->regs.y;

      // Retrieve the indirect addr.
      addr = MemoryGetByteAt(arg2);

      // get byte
      arg2 = MemoryGetByteAt(addr);

      // Get its twos compliment
      // Negate the bits
      arg2 = ~arg2;

      // Add one unless (not) carry indicates that borrow occurred
      // thus the difference will be one less.
      if (FLAG_CARRY(processor->regs.flags)){
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
	FLAG_CARRY_CLEAR(processor->regs.flags);
      else
	FLAG_CARRY_SET(processor->regs.flags);

      // Magic makes overflow logic work as if it's an add.
      SETOVER(arg1, arg2, result);
           
      SETSIGN(processor->regs.accumulator);
      SETZERO(processor->regs.accumulator);
      processor->regs.pc += 2;
      cycles = 5;

      break;

    case OPCODE_STA_ZERO:
      OPPRINTF("OPCODE_STA_ZERO:\n");
      arg1 = processor->regs.accumulator;
      addr = MemoryGetByteAt(processor->regs.pc+1); 
      MemorySetByteAt(addr, arg1);
      // memmap->memory[addr] = arg1;
      processor->regs.pc += 2;
      cycles = 3;

      break;
    case OPCODE_STA_ZERO_X:
      OPPRINTF("OPCODE_STA_ZERO_X:\n");
      arg1 = processor->regs.accumulator;
      addr = MemoryGetByteAt(processor->regs.pc+1) + processor->regs.x;
      addr &= 0xFF;
      //printf("arg1: %#2x, arg2: %#2x\n", arg1, arg2);
      // memmap->memory[addr] = arg1;
      MemorySetByteAt(addr, arg1);
      processor->regs.pc += 2;
      cycles = 4;

      break;
    case OPCODE_STA_ABS:
      OPPRINTF("OPCODE_STA_ABS:\n");
      arg1 = processor->regs.accumulator;
      addr =  MemoryGetTwoBytesAt(processor->regs.pc+1);
      MemorySetByteAt(addr, arg1);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 4;

      break;
    case OPCODE_STA_ABS_X:
      OPPRINTF("OPCODE_STA_ABS_X:\n");
      arg1 = processor->regs.accumulator;
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1) +
	processor->regs.x;
      MemorySetByteAt(addr, arg1);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 5;

      break;
    case OPCODE_STA_ABS_Y:
      OPPRINTF("OPCODE_STA_ABS_Y:\n");
      arg1 = processor->regs.accumulator;
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1) +
	processor->regs.y;
      MemorySetByteAt(addr, arg1);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 5;

      break;
    case OPCODE_STA_IND_X:
      OPPRINTF("OPCODE_STA_IND_X:\n");
      arg1 = processor->regs.accumulator;
      tmp = MemoryGetByteAt(processor->regs.pc+1);
      tmp += processor->regs.x;
      addr = MemoryGetTwoBytesAt(tmp);
      MemorySetByteAt(addr, arg1);
      processor->regs.pc += 2;
      cycles = 6;

      break;
    case OPCODE_STA_IND_Y:
      OPPRINTF("OPCODE_STA_IND_Y:\n");
      arg1 = processor->regs.accumulator;
     
      addr = MemoryGetTwoBytesAt(MemoryGetByteAt(processor->regs.pc+1));
      addr += processor->regs.y;
      
      MemorySetByteAt(addr, arg1);

      processor->regs.pc += 2;
      cycles = 6;

      break;
    case OPCODE_TXS:
      OPPRINTF("OPCODE_TXS:\n");
      processor->regs.sp = processor->regs.x;
      processor->regs.pc = processor->regs.pc+1;
      cycles = 2;

      break;
    case OPCODE_TSX:
      OPPRINTF("OPCODE_TSX:\n");
      processor->regs.x = processor->regs.sp;
      processor->regs.pc = processor->regs.pc+1;
      cycles = 2;

      break;
    case OPCODE_PHA:
      OPPRINTF("OPCODE_PHA:\n");
      StackPushByte(processor->regs.accumulator);
      processor->regs.pc = processor->regs.pc+1;
      cycles = 3;

      break;      
      
    case OPCODE_PLA:
      OPPRINTF("OPCODE_PLA:\n");
      processor->regs.accumulator = StackPopByte();
      processor->regs.pc = processor->regs.pc+1;
      cycles = 4;

      break;      

    case OPCODE_PHP:
      OPPRINTF("OPCODE_PHP:\n");
      StackPushByte(processor->regs.flags);
      processor->regs.pc = processor->regs.pc+1;
      cycles = 3;

      break;      

    case OPCODE_PLP:
      OPPRINTF("OPCODE_PLP:\n");
      processor->regs.flags = StackPopByte();
      processor->regs.pc = processor->regs.pc+1;
      cycles = 4;

      break;      
    case OPCODE_STX_ZERO:
      OPPRINTF("OPCODE_STX_ZERO:\n");
      arg1 = processor->regs.x;
      addr = MemoryGetByteAt(processor->regs.pc+1); 
      MemorySetByteAt(addr, arg1);
      processor->regs.pc += 2;
      cycles = 3;

      break;
    case OPCODE_STX_ZERO_Y:
      OPPRINTF("OPCODE_STX_ZERO_Y:\n");
      arg1 = processor->regs.x;
      addr = MemoryGetByteAt(processor->regs.pc+1) + processor->regs.y; 
      MemorySetByteAt(addr, arg1);
      processor->regs.pc += 2;
      cycles = 4;

      break;
    case OPCODE_STX_ABS:
      OPPRINTF("OPCODE_STX_ABS:\n");
      arg1 = processor->regs.x;
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);
      MemorySetByteAt(addr, arg1);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 4;

      break;
    case OPCODE_STY_ZERO:
      OPPRINTF("OPCODE_STY_ZERO:\n");
      arg1 = processor->regs.y;
      addr = MemoryGetByteAt(processor->regs.pc+1); 
      MemorySetByteAt(addr, arg1);
      processor->regs.pc += 2;
      cycles = 3;
           
      break;
    case OPCODE_STY_ZERO_X:
      OPPRINTF("OPCODE_STY_ZERO_X:\n");
      arg1 = processor->regs.y;
      addr = MemoryGetByteAt(processor->regs.pc+1) + processor->regs.x;
      addr &= 0xFF;
      MemorySetByteAt(addr, arg1);
      processor->regs.pc += 2;
      cycles = 4;

      break;
    case OPCODE_STY_ABS:
      OPPRINTF("OPCODE_STY_ABS:\n");
      arg1 = processor->regs.y;
      addr = MemoryGetTwoBytesAt(processor->regs.pc+1);
      MemorySetByteAt(addr, arg1);
      processor->regs.pc = processor->regs.pc+3;
      cycles = 4;
	    
      break;
    default:
      printf("ERROR: UNKNOWN OPCODE:%d\n", opcode);
      //programrunning = false;

      break;
    }
    assert(cycles != 0);

    processor->cycles = cycles;

    return 0;
}



int
CpuCreate()
{
  /* pthread_t thread; */
  /* int rc; */
  /* long t; */
  //  Cpu* processor = NULL;
  
  processor = (Cpu*)calloc(1, sizeof(Cpu));
  if ( !processor )
    printf("%s: failed", __FUNCTION__);
  // initial regs
  processor->regs.sp = 0xFF;

    // Start with the reset vector
  processor->regs.pc = MemoryGetTwoBytesAt(RESET_VECTOR_ADDR);

  /* rc = pthread_create(&thread, NULL, CpuLoopThd, (void *)t); */
  /* if (rc){ */
  /*   printf("ERROR; return code from pthread_create() is %d\n", rc); */
  /*   exit(-1); */
  /* } */
 
  return 0;
}


 

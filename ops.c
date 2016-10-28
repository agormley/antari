#include "includes.h"

BYTE
add(BYTE arg1, BYTE arg2, BYTE carry)
{
  ushort result = 0;
    
  assert(!FLAG_DECIMAL(processor->regs.flags)); 

  result = arg1 + arg2 + carry;

  SETSIGN((BYTE)result);
  SETZERO((BYTE)result);
  SETOVER(arg1, arg2, result);
  SETCARRY(result);

  return (BYTE)result;
}

BYTE
and(BYTE arg1, BYTE arg2){
  ushort result = 0;
  result = arg1 & arg2;

  SETSIGN((BYTE)result);
  SETZERO((BYTE)result);
  return (BYTE)result;
}

BYTE
asl(BYTE arg1){
  ushort result = 0;

  result = arg1 << 1;
  SETSIGN(result);
  SETZERO(result);
  SETCARRY(result);

  return (BYTE)result;
}

void
bit(BYTE arg1, BYTE arg2)
{
  BYTE result = 0;
  
  result = arg1 & arg2;
  
  SETZERO(result);

  processor->regs.flags |= (result & FLAG_OVER_MASK);
  processor->regs.flags |= (result & FLAG_NEG_MASK);

}

void
cmp(BYTE arg1, BYTE arg2)
{
  BYTE result = 0;
  
  
  // Get its twos compliment
  // Negate the bits
  arg2 = ~arg2;
  arg2++;

  // Subraction of two's comp is really just an add.
  result = arg1 + arg2;

  SETSIGN(result);
  SETZERO(result);

  if(arg1<arg2)
    FLAG_CARRY_CLEAR(processor->regs.flags);
  else
    FLAG_CARRY_SET(processor->regs.flags);
}

void
dec(BYTE val, ushort addr)
{
  
  val--;
  MemorySetByteAt(addr, val);
  SETSIGN(val);
  SETZERO(val);
}

BYTE
eor(BYTE arg1, BYTE arg2)
{
  BYTE result = 0;

  result = arg1 ^ arg2;

  SETSIGN(result);
  SETZERO(result);

  return result;
}

void
inc(BYTE val, ushort addr)
{
  val++;
  MemorySetByteAt(addr, val);
  SETSIGN(val);
  SETZERO(val);
}

void
lda(BYTE val)
{
  REG_A = val;
  SETSIGN(val);
  SETZERO(val);
}

void
ldx(BYTE val)
{
  REG_X = val;
  SETSIGN(val);
  SETZERO(val);
}

void
ldy(BYTE val)
{
  REG_Y = val;
  SETSIGN(val);
  SETZERO(val);
}

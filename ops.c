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

void
lsr(BYTE *val)
{
  
 *val = 0x7F & (*val >> 1);
 SETSIGN(*val);
 SETZERO(*val);
 if(*val & 0xFE)
   FLAG_CARRY_SET(REG_ST);
 else
   FLAG_CARRY_CLEAR(REG_ST);

}

void
ora(BYTE val)
{
  REG_A |= val;

  SETSIGN(REG_A);
  SETZERO(REG_A);
    
}

void
rol(BYTE *val)
{
  ushort result = 0;

  result = *val << 1;

  // Set lowest bit according to carry flag
  if(FLAG_CARRY(REG_ST))
    result |= 0x1;
  else
    result &= 0x1FE;

  // write back value
  *val = (char)result;

  // set flags
  SETSIGN(*val);
  SETZERO(*val);
  SETCARRY(result);

}

void
ror(BYTE *val)
{
  ushort result = 0;
  BYTE carry = *val & 0xFE;

  // Calculate new value
  result = *val >> 1;
  
  // Set highest bit according to carry flag
  if(FLAG_CARRY(REG_ST))
    result |= 0x80;
  else
    result &= 0x7F;
  
  // write back value
  *val = (char)result;

  // set flags
  SETSIGN(*val);
  SETZERO(*val);
  
  if(carry)
    FLAG_CARRY_SET(REG_ST);
  else
    FLAG_CARRY_CLEAR(REG_ST);
}

BYTE
sbc(BYTE arg1, BYTE arg2, BYTE carry)
{
  ushort result = 0;
  BYTE tmp = 0; 
  assert(!FLAG_DECIMAL(processor->regs.flags)); 

  // Get its twos compliment
  // Negate the bits
  arg2 = ~arg2;

  // Add one unless (not) carry indicates that borrow occurred
  // thus the difference will be one less.
  if (carry){
    tmp = 1;
  }
  
  result = arg1 + arg2 + tmp;

  SETSIGN((BYTE)result);
  SETZERO((BYTE)result);
  SETOVER(arg1, arg2, result);

  // Carry is reverse borrow.
  if( result > 255 )
    FLAG_CARRY_CLEAR(REG_ST);
  else
    FLAG_CARRY_SET(REG_ST);
  
  return (BYTE)result;
}

void
sta(ushort addr)
{
  MemorySetByteAt(addr, REG_A);
}

void
stx(ushort addr)
{
  MemorySetByteAt(addr, REG_X);
}

void
sty(ushort addr)
{
  MemorySetByteAt(addr, REG_Y);
}

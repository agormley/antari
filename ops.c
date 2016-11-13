#include "includes.h"

BYTE
add(BYTE arg1, BYTE arg2, BYTE carry)
{
  ushort result = 0;

  result = arg1 + arg2 + carry;
  
  // Best I can tell, these are set based on binary addition
  // So do that unconditionally.
  SETSIGN((BYTE)result);
  SETZERO((BYTE)result);
  SETOVER(arg1, arg2, result);
    
  
  if (FLAG_DECIMAL(REG_ST)) {
    BYTE ones = (arg1 & 0xF) + (arg2 & 0xF) + carry;
    if (ones > 0x9){
      carry = 0x10;
      ones = ones % 10;
    }
    else{
      carry = 0;
    }
    
    BYTE tens = (arg1 & 0xF0) + (arg2 & 0xF0) + carry;
    if (tens / 0xA0){
      FLAG_CARRY_SET(REG_ST);
    }
    else {
      FLAG_CARRY_CLEAR(REG_ST);
    }
    tens = tens % 0xA0;
    
    result = tens | ones;
    return (BYTE)result;
  }

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

  if(*val & 0x1)
    FLAG_CARRY_SET(REG_ST);
  else
    FLAG_CARRY_CLEAR(REG_ST);
  
  *val = 0x7F & (*val >> 1);
  SETSIGN(*val);
  SETZERO(*val);
  
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
  BYTE carry = *val & 0x1;

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
  BYTE tmp = 1; 
  BYTE savedarg2 = arg2;
  // Get its twos compliment
  // Negate the bits
  arg2 = ~arg2;

  // Add one unless (not) carry indicates that borrow occurred
  // thus the difference will be one less.
  tmp = carry;

  result = arg1 + arg2 + tmp;

  SETSIGN((BYTE)result);
  SETZERO((BYTE)result);
  SETOVER(arg1, arg2, result);

  if (FLAG_DECIMAL(REG_ST)) {
    BYTE borrowOnes = 0;
    BYTE borrowTens = 0;
    if((arg1 & 0xF) < (arg2 & 0xF)) {
      borrowOnes = 0xA;
    }
    BYTE ones = borrowOnes + (arg1 & 0xF) - (arg2 & 0xF) - (1 ^ carry);

    if(((arg1 & 0xF0) - borrowOnes) < (arg2 & 0xF0)) {
      borrowTens = 0xA0;
    }

    BYTE tens = borrowTens + (arg1 & 0xF0) - (arg2 & 0xF0) - borrowOnes;
    if (borrowTens){
      FLAG_CARRY_CLEAR(REG_ST);
    }
    else {
      FLAG_CARRY_SET(REG_ST);
    }
    
    result = tens | ones;
    return (BYTE)result;
  }
  
  // Carry is reverse borrow.
    // Carry is reverse borrow.
  if( arg1 < savedarg2 )
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

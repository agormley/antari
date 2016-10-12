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

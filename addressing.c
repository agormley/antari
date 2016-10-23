#include "includes.h"

BYTE
getImmediate(ushort addr)
{
  return MemoryGetByteAt(addr);
}

BYTE
getZero(ushort addr, ushort* mid){
  *mid = MemoryGetByteAt(addr );
  return MemoryGetByteAt(*mid);
}

BYTE
getZeroX(ushort addr, ushort* mid){
  ushort final_addr = 0;
  
  final_addr = *mid = MemoryGetByteAt( addr );
  final_addr += processor->regs.x;
  final_addr &= 0xFF;
    
  return MemoryGetByteAt(final_addr);
}

BYTE
getAbsolute(ushort addr, ushort* mid)
{
    *mid  = MemoryGetTwoBytesAt(addr);
    return MemoryGetByteAt(*mid);
}

BYTE
getAbsoluteX(ushort addr, ushort* mid)
{
  *mid  = MemoryGetTwoBytesAt(addr);
    
  // get byte
  return MemoryGetByteAt(processor->regs.x + *mid);
}

BYTE
getAbsoluteY(ushort addr, ushort* mid)
{
  *mid  = MemoryGetTwoBytesAt(addr);
  
  // get byte
  return MemoryGetByteAt(processor->regs.y + *mid);
}

BYTE
getIndirect(ushort addr, ushort* mid)
{
  *mid = MemoryGetTwoBytesAt(MemoryGetTwoBytesAt(addr));
  return MemoryGetByteAt(*mid);
}

BYTE
getIndirectX(ushort addr, ushort* mid)
{
  *mid = MemoryGetByteAt(addr);
    
  return MemoryGetByteAt(MemoryGetTwoBytesAt(*mid +
					     processor->regs.x));
}

BYTE
getIndirectY(ushort addr, ushort* mid)
{
 
  *mid = MemoryGetTwoBytesAt(MemoryGetByteAt(addr));
  
  return MemoryGetByteAt(processor->regs.y + *mid);
}

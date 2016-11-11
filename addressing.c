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
getZeroX(ushort pc, ushort* mid, ushort* addr){
  ushort final_addr = 0;
  
  final_addr = *mid = MemoryGetByteAt( pc );
  final_addr += processor->regs.x;
  final_addr &= 0xFF;
  if(addr)
    *addr = final_addr;
  
  return MemoryGetByteAt(final_addr);
}

BYTE
getZeroY(ushort pc, ushort* mid, ushort* addr)
{
  ushort final_addr = 0;
  
  final_addr = *mid = MemoryGetByteAt( pc );
  final_addr += REG_Y;
  final_addr &= 0xFF;
  if(addr)
    *addr = final_addr;
  return MemoryGetByteAt(final_addr);
}

BYTE
getAbsolute(ushort addr, ushort* mid)
{
    *mid  = MemoryGetTwoBytesAt(addr);
    return MemoryGetByteAt(*mid);
}

BYTE
getAbsoluteX(ushort addr, ushort* mid, ushort *write_addr)
{
  *mid  = MemoryGetTwoBytesAt(addr);
  if(write_addr)
    *write_addr = processor->regs.x + *mid;
  // get byte
  return MemoryGetByteAt(processor->regs.x + *mid);
}

BYTE
getAbsoluteY(ushort addr, ushort* mid, ushort *write_addr)
{
  *mid  = MemoryGetTwoBytesAt(addr);
  if(write_addr)
    *write_addr = REG_Y + *mid;
  
  // get byte
  return MemoryGetByteAt(REG_Y + *mid);
}

BYTE
getIndirect(ushort addr, ushort* mid)
{
  *mid = MemoryGetTwoBytesAt(MemoryGetTwoBytesAt(addr));
  return MemoryGetByteAt(*mid);
}

BYTE
getIndirectX(ushort addr, ushort* mid, ushort *write_addr)
{
  *mid = MemoryGetByteAt(addr);
  if(write_addr)
    *write_addr = MemoryGetTwoBytesAt(*mid + REG_X); 
  return MemoryGetByteAt(MemoryGetTwoBytesAt(*mid + REG_X));
}

BYTE
getIndirectY(ushort addr, ushort* mid, ushort *write_addr)
{
  ushort address = 0;
  
  assert(mid);
  *mid = MemoryGetByteAt(addr);
  address = MemoryGetTwoBytesAt(*mid);

  if(write_addr)
    *write_addr = address + REG_Y; 

  return MemoryGetByteAt(REG_Y + address);
}

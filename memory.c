#include "includes.h"

void MemoryCreate(){
  memmap = (MemoryMap*)calloc(1, sizeof(MemoryMap));
}

unsigned short
MemoryGetTwoBytesAt(unsigned short addr)
{
  ushort bytes = 0;

  ushort addr1 = (addr & MEMORY_MASK);
  //TODO: check that addr is in range'
  bytes = memmap->memory[addr1];
  bytes = bytes | memmap->memory[((addr+1) & MEMORY_MASK)]<<8;
  return bytes;
}

unsigned char
MemoryGetByteAt(unsigned short addr)
{
  assert(addr >= 0 && addr < 0x10000);
  
  unsigned char byte = 0;
  ushort addr1 = (addr & MEMORY_MASK);

  if (addr >= 280  && addr < 299)
    printf("Getting PIA addr %#x\n", addr);

  if (addr >= 0  && addr < 45)
    printf("Getting TIA addr %#x\n", addr);

  
  byte = memmap->memory[addr1];
  return byte;
}

void
MemorySetByteAt(unsigned short addr, unsigned char byte)
{
  assert(addr >= 0 && addr < 0x10000);
  
  ushort addr1 = (addr & MEMORY_MASK);

  if (addr >= 0  && addr < 45)
    printf("Setting TIA addr %#x\n", addr);

  
  //TODO: check that addr is in range'
  memmap->memory[addr1] = byte ;
}

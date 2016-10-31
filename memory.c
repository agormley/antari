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

  /* if (addr >= 0  && addr < 45) */
  /*   printf("Getting TIA addr %#x\n", addr); */
  switch(addr){
    //clear button presses on read.
  case SWCHB:
    pia->game_select = false;
    pia->game_reset = false;
    break;
  default:
    break;
  }
  
  byte = memmap->memory[addr1];
  return byte;
}

void
MemorySetByteAt(unsigned short addr, unsigned char byte)
{
  assert(addr >= 0 && addr < 0x10000);
  
  ushort addr1 = (addr & MEMORY_MASK);
  
  //  if (addr1 >= TIA_WRITE_PF0  && addr <= TIA_WRITE_PF2)
  LOG("addr: %#4x, byte: %#x, row %d, column %d", addr1, byte, tia->row, tia->column);

  // check strobe registers
  // please put vars inside a struct, preferabl tia
  switch(addr1){
  case TIA_WRITE_WSYNC:
    Wsync = true;
    break;
  case TIA_WRITE_RSYNC:
    printf("%s: rsync strobe not implemented\n", __FUNCTION__);
    break;
  case TIA_WRITE_RESP0:
    ResetPlayer0 = true;
    break;
  case TIA_WRITE_RESP1:
    ResetPlayer1 = true;
    break;
  case TIA_WRITE_RESM0:
    ResetMissile0 = true;
    break;
  case TIA_WRITE_RESM1:
    ResetMissile1 = true;
    break;
  case TIA_WRITE_RESBL:
    ResetBall = true;
    break;
 
  case TIA_WRITE_HMOVE:
    tia->hMotionPending = true;
    break;
  case TIA_WRITE_HMCLR:
    TiaClearHMotion();

    break;
  case TIA_WRITE_CXCLR:

    break;
  case TIM1T:
    pia->timer_pending = true;
    pia->timer_interval = 1;
    pia->timer_interval_timer = 0;
    pia->timer = byte;
    break;
  case TIM8T:
    pia->timer_pending = true;
    pia->timer_interval = 8;
    pia->timer_interval_timer = 0;
    pia->timer = byte;
    break;
  case TIM64T:
    pia->timer_pending = true;
    pia->timer_interval = 64;
    pia->timer_interval_timer = 0;
    pia->timer = byte;
    break;
  case T1024T:
    pia->timer_pending = true;
    pia->timer_interval = 1024;
    pia->timer_interval_timer = 0;
    pia->timer = byte;
    break;

  default:
    break;
    
  }
  
  //TODO: check that addr is in range'
  memmap->memory[addr1] = byte ;
}

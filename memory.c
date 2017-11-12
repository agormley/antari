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
  //  assert(addr >= 0 && addr < 0x10000);
  
  unsigned char byte = 0;
  ushort addr1 = (addr & MEMORY_MASK);

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

extern int size;

void
MemorySetByteAt(unsigned short addr, unsigned char byte)
{
  //assert(addr >= 0 && addr < 0x10000);
  ushort addr1; 
  if(size == 2048) {
    addr1 = (addr & 0x7FF);
  }
  else
    addr1 = (addr & MEMORY_MASK);
  
  //  if (addr1 >= TIA_WRITE_PF0  && addr <= TIA_WRITE_PF2)
//  LOG("addr: %#4x, byte: %#x, row %d, column %d", addr1, byte, tia->row, tia->column);

  // check strobe registers
  // please put vars inside a struct, preferabl tia
  switch(addr1){
  case TIA_WRITE_WSYNC:
    tia->wsync = true;
    break;
  case TIA_WRITE_RSYNC:
    printf("%s: rsync strobe not implemented\n", __FUNCTION__);
    break;
  case TIA_WRITE_RESP0:
    // printf("%s: reset p0 %d\n", __FUNCTION__, tia->column);
    tia->player0->pixBit = 0;
    tia->player0->clkStart = tia->column-HORIZONTAL_BLANK<0?16:
      tia->column-HORIZONTAL_BLANK+16;

    tia->player0->reset = true;
    break;
  case TIA_WRITE_RESP1:
    // printf("%s: reset p1 %d\n", __FUNCTION__, tia->column);
    tia->player1->pixBit = 0;
    tia->player1->clkStart = tia->column-HORIZONTAL_BLANK<0?0:tia->column-HORIZONTAL_BLANK;;

    tia->player1->reset = true;
    break;
  case TIA_WRITE_RESM0:
    //printf("%s: reset m0 %d \n", __FUNCTION__, tia->column);

    tia->missile0->pixBit = 0;
    tia->missile0->clkStart = tia->column-HORIZONTAL_BLANK<0?0:tia->column-HORIZONTAL_BLANK;
    tia->missile0->reset = true;
    
    break;
  case TIA_WRITE_RESM1:
    //  printf("%s: reset m1 %d \n", __FUNCTION__, tia->column);
    tia->missile1->pixBit = 0;
    tia->missile1->clkStart = tia->column-HORIZONTAL_BLANK<0?0:tia->column-HORIZONTAL_BLANK;
    tia->missile1->reset = true;
   
    break;
  case TIA_WRITE_RESBL:
    //  printf("%s: reset ball %d\n", __FUNCTION__, tia->column);
    tia->ball->pixBit = 0;
    tia->ball->clkStart = tia->column-HORIZONTAL_BLANK<0?0:tia->column-HORIZONTAL_BLANK;;
    tia->ball->enabled= true;
    tia->ball->reset = true;
    break;
  case TIA_WRITE_VDELBL:
    printf("VDELBL\n");
    tia->ball->enabled = true;
    break;
  case TIA_WRITE_HMOVE:
    tia->hMotionPending = true;
    break;
  case TIA_WRITE_HMCLR:
    TiaClearHMotion();

    break;
  case TIA_WRITE_CXCLR:
      
      MEM_RD(TIA_READ_CXM0P) = 0; 
      
      MEM_RD(TIA_READ_CXM1P) = 0;

      MEM_RD(TIA_READ_CXP0FB) = 0;

      MEM_RD(TIA_READ_CXP1FB) = 0;
  
      MEM_RD(TIA_READ_CXM0FB) = 0;

      MEM_RD(TIA_READ_CXM1FB) = 0;

      MEM_RD(TIA_READ_CXBLPF) = 0;

      MEM_RD(TIA_READ_CXPPMM) = 0;

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
  case TIA_WRITE_AUDF0:
    audio1->frequency = byte;
    break;
  case TIA_WRITE_AUDF1:
    audio2->frequency = byte;
    break;
  case TIA_WRITE_AUDV0:
    audio1->volume = byte;
    break;
  case TIA_WRITE_AUDV1:
    audio2->volume = byte;
    break;
  default:
    break;
  }
  
  //TODO: check that addr is in range'
  memmap->memory[addr1] = byte ;
}



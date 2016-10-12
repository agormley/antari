#include "includes.h"

int
PiaCreate(){
  pia = calloc(1, sizeof(Pia));
  assert(pia);
  // set interval to something sane
  pia->timer_interval = 1;
  
  // Let's start with color;
  pia->color = true;
  pia->game_select = 1;
  pia->game_reset = 1;
  
  MemorySetByteAt(SWCHB, 1 << 3);
  return 0;
}

int
PiaWriteRegs(){

  BYTE byte = 0;
  if(pia->p1difficulty){
    byte = 1 << 7;
  }
  if(pia->p0difficulty){
    byte |= 1 << 6;
  }
  if(pia->color){
    byte |= 1 << 3;
  }
  if(!pia->game_select){
    byte |= 1 << 1;
  }
  if(!pia->game_reset){
    byte |= 1 << 0;
  }

 
  memmap->memory[SWCHB] = byte;
  byte = 0;
  if(pia->p0left){
    byte |= 1 << 6;
    pia->p0left = false;
  }
  if(pia->p0right){
    byte |= 1 << 7;
    pia->p0right = false;
  }
  if(pia->p0up){
    byte |= 1 << 4;
    pia->p0up = false;
  }
  if(pia->p0down){
    byte |= 1 << 5;
    pia->p0down = false;
  }

  memmap->memory[SWCHA] = byte;
  
  if(pia->p0button){
    memmap->tia_read[TIA_READ_INPT4] = 1 << 7;
    pia->p0button = false;
  }
  
 
  return 0;
}
int
PiaReadRegs()
{
  return 0;
}

int
PiaCycle()
{
  // read all the registers!
  PiaReadRegs();
  PiaWriteRegs();
  int intervalTimer = pia->timer_interval_timer;

  if (intervalTimer == 0)
    {
      // underflow, interval is now 1
      if (pia->timer == 0) {
	pia->timer_interval = 1;
	memmap->memory[INSTAT] =
	  3 << 6;
      }
      pia->timer--; 
    }
  pia->timer_interval_timer =
    (intervalTimer + 1) %
    pia->timer_interval;
  
  memmap->memory[INTIM] = pia->timer;
  return 0;

}

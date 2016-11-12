#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <GL/glut.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include "defs.h"
#define SIGN_MASK (1<<7)
#define SETOVER(a,b,c) (((~(a ^ b))&(a ^ c)&SIGN_MASK)>1)

BYTE
sbc(BYTE arg1, BYTE arg2, BYTE *carry, BYTE *overflow, BYTE *sign, BYTE *zero)
{
  ushort result = 0;
  BYTE tmp = 0;
  BYTE tmp2 = arg2;
  // Get its twos compliment
  // Negate the bits
  arg2 = ~arg2;

  // Add one unless (not) carry indicates that borrow occurred
  // thus the difference will be one less.
  if (*carry){
    tmp = 1;
  }
  
  result = arg1 + arg2 + tmp;

  *overflow = SETOVER(arg1, arg2, result);
  if ( result & 0xFF  == 0) *zero = 1;
  else *zero = 0;
  if ( result & SIGN_MASK ) *sign = 1;
  else *sign = 0;
  /*
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
  */
  
  // Carry is reverse borrow.
  if( arg1 < tmp2 )
   *carry = 0;
  else
    *carry = 1;
  
  return (BYTE)result;
}


int
TiaConvertHmToInt(BYTE hm)
{
  int result = 0;
  int sign = 0;

  sign = hm & 0x80;
  result = hm & 0x70;
  result = result >> 4;

  if(sign){
    result++;

    result = -result;

  }

  return result;
}

typedef struct test_pia{
  unsigned char timer;
  bool timer_pending;
  int timer_interval;
  int timer_interval_timer;
}PIA;
PIA *pia;
int
HandleTimer()
{
  int intervalTimer = pia->timer_interval_timer;
  
  if (intervalTimer == 0)
    {
      // underflow, interval is now 1
      if (pia->timer == 0) {
	pia->timer_interval = 1;
	printf("%s: timer has finished\n", __FUNCTION__);
      }
      pia->timer--; 
    }
  pia->timer_interval_timer =
    (intervalTimer + 1) %
    pia->timer_interval;
  
  printf("%s: timer is %#x\n", __FUNCTION__, pia->timer);
  return 0;
}



int
TimerTest()
{
  int i = 0;
  int test_time = 1000;
  
  for(i = 0; i < test_time; i++)
    {
      HandleTimer();
    }
  return 0;
}

int TimerUnitTests()
{

  pia = calloc(1, sizeof(PIA));
  pia->timer_interval = 1;

  // 1T
  printf("1T test\n");
  pia->timer_pending = true;
  pia->timer_interval = 1;
  pia->timer_interval_timer = 0;
  pia->timer = 0xF0;
  TimerTest();

  
  // 8T
  printf("8T test\n");
  pia->timer_pending = true;
  pia->timer_interval = 8;
  pia->timer_interval_timer = 0;
  pia->timer = 0x10;
  TimerTest();
  
  return 0;
}


int main(int argc, char *argv[])
{
  int i;
  BYTE hm = 0;

  TimerUnitTests();

  
  for(i = 0; i < 16; i++, hm += 0x10){

  
    printf("HorMotion: %#x, result: %d\n",
	   hm, TiaConvertHmToInt(hm));

  
  }
  /*
Inputs      Outputs		Example
M7 N7 C6 C7 B S7 V Borrow / Overflow	                        Hex	        Unsigned	Signed
0  1  0  0  1 0  0 Unsigned borrow but no signed overflow	0x50-0xf0=0x60	80-240=96	80--16=96
0  1  1  0  1 1  1 Unsigned borrow and signed overflow	        0x50-0xb0=0xa0	80-176=160	80--80=-96
0  0  0  0  1 1  0 Unsigned borrow but no signed overflow	0x50-0x70=0xe0	80-112=224	80-112=-32
0  0  1  1  0 0  0 No unsigned borrow or signed overflow	0x50-0x30=0x120	80-48=32	80-48=32
1  1  0  0  1 1  0 Unsigned borrow but no signed overflow	0xd0-0xf0=0xe0	208-240=224	-48--16=-32
1  1  1  1  0 0  0 No unsigned borrow or unsigned overflow	0xd0-0xb0=0x120	208-176=32	-48--80=32
1  0  0  1  0 0  1 No unsigned borrow but signed overflow	0xd0-0x70=0x160	208-112=96	-48-112=96
1  0  1  1  0 1  0 No unsigned borrow or signed overflow	0xd0-0x30=0x1a0	208-48=160	-48-48=-96
   */

  unsigned char arg1 = 0;
  unsigned char arg2 = 0;
  unsigned short result = 0;
  unsigned char carry = 0;
  unsigned char overflow =0;
  unsigned char sign = 0;
  unsigned char zero =0;

  i = 0;

  unsigned char a1[] = {80, 80, 80, 80, 208, 208, 208, 208};
  unsigned char a2[] = {240, 176, 112, 48, 240, 176, 112, 48};  

  for(i = 0; i < 8; i++)
    {
      arg1 = a1[i];
      arg2 = a2[i];
      carry = 1;

      result = sbc(arg1, arg2, &carry, &overflow, &sign, &zero);
      /* result = arg1 + (~arg2) + carry; */
      /* overflow = SETOVER(arg1, (~arg2), result); */

      /* //carry = result>255?0:1; */
      /* carry = ~result>255?1:0; */
      
      printf("arg1: %#x, %u, arg2: %#x, %u, result: %#x, %u, carry: %u, overflow: %u, sign %u, zero %u\n",
	     arg1, arg1, arg2, arg2, (unsigned char)result, (unsigned char) result, carry, overflow, sign, zero);
      
    }
  return 0;
}

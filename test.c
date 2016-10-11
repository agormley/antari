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



int main(int argc, char *argv[])
{
  int i;
  BYTE hm = 0;
  for(i = 0; i < 16; i++, hm += 0x10){

  
    printf("HorMotion: %#x, result: %d\n",
	   hm, TiaConvertHmToInt(hm));

  
  }
  /*
Inputs	Outputs		Example
M7 	N7 	C6 	C7 	B	S7 	V	Borrow / Overflow	Hex	Unsigned	Signed
0	1	0	0	1	0	0	Unsigned borrow but no signed overflow	0x50-0xf0=0x60	80-240=96	80--16=96
0	1	1	0	1	1	1	Unsigned borrow and signed overflow	0x50-0xb0=0xa0	80-176=160	80--80=-96
0	0	0	0	1	1	0	Unsigned borrow but no signed overflow	0x50-0x70=0xe0	80-112=224	80-112=-32
0	0	1	1	0	0	0	No unsigned borrow or signed overflow	0x50-0x30=0x120	80-48=32	80-48=32
1	1	0	0	1	1	0	Unsigned borrow but no signed overflow	0xd0-0xf0=0xe0	208-240=224	-48--16=-32
1	1	1	1	0	0	0	No unsigned borrow or unsigned overflow	0xd0-0xb0=0x120	208-176=32	-48--80=32
1	0	0	1	0	0	1	No unsigned borrow but signed overflow	0xd0-0x70=0x160	208-112=96	-48-112=96
1	0	1	1	0	1	0	No unsigned borrow or signed overflow	0xd0-0x30=0x1a0	208-48=160	-48-48=-96
   */

  unsigned char arg1 = 0;
  unsigned char arg2 = 0;
  unsigned short result = 0;
  unsigned char carry = 0;
  unsigned char overflow =0;
  i = 0;

  unsigned char a1[] = {80, 80, 80, 80, 208, 208, 208, 208};
  unsigned char a2[] = {240, 176, 112, 48, 240, 176, 112, 48};  

  for(i = 0; i < 8; i++)
    {
      arg1 = a1[i];
      arg2 = a2[i];
      carry = 1;
      
      result = arg1 + (~arg2) + carry;
      overflow = SETOVER(arg1, (~arg2), result);

      //carry = result>255?0:1;
      carry = ~result>255?1:0;
      
      printf("arg1: %#x, %u, arg2: %#x, %u, result: %#x, %u, carry: %u, overflow: %u\n",
	     arg1, arg1, arg2, arg2, (unsigned char)result, (unsigned char) result, carry, overflow);
      
    }
  return 0;
}

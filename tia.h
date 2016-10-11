/*
*********************
* TIA Documentation *
*********************
 
--------------------------------------
TIA Addressing Notes for the Atari VCS
--------------------------------------
A12 is connected to /CS0 (Chip Select 0 - active low)
A7 is connected to /CS3  (Chip Select 3 - active low)
A[11:8] and A6 are not connected to the TIA

VCC is connected to CS1  (Chip Select 1 - active high)
GND is connected to /CS2 (Chip Select 2 - active low)

--------------------------------------------
$0000 - $003F = TIA.......(write)......(read)
--------------------------------------------
$0000 = TIA Address $00 - (VSYNC)......(CXM0P)
$0001 = TIA Address $01 - (VBLANK).....(CXM1P)
$0002 = TIA Address $02 - (WSYNC)......(CXP0FB)
$0003 = TIA Address $03 - (RSYNC)......(CXP1FB)
$0004 = TIA Address $04 - (NUSIZ0).....(CXM0FB)
$0005 = TIA Address $05 - (NUSIZ1).....(CXM1FB)
$0006 = TIA Address $06 - (COLUP0).....(CXBLPF)
$0007 = TIA Address $07 - (COLUP1).....(CXPPMM)
$0008 = TIA Address $08 - (COLUPF).....(INPT0)
$0009 = TIA Address $09 - (COLUBK).....(INPT1)
$000A = TIA Address $0A - (CTRLPF).....(INPT2)
$000B = TIA Address $0B - (REFP0)......(INPT3)
$000C = TIA Address $0C - (REFP1)......(INPT4)
$000D = TIA Address $0D - (PF0)........(INPT5)
$000E = TIA Address $0E - (PF1)........(UNDEFINED)
$000F = TIA Address $0F - (PF2)........(UNDEFINED)
$0010 = TIA Address $10 - (RESP0)......(CXM0P)
$0011 = TIA Address $11 - (RESP1)......(CXM1P)
$0012 = TIA Address $12 - (RESM0)......(CXP0FB)
$0013 = TIA Address $13 - (RESM1)......(CXP1FB)
$0014 = TIA Address $14 - (RESBL)......(CXM0FB)
$0015 = TIA Address $15 - (AUDC0)......(CXM1FB)
$0016 = TIA Address $16 - (AUDC1)......(CXBLPF)
$0017 = TIA Address $17 - (AUDF0)......(CXPPMM)
$0018 = TIA Address $18 - (AUDF1)......(INPT0)
$0019 = TIA Address $19 - (AUDV0)......(INPT1)
$001A = TIA Address $1A - (AUDV1)......(INPT2)
$001B = TIA Address $1B - (GRP0).......(INPT3)
$001C = TIA Address $1C - (GRP1).......(INPT4)
$001D = TIA Address $1D - (ENAM0)......(INPT5)
$001E = TIA Address $1E - (ENAM1)......(UNDEFINED)
$001F = TIA Address $1F - (ENABL)......(UNDEFINED)
$0020 = TIA Address $20 - (HMP0).......(CXM0P)
$0021 = TIA Address $21 - (HMP1).......(CXM1P)
$0022 = TIA Address $22 - (HMM0).......(CXP0FB)
$0023 = TIA Address $23 - (HMM1).......(CXP1FB)
$0024 = TIA Address $24 - (HMBL).......(CXM0FB)
$0025 = TIA Address $25 - (VDELP0).....(CXM1FB)
$0026 = TIA Address $26 - (VDELP1).....(CXBLPF)
$0027 = TIA Address $27 - (VDELBL).....(CXPPMM)
$0028 = TIA Address $28 - (RESMP0).....(INPT0)
$0029 = TIA Address $29 - (RESMP1).....(INPT1)
$002A = TIA Address $2A - (HMOVE)......(INPT2)
$002B = TIA Address $2B - (HMCLR)......(INPT3)
$002C = TIA Address $2C - (CXCLR)......(INPT4)
$002D = TIA Address $2D - (UNDEFINED)..(INPT5)
$002E = TIA Address $2E - (UNDEFINED)..(UNDEFINED)
$002F = TIA Address $2F - (UNDEFINED)..(UNDEFINED)
$0030 = TIA Address $30 - (UNDEFINED)..(CXM0P)
$0031 = TIA Address $31 - (UNDEFINED)..(CXM1P)
$0032 = TIA Address $32 - (UNDEFINED)..(CXP0FB)
$0033 = TIA Address $33 - (UNDEFINED)..(CXP1FB)
$0034 = TIA Address $34 - (UNDEFINED)..(CXM0FB)
$0035 = TIA Address $35 - (UNDEFINED)..(CXM1FB)
$0036 = TIA Address $36 - (UNDEFINED)..(CXBLPF)
$0037 = TIA Address $37 - (UNDEFINED)..(CXPPMM)
$0038 = TIA Address $38 - (UNDEFINED)..(INPT0)
$0039 = TIA Address $39 - (UNDEFINED)..(INPT1)
$003A = TIA Address $3A - (UNDEFINED)..(INPT2)
$003B = TIA Address $3B - (UNDEFINED)..(INPT3)
$003C = TIA Address $3C - (UNDEFINED)..(INPT4)
$003D = TIA Address $3D - (UNDEFINED)..(INPT5)
$003E = TIA Address $3E - (UNDEFINED)..(UNDEFINED)
$003F = TIA Address $3F - (UNDEFINED)..(UNDEFINED)
  */

/*
TIA - WRITE ADDRESS SUMMARY (Write only)

  00      VSYNC   ......1.  vertical sync set-clear
  01      VBLANK  11....1.  vertical blank set-clear
  02      WSYNC   <strobe>  wait for leading edge of horizontal blank
  03      RSYNC   <strobe>  reset horizontal sync counter
  04      NUSIZ0  ..111111  number-size player-missile 0
  05      NUSIZ1  ..111111  number-size player-missile 1
  06      COLUP0  1111111.  color-lum player 0 and missile 0
  07      COLUP1  1111111.  color-lum player 1 and missile 1
  08      COLUPF  1111111.  color-lum playfield and ball
  09      COLUBK  1111111.  color-lum background
  0A      CTRLPF  ..11.111  control playfield ball size & collisions
  0B      REFP0   ....1...  reflect player 0
  0C      REFP1   ....1...  reflect player 1
  0D      PF0     1111....  playfield register byte 0
  0E      PF1     11111111  playfield register byte 1
  0F      PF2     11111111  playfield register byte 2
  10      RESP0   <strobe>  reset player 0
  11      RESP1   <strobe>  reset player 1
  12      RESM0   <strobe>  reset missile 0
  13      RESM1   <strobe>  reset missile 1
  14      RESBL   <strobe>  reset ball
  15      AUDC0   ....1111  audio control 0
  16      AUDC1   ....1111  audio control 1
  17      AUDF0   ...11111  audio frequency 0
  18      AUDF1   ...11111  audio frequency 1
  19      AUDV0   ....1111  audio volume 0
  1A      AUDV1   ....1111  audio volume 1
  1B      GRP0    11111111  graphics player 0
  1C      GRP1    11111111  graphics player 1
  1D      ENAM0   ......1.  graphics (enable) missile 0
  1E      ENAM1   ......1.  graphics (enable) missile 1
  1F      ENABL   ......1.  graphics (enable) ball
  20      HMP0    1111....  horizontal motion player 0
  21      HMP1    1111....  horizontal motion player 1
  22      HMM0    1111....  horizontal motion missile 0
  23      HMM1    1111....  horizontal motion missile 1
  24      HMBL    1111....  horizontal motion ball
  25      VDELP0  .......1  vertical delay player 0
  26      VDELP1  .......1  vertical delay player 1
  27      VDELBL  .......1  vertical delay ball
  28      RESMP0  ......1.  reset missile 0 to player 0
  29      RESMP1  ......1.  reset missile 1 to player 1
  2A      HMOVE   <strobe>  apply horizontal motion
  2B      HMCLR   <strobe>  clear horizontal motion registers
  2C      CXCLR   <strobe>  clear collision latches

TIA - READ ADDRESS SUMMARY (Read only)

  30      CXM0P   11......  read collision M0-P1, M0-P0 (Bit 7,6)
  31      CXM1P   11......  read collision M1-P0, M1-P1
  32      CXP0FB  11......  read collision P0-PF, P0-BL
  33      CXP1FB  11......  read collision P1-PF, P1-BL
  34      CXM0FB  11......  read collision M0-PF, M0-BL
  35      CXM1FB  11......  read collision M1-PF, M1-BL
  36      CXBLPF  1.......  read collision BL-PF, unused
  37      CXPPMM  11......  read collision P0-P1, M0-M1
  38      INPT0   1.......  read pot port
  39      INPT1   1.......  read pot port
  3A      INPT2   1.......  read pot port
  3B      INPT3   1.......  read pot port
  3C      INPT4   1.......  read input
  3D      INPT5   1.......  read input

PIA 6532 - RAM, Switches, and Timer (Read/Write)

  80..FF  RAM     11111111  128 bytes RAM (in PIA chip) for variables and stack
  0280    SWCHA   11111111  Port A; input or output  (read or write)
  0281    SWACNT  11111111  Port A DDR, 0= input, 1=output
  0282    SWCHB   11111111  Port B; console switches (read only)
  0283    SWBCNT  11111111  Port B DDR (hardwired as input)
  0284    INTIM   11111111  Timer output (read only)
  0285    INSTAT  11......  Timer Status (read only, undocumented)
  0294    TIM1T   11111111  set 1 clock interval (838 nsec/interval)
  0295    TIM8T   11111111  set 8 clock interval (6.7 usec/interval)
  0296    TIM64T  11111111  set 64 clock interval (53.6 usec/interval)
  0297    T1024T  11111111  set 1024 clock interval (858.2 usec/interval)

*/


#define TIA_WRITE_VSYNC 0x00
#define TIA_WRITE_VBLANK 0x01
#define TIA_WRITE_WSYNC 0x02
#define TIA_WRITE_RSYNC 0x03
#define TIA_WRITE_NUSIZ0  0x04
#define TIA_WRITE_NUSIZ1  0x05
#define TIA_WRITE_COLUP0  0x06
#define TIA_WRITE_COLUP1  0x07
#define TIA_WRITE_COLUPF  0x08
#define TIA_WRITE_COLUBK  0x09
#define TIA_WRITE_CTRLPF  0x0A
#define TIA_WRITE_REFP0  0x0B
#define TIA_WRITE_REFP1  0x0C
#define TIA_WRITE_PF0  0x0D
#define TIA_WRITE_PF1  0x0E
#define TIA_WRITE_PF2  0x0F
#define TIA_WRITE_RESP0  0x10
#define TIA_WRITE_RESP1  0x11
#define TIA_WRITE_RESM0  0x12
#define TIA_WRITE_RESM1  0x13
#define TIA_WRITE_RESBL  0x14
#define TIA_WRITE_AUDC0  0x15
#define TIA_WRITE_AUDC1  0x16
#define TIA_WRITE_AUDF0  0x17
#define TIA_WRITE_AUDF1  0x18
#define TIA_WRITE_AUDV0  0x19
#define TIA_WRITE_AUDV1  0x1A
#define TIA_WRITE_GRP0  0x1B
#define TIA_WRITE_GRP1  0x1C
#define TIA_WRITE_ENAM0  0x1D
#define TIA_WRITE_ENAM1  0x1E
#define TIA_WRITE_ENABL  0x1F
#define TIA_WRITE_HMP0  0x20
#define TIA_WRITE_HMP1  0x21
#define TIA_WRITE_HMM0  0x22
#define TIA_WRITE_HMM1  0x23
#define TIA_WRITE_HMBL  0x24
#define TIA_WRITE_VDELP0  0x25
#define TIA_WRITE_VDELP1  0x26
#define TIA_WRITE_VDELBL  0x27
#define TIA_WRITE_RESMP0  0x28
#define TIA_WRITE_RESMP1  0x29
#define TIA_WRITE_HMOVE  0x2A
#define TIA_WRITE_HMCLR  0x2B
#define TIA_WRITE_CXCLR  0x2C
  

#define TIA_READ_CXM0P 0x00
#define TIA_READ_CXM1P 0x01
#define TIA_READ_CXP0FB 0x02
#define TIA_READ_CXP1FB 0x03
#define TIA_READ_CXM0FB  0x04
#define TIA_READ_CXM1FB  0x05
#define TIA_READ_CXBLPF  0x06
#define TIA_READ_CXPPMM  0x07
#define TIA_READ_INPT0  0x08
#define TIA_READ_INPT1  0x09
#define TIA_READ_INPT2  0x0A
#define TIA_READ_INPT3  0x0B
#define TIA_READ_INPT4  0x0C
#define TIA_READ_INPT5  0x0D

#define  PF0 (memmap->tia_write[TIA_WRITE_PF0])
#define  PF1 (memmap->tia_write[TIA_WRITE_PF1])
#define  PF2 (memmap->tia_write[TIA_WRITE_PF2])

#define CLOCK_COUNTS 228
#define SCAN_LINES 262
#define FRAME_CLOCK_COUNTS 160
#define FRAME_LINES 192
#define HORIZONTAL_BLANK 68
#define VERTICAL_SYNC 3
#define VERTICAL_BLANK 37
#define VERTICAL_TIMING (VERTICAL_SYNC + VERTICAL_BLANK)
#define OVERSCAN 30
#define RIGHT_FRAME (HORIZONTAL_BLANK + (FRAME_CLOCK_COUNTS / 2))


//extern Uint32 framebuffer[STELLA_VERTICAL_CLOCK_COUNTS][STELLA_HORIZONTAL_LINES];
extern Uint32 framebuffer[FRAME_LINES][FRAME_CLOCK_COUNTS];

typedef struct _sprite
{
  int color;
  int lum;
  int pixBit;
  int clkStart;
  int hMotion;
}Sprite;

typedef struct _playfield{
  bool reflect;
  bool score;
  bool priority;
  int ball_size;
  int pf_color;
  int pf_lum;
  int bk_color;
  int bk_lum;

}PlayField;


typedef struct _Tia {
  bool wsync;
  
  unsigned char pf0;
  unsigned char pf1;
  unsigned char pf2;
  int row;
  int column;
  bool hMotionPending;
  
  Sprite *player0;
  Sprite *player1;
  PlayField *playfield;
}Tia;

int TiaCreate();
int TiaCycle();
int
TiaConvertHmToInt(BYTE hm);
int TiaClearHMotion();

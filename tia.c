#include "includes.h"

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

Uint32 framebuffer[FRAME_CLOCK_COUNTS][FRAME_LINES];


/* unsigned char PF0 = 0; */
/* unsigned char PF1 = 0; */
/* unsigned char PF2 = 0; */

#define  PF0 (memmap->tia_write[TIA_WRITE_PF0])
#define  PF1 (memmap->tia_write[TIA_WRITE_PF1])
#define  PF2 (memmap->tia_write[TIA_WRITE_PF2])
#define CTRLPF (memmap->tia_write[TIA_WRITE_CTRLPF])
#define TIA_CTRLPF_REFLECT_MASK (1 << 0)
#define TIA_CTRLPF_SCORE_MASK (1 << 1)
#define TIA_CTRLPF_PRIORITY_MASK (1 << 2)
#define TIA_CTRLPF_BALL_SIZE_MASK 0x30
#define TIA_COLOR_MASK 0xF0
#define TIA_LUM_MASK 0x0E


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

PlayField *playField;

static
int
TiaParsePlayField()
{

  playField->pf_color = (memmap->tia_write[TIA_WRITE_COLUPF] & TIA_COLOR_MASK) >> 4;
  playField->pf_lum = (memmap->tia_write[TIA_WRITE_COLUPF] & TIA_LUM_MASK) >> 1;

  playField->bk_color = (memmap->tia_write[TIA_WRITE_COLUBK] & TIA_COLOR_MASK) >> 4;
  playField->bk_lum = (memmap->tia_write[TIA_WRITE_COLUBK] & TIA_LUM_MASK) >> 1;
    
  switch(CTRLPF & TIA_CTRLPF_BALL_SIZE_MASK){
  case 0x00:
    playField->ball_size = 1;
    break;
  case 0x10:
    playField->ball_size = 2;
    break;
  case 0x20:
    playField->ball_size = 4;
    break;
  case 0x30:
    playField->ball_size = 8;
    break;
  default:
    assert(0);
    break;
  }

  playField->reflect = CTRLPF & TIA_CTRLPF_REFLECT_MASK;
  playField->score = CTRLPF & TIA_CTRLPF_SCORE_MASK;
  playField->priority = CTRLPF & TIA_CTRLPF_PRIORITY_MASK;
  
  return 0;
}


int
TiaPlayField(int row, int column){
  ColorPalette bk_color = {0,0,0};
  ColorPalette pf_color = {0,0,0};
  Uint32 bk_pixel = 0;
  Uint32 pf_pixel = 0;
  uint row_adj = 0;
  uint col_adj = 0;
  uint col_pixel = 0;
  bool hasPf = false;
  
  // Are we in the printable area and which PF register to use?
  if ( row < VERTICAL_TIMING || row >= VERTICAL_TIMING + FRAME_LINES || column < HORIZONTAL_BLANK)
    return 0;
  
  TiaParsePlayField();

  bk_color = palette[playField->bk_lum][playField->bk_color];
  bk_pixel = StellaCreatePixel(0x00, bk_color.red, bk_color.green, bk_color.blue);

  // TODO: check score, color may be based on player colors 
  pf_color = palette[playField->pf_lum][playField->pf_color];
  pf_pixel = StellaCreatePixel(0x00, pf_color.red, pf_color.green, pf_color.blue);

  
  row_adj = row - VERTICAL_TIMING;
  col_adj = column - HORIZONTAL_BLANK;

  col_pixel = col_adj / 4;

  switch(col_pixel){
  case 0:
    hasPf = (PF0 & 1 << 4);
    break;
  case 1:
    hasPf = (PF0 & 1 << 5);
    break;
  case 2:
    hasPf = (PF0 & 1 << 6);
    break;
  case 3:
    hasPf = (PF0 & 1 << 7);
    break;
  case 4:
    hasPf = (PF1 & 1 << 7);
    break;
  case 5:
    hasPf = (PF1 & 1 << 6);
    break;
  case 6:
    hasPf = (PF1 & 1 << 5);
    break;
  case 7:
    hasPf = (PF1 & 1 << 4);
    break;
  case 8:
    hasPf = (PF1 & 1 << 3);
    break;
  case 9:
    hasPf = (PF1 & 1 << 2);
    break;
  case 10:
    hasPf = (PF1 & 1 << 1);
    break;
  case 11:
    hasPf = (PF1 & 1 << 0);
    break;
  case 12:
    hasPf = (PF2 & 1 << 0);
    break;
  case 13:
    hasPf = (PF2 & 1 << 1);
    break;
  case 14:
    hasPf = (PF2 & 1 << 2);
    break;
  case 15:
    hasPf = (PF2 & 1 << 3);
    break;
  case 16:
    hasPf = (PF2 & 1 << 4);
    break;
  case 17:
    hasPf = (PF2 & 1 << 5);
    break;
  case 18:
    hasPf = (PF2 & 1 << 6);
    break;
  case 19:
    hasPf = (PF2 & 1 << 7);
    break;
  case 20:
    if(playField->reflect)
      hasPf = (PF2 & 1 << 7);
    else
      hasPf = (PF0 & 1 << 4);
    break;
  case 21:
    if(playField->reflect)
      hasPf = (PF2 & 1 << 6);
    else
      hasPf = (PF0 & 1 << 5);
    break;
  case 22:
    if(playField->reflect)
      hasPf = (PF2 & 1 << 5);
    else
      hasPf = (PF0 & 1 << 6);
    break;
  case 23:
    if(playField->reflect)
      hasPf = (PF2 & 1 << 4);
    else
      hasPf = (PF0 & 1 << 7);
    break;
  case 24:
    if(playField->reflect)
      hasPf = (PF2 & 1 << 3);
    else
      hasPf = (PF1 & 1 << 7);
    break;
  case 25:
    if(playField->reflect)
      hasPf = (PF2 & 1 << 2);
    else
      hasPf = (PF1 & 1 << 6);
    break;
  case 26:
    if(playField->reflect)
      hasPf = (PF2 & 1 << 1);
    else
      hasPf = (PF1 & 1 << 5);
    break;
  case 27:
    if(playField->reflect)
      hasPf = (PF2 & 1 << 0);
    else
      hasPf = (PF1 & 1 << 4);
    break;
  case 28:
    if(playField->reflect)
      hasPf = (PF1 & 1 << 0);
    else
      hasPf = (PF1 & 1 << 3);
    break;
  case 29:
    if(playField->reflect)
      hasPf = (PF1 & 1 << 1);
    else
      hasPf = (PF1 & 1 << 2);
    break;
  case 30:
    if(playField->reflect)
      hasPf = (PF1 & 1 << 2);
    else
      hasPf = (PF1 & 1 << 1);
    break;
  case 31:
    if(playField->reflect)
      hasPf = (PF1 & 1 << 3);
    else
      hasPf = (PF1 & 1 << 0);
    break;
  case 32:
    if(playField->reflect)
      hasPf = (PF1 & 1 << 4);
    else
      hasPf = (PF2 & 1 << 0);
    break;
  case 33:
    if(playField->reflect)
      hasPf = (PF1 & 1 << 5);
    else
      hasPf = (PF2 & 1 << 1);
    break;
  case 34:
    if(playField->reflect)
      hasPf = (PF1 & 1 << 6);
    else
      hasPf = (PF2 & 1 << 2);
    break;
  case 35:
    if(playField->reflect)
      hasPf = (PF1 & 1 << 7);
    else
      hasPf = (PF2 & 1 << 3);
    break;
  case 36:
    if(playField->reflect)
      hasPf = (PF0 & 1 << 7);
    else
      hasPf = (PF2 & 1 << 4);
    break;
  case 37:
    if(playField->reflect)
      hasPf = (PF0 & 1 << 6);
    else
      hasPf = (PF2 & 1 << 5);
    break;
  case 38:
    if(playField->reflect)
      hasPf = (PF0 & 1 << 5);
    else
      hasPf = (PF2 & 1 << 6);
    break;
  case 39:
    if(playField->reflect)
      hasPf = (PF0 & 1 << 4);
    else
      hasPf = (PF2 & 1 << 7);
    break;
  default:
    assert(0);
    break;
  }
  
  framebuffer[col_adj][row_adj] = hasPf?pf_pixel:bk_pixel;

  //  assert( framebuffer[75][ 192/2] == 0);
  
  return 0;
}

int
TiaPlayField2(int row, int column){
  int bit = 0;
  unsigned char mask = 0;
  ColorPalette bk_color = {0,0,0};
  ColorPalette pf_color = {0,0,0};
  Uint32 bk_pixel = 0;
  Uint32 pf_pixel = 0;
  
  // Are we in the printable area and which PF register to use?
  if ( row < VERTICAL_TIMING || row >= VERTICAL_TIMING + FRAME_LINES || column < HORIZONTAL_BLANK)
    return 0;
  
  TiaParsePlayField();

  bk_color = palette[playField->bk_lum][playField->bk_color];
  bk_pixel = StellaCreatePixel(0x00, bk_color.red, bk_color.green, bk_color.blue);
  pf_color = palette[playField->pf_lum][playField->pf_color];
  pf_pixel = StellaCreatePixel(0x00, pf_color.red, pf_color.green, pf_color.blue);

  unsigned char pf0 = PF0;
  unsigned char pf1 = PF1;
  unsigned char pf2 = PF2;

  if( column == CLOCK_COUNTS - 1)
    printf("%x%x%x\n", pf0, pf1, pf2);
    
  if( column >= HORIZONTAL_BLANK && column < HORIZONTAL_BLANK + 16 ){

    // Calculate the bit.
    bit = HORIZONTAL_BLANK + 16 - 1 - column;

    // Mask the bit, only left four bits used
    mask = 1 << (4 + bit / 4);
    if ( PF0 & mask ){
      //printf("PF0 %#x, bit %d, %#x\n", PF0, bit, (PF0 & mask));
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = pf_pixel;

    }
    else{
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = bk_pixel;
    }
  } else if( column >= HORIZONTAL_BLANK + 16 &&
	     column < HORIZONTAL_BLANK + 16 + 32){

    // Calculate the bit.
    bit = HORIZONTAL_BLANK + 16 + 32 - 1 - column;

    // Mask the bit
    mask = 1 << (bit / 8);
    if ( PF1 & mask ){
      //      printf("PF1 %#x, bit %d, %#x\n", PF1, bit, (PF1 & mask));

      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = pf_pixel;
    }
    else{
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = bk_pixel;
    }
  } else if( column >= HORIZONTAL_BLANK + 16 + 32 &&
	     column < HORIZONTAL_BLANK + 16 + 32 + 32 ){
    // Calculate the bit.
    bit = HORIZONTAL_BLANK + 16 + 32 + 32 - 1 - column;
    
    // Mask the bit
    mask = 1 << (bit / 8 );
    if ( PF2 & mask ){
      //      printf("PF2 %#x, bit %d, %#x\n", PF2, bit, (PF2 & mask));

      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = pf_pixel;
    }
    else{
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = bk_pixel;
    }
  } else if ( column >= HORIZONTAL_BLANK + 16 + 32 + 32 ) { 

    // TODO: update second half per registers
    if(playField->reflect){
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] =
  	framebuffer[HORIZONTAL_BLANK +  (FRAME_CLOCK_COUNTS / 2) -
  		     ( column - HORIZONTAL_BLANK + FRAME_CLOCK_COUNTS / 2 )][row - VERTICAL_TIMING];
    } else {
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] =
  	framebuffer[column - HORIZONTAL_BLANK - FRAME_CLOCK_COUNTS / 2][row - VERTICAL_TIMING];
    }
    
  }
  /*
  else if( column >= RIGHT_FRAME && column < RIGHT_FRAME + 16 ){
    if(!playField->reflect){
    
      // Calculate the bit.
      bit = HORIZONTAL_BLANK + 16 - 1 - column;
      
      // Mask the bit, only left four bits used
      mask = 1 << (4 + bit / 4);
      if ( PF0 & mask ){
	//printf("PF0 %#x, bit %d, %#x\n", PF0, bit, (PF0 & mask));
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = pf_pixel;
      
      }
      else{
	framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = bk_pixel;
      }
    } else {


    }
  } else if( column >= HORIZONTAL_BLANK + 16 &&
	     column < HORIZONTAL_BLANK + 16 + 32){

    // Calculate the bit.
    bit = HORIZONTAL_BLANK + 16 + 32 - 1 - column;

    // Mask the bit
    mask = 1 << (bit / 8);
    if ( PF1 & mask ){
      //      printf("PF1 %#x, bit %d, %#x\n", PF1, bit, (PF1 & mask));

      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = pf_pixel;
    }
    else{
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = bk_pixel;
    }
  } else if( column >= HORIZONTAL_BLANK + 16 + 32 &&
	     column < HORIZONTAL_BLANK + 16 + 32 + 32 ){
    // Calculate the bit.
    bit = HORIZONTAL_BLANK + 16 + 32 + 32 - 1 - column;
    
    // Mask the bit
    mask = 1 << (bit / 8);
    if ( PF2 & mask ){
      //      printf("PF2 %#x, bit %d, %#x\n", PF2, bit, (PF2 & mask));

      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = pf_pixel;
    }
    else{
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = bk_pixel;
    }
  } else if ( column >= HORIZONTAL_BLANK + 16 + 32 + 32 ) {

    // TODO: update second half per registers
    if(playField->reflect){
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] =
	framebuffer[HORIZONTAL_BLANK +  (FRAME_CLOCK_COUNTS / 2) -
		     ( column - HORIZONTAL_BLANK + FRAME_CLOCK_COUNTS / 2 )][row - VERTICAL_TIMING];
    } else {
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] =
	framebuffer[column - HORIZONTAL_BLANK - FRAME_CLOCK_COUNTS / 2][row - VERTICAL_TIMING];
    }
    
  }
  */
  
  /* if( column > HORIZONTAL_BLANK) */
  /*   assert(framebuffer[column - HORIZONTAL_BLANK -1][row - VERTICAL_TIMING] == framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING]); */
  return 1;
}

int TiaReadRegs()
{
  /* static int vsync = 0; */
  /* static int vblank = 0; */

  // strobe registers don't change value
  /* if(memmap->tia_write[TIA_WRITE_WSYNC]) */
  /*   { */
  /*     Wsync = true; */
  /*     // strobe reg */
  /*     memmap->tia_write[TIA_WRITE_WSYNC] = 0; */
  /*   } */
  
  if(memmap->tia_write[TIA_WRITE_VSYNC] & (1<<1))
    {
      tia->row = VERTICAL_SYNC - 1;
    }
  if(memmap->tia_write[TIA_WRITE_VBLANK] & (1<<1))
    {
      //printf("VBLANK\n");
      tia->row = 0;
    }

  return 0;
}

  /* if(memmap->tia_write[TIA_WRITE_VSYNC] & (1<<1))   { */
  /*   vsync = 1; */
  /* } else if (vsync == 1) { */
  /*   row = VERTICAL_SYNC; */
  /*   vsync = 0; */
  /* } */
  /* if(memmap->tia_write[TIA_WRITE_VBLANK] & (1<<1)) */
  /*   { */
  /*     vblank = 1; */
  /*     row = 0; */
  /*   } else if (vblank == 1){ */
  /*   vblank = 0; */
  /* } */


int
TiaCycle()
{
  // read all the registers!
  TiaReadRegs();
  TiaPlayField(tia->row, tia->column);
 
  tia->column = (tia->column + 1) % CLOCK_COUNTS;
  if(tia->column == 0) {
    Wsync = false;
    tia->row = (tia->row + 1) % SCAN_LINES;
  }
    
  return tia->column * tia->row;
}

int
TiaCreate()
{
  tia = calloc(1, sizeof(Tia));
  assert(tia);
  playField = calloc(1, sizeof(PlayField));

  assert(playField);
  memset(&framebuffer, 0, sizeof(int) * FRAME_CLOCK_COUNTS * FRAME_LINES);
  return 0;
}



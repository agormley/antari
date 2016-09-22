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


int row = 0;
int column = 0;
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
  int bit = 0;
  unsigned char mask = 0;
  ColorPalette bk_color = {0,0,0};
  ColorPalette pf_color = {0,0,0};
  Uint32 bk_pixel = 0;
  Uint32 pf_pixel = 0;
  
  // Are we in the printable area and which PF register to use?
  if ( row < VERTICAL_TIMING || row >= VERTICAL_TIMING + FRAME_LINES)
    return 0;
  
  TiaParsePlayField();

  bk_color = palette[playField->bk_lum][playField->bk_color];
  bk_pixel = StellaCreatePixel(0x00, bk_color.red, bk_color.green, bk_color.blue);
  pf_color = palette[playField->pf_lum][playField->pf_color];
  pf_pixel = StellaCreatePixel(0x00, pf_color.red, pf_color.green, pf_color.blue);

  
  if( column >= HORIZONTAL_BLANK && column < HORIZONTAL_BLANK + 16 ){

    // Calculate the bit.
    bit = HORIZONTAL_BLANK + 16 - 1 - column;

    // Mask the bit, only left four bits used
    mask = 1 << (4 + bit);
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
    mask = 1 << (bit);
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
    mask = 1 << (bit);
    if ( PF2 & mask ){
      //      printf("PF2 %#x, bit %d, %#x\n", PF2, bit, (PF2 & mask));

      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = pf_pixel;
    }
    else{
      framebuffer[column - HORIZONTAL_BLANK][row - VERTICAL_TIMING] = bk_pixel;
    }
  }

  return 1;
}

int TiaReadRegs()
{


  if(memmap->tia_write[TIA_WRITE_VSYNC])
    {
      Wsync = true;
      // Does this clear on read?
      // memmap->tia_write[TIA_WRITE_VSYNC] = 0;
    }
  if(memmap->tia_write[TIA_WRITE_VSYNC])
    {
      printf("VSYNC\n");
    }
  if(memmap->tia_write[TIA_WRITE_VBLANK])
    {
      printf("VBLAND\n");
    }

  return 0;
}

int
TiaCycle()
{
  // read all the registers!
  TiaReadRegs();
  TiaPlayField(row, column);


  //  drawFramebuffer();
  
  column = (column + 1) % CLOCK_COUNTS;
  if(column == 0) {
    Wsync = false;
    row = (row + 1) % SCAN_LINES;
  }
    
  return column * row;
}

int
TiaCreate()
{
  playField = calloc(1, sizeof(PlayField));

  assert(playField);
  memset(&framebuffer, 0, sizeof(int) * FRAME_CLOCK_COUNTS * FRAME_LINES);
  return 0;
}



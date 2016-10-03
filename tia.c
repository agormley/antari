#include "includes.h"

//Uint32 framebuffer[FRAME_CLOCK_COUNTS][FRAME_LINES];

Uint32 framebuffer[FRAME_LINES][FRAME_CLOCK_COUNTS];


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


Sprite *player0;
Sprite *player1;
PlayField *playField;

static
int
TiaParsePlayField()
{

  player0->color = (memmap->tia_write[TIA_WRITE_COLUP0] & TIA_COLOR_MASK) >> 4;
  player0->lum = (memmap->tia_write[TIA_WRITE_COLUP0] & TIA_LUM_MASK) >> 1;
  player1->color = (memmap->tia_write[TIA_WRITE_COLUP1] & TIA_COLOR_MASK) >> 4;
  player1->lum = (memmap->tia_write[TIA_WRITE_COLUP1] & TIA_LUM_MASK) >> 1;
  
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
getSpritePixels(int row,
		int column,
		bool *hasP0,
		bool *hasP1,
		bool *hasM0,
		bool *hasM1,
		bool *hasB,
		Uint32 *p0Pixel,
		Uint32 *p1Pixel,
		Uint32 *m0Pixel,
		Uint32 *m1Pixel,
		Uint32 *bPixel)
{
  Uint32 p0_pixel = 0;
  Uint32 p1_pixel = 0;

  ColorPalette p0_color = {0,0,0};
  ColorPalette p1_color = {0,0,0};

  p0_color = palette[player0->lum][player0->color];
  p0_pixel = StellaCreatePixel(0x00, p0_color.red, p0_color.green, p0_color.blue);

  p1_color = palette[player1->lum][player1->color];
  p1_pixel = StellaCreatePixel(0x00, p1_color.red, p1_color.green, p1_color.blue);

  // player 0
  if(ResetPlayer0){
    player0->pixBit = 0;
    ResetPlayer0 = false;
  }

  if (player0->pixBit < 0 ) {
    *hasP0 = false;
    // could set the alpha?
    *p0Pixel = 0;
  } else {
    // get p0 pixel!
    if ( memmap->tia_write[TIA_WRITE_GRP0] &
	 (1 << (7 - player0->pixBit))){
      *hasP0 = true;
      // could set the alpha?
      *p0Pixel = p0_pixel;
    
    }
    else {
      *hasP0 = false;
      // could set the alpha?
      *p0Pixel = 0;
      
    }
    player0->pixBit=player0->pixBit==7?-1:player0->pixBit+1;

  }

  // player 1
  if(ResetPlayer1){
    player1->pixBit = 0;
    ResetPlayer1 = false;

  }
  
  if (player1->pixBit < 0 ) {
    *hasP1 = false;
    // could set the alpha?
    *p1Pixel = 0;
  } else {
    // get p1 pixel!
    if ( memmap->tia_write[TIA_WRITE_GRP1] &
	 (1 << (7 - player1->pixBit))){
      *hasP1 = true;
      // could set the alpha?
      *p1Pixel = p1_pixel;
      
    }
    else {
      *hasP1 = false;
      // could set the alpha?
      *p1Pixel = 0;
      
    }
    player1->pixBit=player1->pixBit==7?-1:player1->pixBit+1;

  }

  *hasM0 = false;
  *hasM1 = false;
  *hasB = false;
    
  *m0Pixel = 0;
  *m1Pixel = 0;
  *bPixel = 0;
  
  return 0;
}
				
		

int
TiaPlayField(int row, int column){
  ColorPalette bk_color = {0,0,0};
  ColorPalette pf_color = {0,0,0};
  
  Uint32 bk_pixel = 0;
  Uint32 pf_pixel = 0;
  Uint32 p0_pixel = 0;
  Uint32 p1_pixel = 0;
  Uint32 m0_pixel = 0;
  Uint32 m1_pixel = 0;
  Uint32 b_pixel = 0;

  uint row_adj = 0;
  uint col_adj = 0;
  uint col_pixel = 0;
  bool hasPf = false;
  bool hasP0 = false;
  bool hasP1 = false;
  bool hasM0 = false;
  bool hasM1 = false;
  bool hasB = false;
  
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

  getSpritePixels(row_adj,
		  col_adj,
		  &hasP0,
		  &hasP1,
		  &hasM0,
		  &hasM1,
		  &hasB,
		  &p0_pixel,
		  &p1_pixel,
		  &m0_pixel,
		  &m1_pixel,
		  &b_pixel);
		  

  // PLEASE MOVE THIS INTO ITS OWN FUNCTION!
  
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
  
  framebuffer[row_adj][col_adj] = hasP0?p0_pixel:hasP1?p1_pixel:
    hasPf?pf_pixel:bk_pixel;

  //  assert( framebuffer[ 192/2][75] == 0);
  
  return 0;
}

int TiaReadRegs()
{
  
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


  player0 = calloc(1, sizeof(Sprite));
  assert(player0);
  player0->pixBit = -1;
  
  player1 = calloc(1, sizeof(Sprite));
  assert(player1);
  player1->pixBit = -1;
  return 0;
}



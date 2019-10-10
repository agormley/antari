/***
 * TIA is the Television Interface Adapter which is the 
 * graphics and sound processor. 
 */

#include "includes.h"

Uint32 framebuffer[FRAME_LINES][FRAME_CLOCK_COUNTS];

int
TiaConvertHmToInt(BYTE hm)
{

  switch(hm & 0xF0) {
  case 0x00:
    return 0;
  case 0x10:
    return 1;
  case 0x20:
    return 2;
  case 0x30:
    return 3;
  case 0x40:
    return 4;
  case 0x50:
    return 5;
  case 0x60:
    return 6;
  case 0x70:
    return 7;

  case 0x80:
    return -8;
  case 0x90:
    return -7;
  case 0xA0:
    return -6;
  case 0xB0:
    return -5;
  case 0xC0:
    return -4;
  case 0xD0:
    return -3;
  case 0xE0:
    return -2;
  case 0xF0:
    return -1;
  }

  return 0;
}

void
updatePlayerSpace(Sprite* sp) {
  sp->spaceCount = sp->space * 8;
}

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


int missile0Pos;
int missile1Pos;

PlayField *playField;

static
int
TiaParsePlayField(bool isLeft)
{

  tia->player0->color = (memmap->tia_write[TIA_WRITE_COLUP0] & TIA_COLOR_MASK) >> 4;
  tia->player0->lum = (memmap->tia_write[TIA_WRITE_COLUP0] & TIA_LUM_MASK) >> 1;
  tia->player1->color = (memmap->tia_write[TIA_WRITE_COLUP1] & TIA_COLOR_MASK) >> 4;
  tia->player1->lum = (memmap->tia_write[TIA_WRITE_COLUP1] & TIA_LUM_MASK) >> 1;

  playField->bk_color = (memmap->tia_write[TIA_WRITE_COLUBK] & TIA_COLOR_MASK) >> 4;
  playField->bk_lum = (memmap->tia_write[TIA_WRITE_COLUBK] & TIA_LUM_MASK) >> 1;
    
  switch(CTRLPF & TIA_CTRLPF_BALL_SIZE_MASK){
  case 0x00:
    playField->ball_size = 1;
    tia->ball->width = 1;
    break;
  case 0x10:
    playField->ball_size = 2;
    tia->ball->width = 2;

    break;
  case 0x20:
    playField->ball_size = 4;
    tia->ball->width = 4;

    break;
  case 0x30:
    playField->ball_size = 8;
    tia->ball->width = 8;

    break;
  default:
    assert(0);
    break;
  }

  playField->reflect = CTRLPF & TIA_CTRLPF_REFLECT_MASK;
  playField->score = CTRLPF & TIA_CTRLPF_SCORE_MASK;
  playField->priority = CTRLPF & TIA_CTRLPF_PRIORITY_MASK;
  if (playField->score) {
      if (isLeft) {
          playField->pf_color = tia->player0->color;
          playField->pf_lum = tia->player0->lum;
      } else {
          playField->pf_color = tia->player1->color;
          playField->pf_lum = tia->player1->lum;
      }
      
  } else {
      playField->pf_color = (memmap->tia_write[TIA_WRITE_COLUPF] & TIA_COLOR_MASK) >> 4;
      playField->pf_lum = (memmap->tia_write[TIA_WRITE_COLUPF] & TIA_LUM_MASK) >> 1;
  }
  
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
  Uint32 ball_pixel = 0;
  Uint32 m0_pixel = 0;
  Uint32 m1_pixel = 0;

  ColorPalette p0_color = {0,0,0};
  ColorPalette p1_color = {0,0,0};
  ColorPalette ball_color = {0,0,0};
  Sprite *p0 = tia->player0;
  Sprite *p1 = tia->player1;

  p0_color = palette[p0->lum][p0->color];
  p0_pixel = StellaCreatePixel(0x00, p0_color.red, p0_color.green, p0_color.blue);

  p1_color = palette[p1->lum][p1->color];
  p1_pixel = StellaCreatePixel(0x00, p1_color.red, p1_color.green, p1_color.blue);

  m0_pixel = StellaCreatePixel(0x00, p0_color.red, p0_color.green, p0_color.blue);
  
  m1_pixel = StellaCreatePixel(0x00, p1_color.red, p1_color.green, p1_color.blue);
  
  ball_color = palette[playField->pf_lum][playField->pf_color];
  ball_pixel = StellaCreatePixel(0x00, ball_color.red, ball_color.green, ball_color.blue);
  
  // is this the right place for horizontal motion?
  if(tia->hMotionPending){
    tia->hMotionPending = false;

    p0->clkStart = p0->clkStart - p0->hMotion;
    // printf("p0 clkStart %d, hMotion %d\n", p0->clkStart, p0->hMotion);
    /* if(p0->clkStart < 0) */
    /*   p0->clkStart = 0; */

    p1->clkStart -= p1->hMotion;
    /* if(p1->clkStart < 0) */
    /*   p1->clkStart = 0; */

    missile0Pos = tia->missile0->clkStart - tia->missile0->hMotion;
    // if(missile0Pos < 0)
    //  missile0Pos = 0;

    missile1Pos = tia->missile1->clkStart - tia->missile1->hMotion;
    //if(missile1Pos < 0)
    //  missile1Pos = 0;

    tia->ball->clkStart -= tia->ball->hMotion;
    //if(tia->ball->clkStart < 0)
    //  tia->ball->clkStart = 0;
  }
  else {
      p0->adjClkStart = p0->clkStart;
  }
  //printf("sprite update\n");
  // player 0
  if(p0->reset) {
    p0->reset = false;
  }
  else {
      bool p0Enable = p0->copies > 1 &&
          p0->copyCount < p0->copies &&
          p0->copyCount >= 0 &&
          p0->spaceCount == 0 &&
          column >= p0->adjClkStart;

    printf("column %d, enable? %s copyCount %d, copies %d, spaceCount %d\n",
           column,
           p0Enable?"true":"false",
           p0->copyCount,
          p0->copies,
         p0->spaceCount);
      if((p0->adjClkStart == column &&
       !p0->delayed)||
        p0Enable
            ){
      p0->pixBit = 0;
      p0->spaceCount = -1;
      p0->adjClkStart = column;
    }

    if (p0->pixBit < 0 ) {
      if (p0->copyCount > 0 && p0->spaceCount > 0) {
        p0->spaceCount--;
        
      }

      
      *hasP0 = false;
      // could set the alpha?
      *p0Pixel = 0;
    } else {
      // get p0 pixel!
	if(p0->reflect){
	    if ( memmap->tia_write[TIA_WRITE_GRP0] &
		 (1 << (p0->pixBit / p0->width))){
		*hasP0 = true;
		// could set the alpha?
		*p0Pixel = p0_pixel;
		
	    }	else {
		*hasP0 = false;
		// could set the alpha?
		*p0Pixel = 0;
	    }
	    
	}
	else{
	    if ( memmap->tia_write[TIA_WRITE_GRP0] &
		 (1 << (7 - p0->pixBit / p0->width))){
		*hasP0 = true;
		// could set the alpha?
		*p0Pixel = p0_pixel;
		
	    } else {
		*hasP0 = false;
		// could set the alpha?
		*p0Pixel = 0;
	    }
	}
        // Finish p0 sprite
        if ((p0->pixBit + 1)/p0->width > 7) {
            
            updatePlayerSpace(p0);
            p0->copyCount--;
            p0->pixBit = -1;
        } else {
            //printf("pixBit %d\n", p0->pixBit);
            p0->pixBit = p0->pixBit+1;
        }
        
    }
  }

  // player 1
  if(p1->reset){
      p1->reset = false;
      
  } else {
      bool p1Enable = p1->copies > 1 &&
          p1->copyCount < p1->copies &&
          p1->copyCount > 0 &&
          p1->spaceCount == 0 &&
          column >= p1->adjClkStart;
    printf("column %d, p1 enable? %s copyCount %d, copies %d, spaceCount %d\n",
           column,
           p1Enable?"true":"false",
           p0->copyCount,
           p0->copies,
           p0->spaceCount);
    
      if((p1->clkStart == column &&
          !p1->delayed) || 
          p1Enable){ // or matches 
	  p1->pixBit = 0;
          p1->spaceCount = -1;

          
	  p1->clkStart = column;
      }
      
      if (p1->pixBit < 0 ) {
          if (p1->copyCount > 0 && p1->spaceCount > 0) {
              p1->spaceCount--;
          }

 
          *hasP1 = false;
	  // could set the alpha?
	  *p1Pixel = 0;
      } else {
	  if(p1->reflect &&
	     memmap->tia_write[TIA_WRITE_GRP1] &
	     (1 << (p1->pixBit / p1->width))){
	      *hasP1 = true;
	      // could set the alpha?
	      *p1Pixel = p1_pixel;
	      
	  } else if(!p1->reflect &&
		    memmap->tia_write[TIA_WRITE_GRP1] &
		    (1 << (7 - p1->pixBit / p1->width))){
	      *hasP1 = true;
	      // could set the alpha?
	      *p1Pixel = p1_pixel;
	  }
	  else {
	      *hasP1 = false;
	      // could set the alpha?
	      *p1Pixel = 0;
	      
	  }
	  
	  if ((p1->pixBit+1)/p1->width>7) {
              updatePlayerSpace(p1);
              p1->copyCount--;
              p1->pixBit = -1;
          } else {
              p1->pixBit = p1->pixBit+1;
          }
      }
  }

  // ball
  if(tia->ball->reset){
      tia->ball->reset = false;

  } else if (tia->ball->enabled) {
    if(tia->ball->clkStart == column){
      tia->ball->pixBit = 0;
    }
  
    if (tia->ball->pixBit != -1 &&
	tia->ball->pixBit < tia->ball->width &&
	tia->ball->clkStart <= column){
	*hasB = true;
	// could set the alpha?
	*bPixel = ball_pixel;
	tia->ball->pixBit++;      
    }
    else {
	*hasB = false;
	// could set the alpha?
	*bPixel = 0;
	tia->ball->pixBit = -1;      
    }
  }

  // missiles
  
  // missile 0
  if(tia->missile0->reset) {
    tia->missile0->reset = false;
  
  }
  else if(tia->missile0->enabled) {
    if( missile0Pos == column){
      tia->missile0->pixBit = 0;
      //tia->missile0->clkStart = column;
    }

    if (tia->missile0->pixBit < 0 ) {
      *hasM0 = false;
      // could set the alpha?
      *m0Pixel = 0;
    } else {
      // get p0 pixel!
      if (tia->missile0->enabled ){
	*hasM0 = true;
	// could set the alpha?
	*m0Pixel = m0_pixel;
    
      }
      else {
	*hasM0 = false;
	// could set the alpha?
	*m0Pixel = 0;
      
      }
      tia->missile0->pixBit=tia->missile0->pixBit==7?-1:
	tia->missile0->pixBit+1;

    }
  }


  // LOOK TO problemkaputt.de for good explanation on delay.
  /*

    Basically, don't a write to a register doesn't happen until a write to a corresponding second register happens.
    25h - VDELP0 - Vertical delay player 0 (Delay GRP0 until writing to GRP1)
    26h - VDELP1 - Vertical delay player 1 (Delay GRP1 until writing to GRP0)
    27h - VDELBL - Vertical delay ball (Delay ENABL until writing to GRP1)
    When VDELPx is set, writes to GRPx are delayed until GRPy is written to - which may be done in the same scanline, in one of the next scanlines, or in the next frame, or never - and may thus actually cause a horizontal, vertical, framerate, or even endless delay.

    Bit  Expl.
    0    Vertical Delay  (0=No delay, 1=Delay until writing to GRP0/GRP1)
    1-7  Not used

When VDELPx is set, writes to GRPx are delayed until GRPy is written to - which may be done in the same scanline, in one of the next scanlines, or in the next frame, or never - and may thus actually cause a horizontal, vertical, framerate, or even endless delay.
  Bit  Expl.
  0    Vertical Delay  (0=No delay, 1=Delay until writing to GRP0/GRP1)
  1-7  Not used
The misleading name "vertical" delay is used because these registers were originally intended to be used to update GRPx in one scanline and to have the changes delayed until GRPy is updated in the next scanline.

Notes on High Resolution Text and Vertical Delay
Many newer games (eg. Pacman) use high resolution text or graphics output, of 6 characters or 48 pixels width. This is done by configuring NUSIZ to "three copies close", and positioning GRP0 ("0_0_0") eight pixels to the left of GRP1 ("1_1_1"), resulting in the pattern "010101".
Updating GRP0 and GRP1 multiple times within each scanline can be done to change the pattern to 6 different characters ("012345"), the drawing procedure requires proper timing, and typically uses "vertical" delay (in this case behaving as horizontal delay), note that one usually needs 7 writes to GRP registers to draw 6 characters (with one final dummy write to apply the last (delayed) character).
The GRPx registers actually consist of two registers each, hereby called GRPxA (delayed data latch) and GRPxB (actual display data). When VDELx is disabled, writes to GRPx go directly to GRPxB. When VDELx is enabled, writes to GRPx go to GRPxA, and GRPxA is copied to GRPxB at the time when writing to GRPy.

   */  
  // missile 1
  if(tia->missile1->reset) {
    tia->missile1->reset = false;
  
  }
  else  if (tia->missile1->enabled) {
    if(missile1Pos == column){
      tia->missile1->pixBit = 0;
      //tia->missile1->clkStart = column;
    }
  
    if (tia->missile1->pixBit < 0 ) {
      *hasM1 = false;
      // could set the alpha?
      *m1Pixel = 0;
    } else {
      // get p1 pixel!
      if ( tia->missile1->enabled ){
	*hasM1 = true;
	// could set the alpha?
	*m1Pixel = m1_pixel;
      
      }
      else {
	*hasM1 = false;
	// could set the alpha?
	*m1Pixel = 0;
      
      }
      tia->missile1->pixBit=tia->missile1->pixBit==7?-1:tia->missile1->pixBit+1;
    }
  }


  
  *hasM0 = false;
  *hasM1 = false;
    
  *m0Pixel = 0;
  *m1Pixel = 0;
  
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
  bool isLeft = true;
  if( column - HORIZONTAL_BLANK > (4 + 8 + 8) * 4) {
      isLeft = false;
  } 
  
  TiaParsePlayField(isLeft);
 
  bk_color = palette[playField->bk_lum][playField->bk_color];
  bk_pixel = StellaCreatePixel(0x00, bk_color.red, bk_color.green, bk_color.blue);

  // TODO: check score, color may be based on player colors 
  pf_color = palette[playField->pf_lum][playField->pf_color];
  pf_pixel = StellaCreatePixel(0x00, pf_color.red, pf_color.green, pf_color.blue);

 
  row_adj = row - VERTICAL_TIMING;
  col_adj = column - HORIZONTAL_BLANK;
  //  printf("row: %d, col %d\n", row, column);
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


  // collisions
  MEM_RD(TIA_READ_CXM0P) |= ((hasM0 & hasP1) << 7); 
  MEM_RD(TIA_READ_CXM0P) |= ((hasM0 & hasP0) << 6); 

  MEM_RD(TIA_READ_CXM1P) |= ((hasM1 & hasP0) << 7); 
  MEM_RD(TIA_READ_CXM1P) |= ((hasM1 & hasP1) << 6); 

  MEM_RD(TIA_READ_CXP0FB) |= ((hasP0 & hasPf) << 7); 
  MEM_RD(TIA_READ_CXP0FB) |= ((hasP0 & hasB) << 6); 

  MEM_RD(TIA_READ_CXP1FB) |= ((hasP1 & hasPf) << 7); 
  MEM_RD(TIA_READ_CXP1FB) |= ((hasP1 & hasB) << 6); 
  
  MEM_RD(TIA_READ_CXM0FB) |= ((hasM0 & hasPf) << 7); 
  MEM_RD(TIA_READ_CXM0FB) |= ((hasM0 & hasB) << 6); 

  MEM_RD(TIA_READ_CXM1FB) |= ((hasM1 & hasPf) << 7); 
  MEM_RD(TIA_READ_CXM1FB) |= ((hasM1 & hasB) << 6); 

  MEM_RD(TIA_READ_CXBLPF) |= ((hasB & hasPf) << 7); 

  MEM_RD(TIA_READ_CXPPMM) |= ((hasP0 & hasP1) << 7); 
  MEM_RD(TIA_READ_CXPPMM) |= ((hasM1 & hasM0) << 6); 

  // fix this. assign based on priority
  framebuffer[row_adj][col_adj] = hasP0?p0_pixel:hasP1?
      p1_pixel:hasB?b_pixel:hasM0?m0_pixel:hasM1?m1_pixel:
      hasPf?pf_pixel:bk_pixel;

  //  assert( framebuffer[ 192/2][75] == 0);
  
  return 0;
}

int TiaClearHMotion(){
  tia->player0->hMotion = 0;
  tia->player1->hMotion = 0;
  tia->missile0->hMotion = 0;
  tia->missile1->hMotion = 0;
  tia->ball->hMotion = 0;
  
  // and the other sprites
  return 0;
}

void
updatePlayerCounts() {
  Sprite* p0 = tia->player0;
  Sprite* p1 = tia->player1;
  p0->copyCount = p0->copies - 1;
  p1->copyCount = p1->copies - 1;
  updatePlayerSpace(p0);
  updatePlayerSpace(p1);
}

void
parsePlayerSize(int reg, Sprite* sp){
  switch (MEM_WR(reg) & 7)
  {
  case ONE_COPY:
      sp->copies = 1;
      sp->width = 1;
      sp->space = 0;
      break;
  case TWO_COPIES_CLOSE:
      sp->copies = 2;
      sp->width = 1;
      sp->space = 1;
      break;
  case TWO_COPIES_MID:
      sp->copies = 2;
      sp->width = 1;
      sp->space = 3;
      break;
  case THREE_COPIES_CLOSE:
      //printf("three copies close\n");
      sp->copies = 3;
      sp->width = 1;
      sp->space = 1;
      break;
  case TWO_COPIES_FAR:
      sp->copies = 2;
      sp->width = 1;
      sp->space = 7;
      break;
  case ONE_COPY_DOUBLE:
      sp->copies = 1;
      sp->width = 2;
      sp->space = 0;
      break;
  case THREE_COPIES_MID:
      printf("three copies mid");
      sp->copies = 3;
      sp->width = 1;
      sp->space = 3;
      break;
  case ONE_COPY_QUAD:
      sp->copies = 1;
      sp->width = 4;
      sp->space = 0;
      break;
  }
}

int TiaReadRegs()
{
  static bool vsync_on = false;
  static bool vblank_on = false;

  tia->missile0->width =
    1 << (memmap->tia_write[TIA_WRITE_NUSIZ0] >> 4 & (0x3));

  tia->missile1->width =
    1 << (memmap->tia_write[TIA_WRITE_NUSIZ1] >> 4 & (0x3));

  parsePlayerSize(TIA_WRITE_NUSIZ0, tia->player0);
  parsePlayerSize(TIA_WRITE_NUSIZ1, tia->player1);
  
  if(memmap->tia_write[TIA_WRITE_ENAM0] & (1<<1)) {
    tia->missile0->enabled = true;
  }
  else{
    tia->missile0->enabled = false;
  }
  
  if(memmap->tia_write[TIA_WRITE_ENAM1] & (1<<1)) {
    tia->missile1->enabled = true;
  }
  else{
    tia->missile1->enabled = false;
  }
  
  if(memmap->tia_write[TIA_WRITE_ENABL] & (1<<1) ||
     tia->ball->reset) {
    tia->ball->enabled = true;
  }
  else{
    tia->ball->enabled = false;
  }

  if(memmap->tia_write[TIA_WRITE_VSYNC] & (1<<1))
    {
      if(!vsync_on)
	LOG("VSYNC ON%s", "");

      vsync_on = true;
      tia->row = 0;
    }
  else if (vsync_on)
    {
      LOG("VSYNC OFF%s", "");
      vsync_on = false;
      tia->row = VERTICAL_SYNC;
    }
  if(memmap->tia_write[TIA_WRITE_VBLANK] & (1<<1))
    {
      if(!vblank_on)
	LOG("VBLANK ON%s", "");
      //printf("VBLANK\n");
      //tia->row = 0;

      vblank_on = true;
    }
  else if (vblank_on)
    {
      vblank_on = false;
      LOG("VBLANK OFF%s", "");
    }

  tia->player0->reflect = memmap->tia_write[TIA_WRITE_REFP0];
  tia->player1->reflect = memmap->tia_write[TIA_WRITE_REFP1];

  tia->player0->hMotion = 
    TiaConvertHmToInt(memmap->tia_write[TIA_WRITE_HMP0]);
  tia->player1->hMotion =
    TiaConvertHmToInt(memmap->tia_write[TIA_WRITE_HMP1]);
  tia->ball->hMotion =
    TiaConvertHmToInt(memmap->tia_write[TIA_WRITE_HMBL]);
  tia->missile0->hMotion =
    TiaConvertHmToInt(memmap->tia_write[TIA_WRITE_HMM0]);
  tia->missile1->hMotion =
    TiaConvertHmToInt(memmap->tia_write[TIA_WRITE_HMM1]);

  return 0;
}

int
TiaCycle()
{
  // read all the registers!
  TiaReadRegs();

  TiaPlayField(tia->row, tia->column);
 
  tia->column = (tia->column + 1) % CLOCK_COUNTS;
  // printf("column: %d\n", tia->column);
  // 30 cycles * 3 + 5 95 - 68 =
  if(tia->column == 0) {
    tia->wsync = false;
    tia->row = (tia->row + 1) % SCAN_LINES;
    updatePlayerCounts();
  }

  tia->colorClocks++;
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


  tia->player0 = calloc(1, sizeof(Sprite));
  assert(tia->player0);
  tia->player0->pixBit = -1;
  tia->player0->width = 1;
  tia->player1 = calloc(1, sizeof(Sprite));
  assert(tia->player1);
  tia->player1->pixBit = -1;
  tia->player1->width = 1;
  tia->ball = calloc(1, sizeof(Sprite));
  assert(tia->ball);
  tia->ball->pixBit = -1;

  tia->missile0 = calloc(1, sizeof(Sprite));
  assert(tia->missile0);
  tia->missile0->pixBit = -1;

  tia->missile1 = calloc(1, sizeof(Sprite));
  assert(tia->missile1);
  tia->missile1->pixBit = -1;

  return 0;
}



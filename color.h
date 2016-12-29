
typedef struct _colorPalette{
  unsigned char red;
  unsigned char green;
  unsigned char blue;
}ColorPalette;

#define PALETTE_COLORS 0x10
// 8 hues but 4 bits.
#define PALETTE_LUMS 0x8 


extern ColorPalette palette[PALETTE_LUMS][PALETTE_COLORS];

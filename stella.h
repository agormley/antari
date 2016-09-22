/* 
  --- stella.txt ---
  Atari 2600      Stella Memory Map

  Write Address Registers
  Addr    Assy Name       Bits Used       Function

  00      Vsync           0000 00x0       Vertical Sync Set-Clear
  01      Vblank          xx00 00x0       Vertical Blank Set-Clear
  02      Wsync           ---- ----       Wait for Horizontal Blank
  03      Rsync           ---- ----       Reset Horizontal Sync Counter
  04      Nusiz0          00xx 0xxx       Number-Size player/missle 0
  05      Nusiz1          00xx 0xxx       Number-Size player/missle 1
  06      Colup0          xxxx xxx0       Color-Luminance Player 0
  07      Colup1          xxxx xxx0       Color-Luminance Player 1
  08      Colupf          xxxx xxx0       Color-Luminance Playfield
  09      Colubk          xxxx xxx0       Color-Luminance Background
  0A      Ctrlpf          00xx 0xxx       Control Playfield, Ball, Collisions
  0B      Refp0           0000 x000       Reflection Player 0
  0C      Refp1           0000 x000       Reflection Player 1
  0D      Pf0             xxxx 0000       Playfield Register Byte 0
  0E      Pf1             xxxx xxxx       Playfield Register Byte 1
  0F      Pf2             xxxx xxxx       Playfield Register Byte 2
  10      Resp0           ---- ----       Reset Player 0
  11      Resp1           ---- ----       Reset Player 1
  12      Resm0           ---- ----       Reset Missle 0
  13      Resm1           ---- ----       Reset Missle 1
  14      Resbl           ---- ----       Reset Ball
  15      Audc0           0000 xxxx       Audio Control 0
  16      Audc1           0000 xxxx       Audio Control 1
  17      Audf0           000x xxxx       Audio Frequency 0
  18      Audf1           000x xxxx       Audio Frequency 1
  19      Audv0           0000 xxxx       Audio Volume 0
  1A      Audv1           0000 xxxx       Audio Volume 1
  1B      Grp0            xxxx xxxx       Graphics Register Player 0
  1C      Grp1            xxxx xxxx       Graphics Register Player 1
  1D      Enam0           0000 00x0       Graphics Enable Missle 0
  1E      Enam1           0000 00x0       Graphics Enable Missle 1
  1F      Enabl           0000 00x0       Graphics Enable Ball
  20      Hmp0            xxxx 0000       Horizontal Motion Player 0
  21      Hmp1            xxxx 0000       Horizontal Motion Player 1
  22      Hmm0            xxxx 0000       Horizontal Motion Missle 0
  23      Hmm1            xxxx 0000       Horizontal Motion Missle 1
  24      Hmbl            xxxx 0000       Horizontal Motion Ball
  25      Vdelp0          0000 000x       Vertical Delay Player 0
  26      Vdelp1          0000 000x       Vertical Delay Player 1
  27      Vdelbl          0000 000x       Vertical Delay Ball
  28      Resmp0          0000 00x0       Reset Missle 0 to Player 0
  29      Resmp1          0000 00x0       Reset Missle 1 to Player 1
  2A      Hmove           ---- ----       Apply Horizontal Motion
  2B      Hmclr           ---- ----       Clear Horizontal Move Registers
  2C      Cxclr           ---- ----       Clear Collision Latches

  Read Address Registers
                                                          bit 6   bit 7
  0       Cxm0p           xx00 0000       Read Collision  M0-P1   M0-P0
  1       Cxm1p           xx00 0000                       M1-P0   M1-P1
  2       Cxp0fb          xx00 0000                       P0-PF   P0-BL
  3       Cxp1fb          xx00 0000                       P1-PF   P1-BL
  4       Cxm0fb          xx00 0000                       M0-PF   M0-BL
  5       Cxm1fb          xx00 0000                       M1-PF   M1-BL
  6       Cxblpf          x000 0000                       BL-PF   -----
  7       Cxppmm          xx00 0000                       P0-P1   M0-M1
  8       Inpt0           x000 0000       Read Pot Port 0
  9       Inpt1           x000 0000       Read Pot Port 1
  A       Inpt2           x000 0000       Read Pot Port 2
  B       Inpt3           x000 0000       Read Pot Port 3
  C       Inpt4           x000 0000       Read Input (Trigger) 0
  D       Inpt5           x000 0000       Read Input (Trigger)
*/

#define STELLA_WRITE_REG_COUNT 0x2D

typedef struct _Stella {
  SDL_Window *window;
  SDL_Texture *texture;
  SDL_Renderer *renderer;
  
  union {
    char write_addr_regs [STELLA_WRITE_REG_COUNT];
    struct {
      char vsync;	// 0000 00x0       Vertical Sync Set-Clear
      char vblank;	// xx00 00x0       Vertical Blank Set-Clear
      char wsync;	// ---- ----       Wait for Horizontal Blank
      char rsync;       // ---- ----       Reset Horizontal Sync Counter
      char nusiz0;	// 00xx 0xxx       Number-Size player/missle 0
      char nusiz1;	// 00xx 0xxx       Number-Size player/missle 1
      char colup0;	// xxxx xxx0       Color-Luminance Player 0
      char colup1;	// xxxx xxx0       Color-Luminance Player 1
      char colupf;	// xxxx xxx0       Color-Luminance Playfield
      char colubk;	// xxxx xxx0       Color-Luminance Background
      char ctrlpf;	// 00xx 0xxx       Control Playfield, Ball, Collisions
      char refp0;	//  0000 x000       Reflection Player 0
      char refp1;	//  0000 x000       Reflection Player 1
      char pf0;		//    xxxx 0000       Playfield Register Byte 0
      char pf1;		//    xxxx xxxx       Playfield Register Byte 1
      char pf2;		//    xxxx xxxx       Playfield Register Byte 2
      char resp0;	//  ---- ----       Reset Player 0
      char resp1;	//  ---- ----       Reset Player 1
      char resm0;	//  ---- ----       Reset Missle 0
      char resm1;	//  ---- ----       Reset Missle 1
      char resbl;	//  ---- ----       Reset Ball
      char audc0;	//  0000 xxxx       Audio Control 0
      char audc1;	//  0000 xxxx       Audio Control 1
      char audf0;	//  000x xxxx       Audio Frequency 0
      char audf1;	//  000x xxxx       Audio Frequency 1
      char audv0;	//  0000 xxxx       Audio Volume 0
      char audv1;	//  0000 xxxx       Audio Volume 1
      char grp0;	//   xxxx xxxx       Graphics Register Player 0
      char grp1;	//   xxxx xxxx       Graphics Register Player 1
      char enam0;	//  0000 00x0       Graphics Enable Missle 0
      char enam1;	//  0000 00x0       Graphics Enable Missle 1
      char enabl;	//  0000 00x0       Graphics Enable Ball
      char hmp0;	//   xxxx 0000       Horizontal Motion Player 0
      char hmp1;	//   xxxx 0000       Horizontal Motion Player 1
      char hmm0;	//   xxxx 0000       Horizontal Motion Missle 0
      char hmm1;	//   xxxx 0000       Horizontal Motion Missle 1
      char hmbl;	//   xxxx 0000       Horizontal Motion Ball
      char vdelp0;	// 0000 000x       Vertical Delay Player 0
      char vdelp1;	// 0000 000x       Vertical Delay Player 1
      char vdelbl;	// 0000 000x       Vertical Delay Ball
      char resmp0;	// 0000 00x0       Reset Missle 0 to Player 0
      char resmp1;	// 0000 00x0       Reset Missle 1 to Player 1
      char hmove;	//  ---- ----       Apply Horizontal Motion
      char hmclr;	//  ---- ----       Clear Horizontal Move Registers
      char cxclr;	//  ---- ----       Clear Collision Latches
    };
  }regs;
} STELLA;


#define RESET	0
#define BRIGHT	1
#define DIM	2
#define UNDERLINE	3
#define BLINK	4
#define REVERSE	7
#define HIDDEN	8

#define BLACK	0
#define RED	1
#define GREEN	2
#define YELLOW	3
#define BLUE	4
#define MAGENTA	5
#define CYAN	6
#define WHITE	7

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define STELLA_HORIZONTAL_LINES 192
#define STELLA_VERTICAL_CLOCK_COUNTS 160

//prototypes

STELLA*
StellaCreate();

int StellaPrintColors();

SDL_Window* init(char *title, int x, int y);
void StellaPutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void cleanup();

void getInput(void);

int
StellaPrintFrame(STELLA *stella, Uint32 framebuffer[STELLA_VERTICAL_CLOCK_COUNTS]
		 [STELLA_HORIZONTAL_LINES]);
Uint32 StellaCreatePixel(unsigned char alpha,
			 unsigned char red,
			 unsigned char green,
			 unsigned char blue);

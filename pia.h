/*
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

// 11111111  Port A; input or output  (read or write) 
#define SWCHA 0x280

//  11111111  Port A DDR, 0= input, 1=output
#define SWACNT 0x281

//   0282       11111111  Port B; console switches (read only)
#define SWCHB  0x282
#define     SWBCNT   0x283 //  11111111  Port B DDR (hardwired as input)
#define     INTIM   0x284 //   11111111  Timer output (read only)
#define    INSTAT   0x285  // 11......  Timer Status (read only, undocumented)

 //   11111111  set 1 clock interval (838 nsec/interval)
#define TIM1T    0x294 

//   11111111  set 8 clock interval (6.7 usec/interval)
#define    TIM8T  0x295
#define    TIM64T   0x296  ///11111111  set 64 clock interval (53.6 usec/interval)
#define     T1024T  0x297 //  11111111  set 1024 clock interval (858.2 usec/interval)
typedef struct _pia{
  unsigned char timer;
  bool timer_pending;
  int timer_interval;
  int timer_interval_timer;
  bool p0difficulty;
  bool p1difficulty;

  bool color;

  bool game_select;
  bool game_reset;
  struct {
    bool p0left;
    bool p0right;
    bool p0up;
    bool p0down;
    bool p0button;
  };
}Pia;

int
PiaCreate();
int
PiaCycle();

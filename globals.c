#include "includes.h"
Cpu *processor;
MemoryMap *memmap;
Tia *tia;

// strobe regs
bool Wsync = false;
bool ResetPlayer0 = false;
bool ResetPlayer1 = false;
bool ResetMissile0 = false;
bool ResetMissile1 = false;
bool ResetBall = false;



bool gQuit = false;
FILE *f = NULL;

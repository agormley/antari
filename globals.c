#include "includes.h"

Cpu *processor;
MemoryMap *memmap;
Tia *tia;
Pia *pia;
SoundGenerator *audio1;
SoundGenerator *audio2;

bool gQuit = false;

FILE *f = NULL;

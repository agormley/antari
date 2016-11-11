#include "includes.h"

char *UsageString = "Usage: atari <rom-name>";

int
RunProgram(STELLA* stella){
  long long counter = 0;

  f = fopen("debug.log", "w+");

  while(1) {
    if(gQuit)
      return 1;
    if(counter % 3 == 0){
      CpuCycle();
      PiaCycle();
    }

    TiaCycle();

    if(counter % (STELLA_VERTICAL_CLOCK_COUNTS*STELLA_HORIZONTAL_LINES) == 0){
      StellaPrintFrame(stella, framebuffer);
    }
    counter++;

  }
}

int size = 4096;

int main(int argc, char *argv[])
{
  STELLA* stella = NULL;

  if (argc != 2){
    printf("%s\n", UsageString);
    return -1;
  }

  /* Start up SDL */
  stella = StellaCreate();

  printf("Stella created\n");

  MemoryCreate();

  TiaCreate();

  PiaCreate();

  //open rom
  int fd  = open(argv[1], O_RDONLY);
  assert(fd >= 0);

  size = read(fd, memmap->rom, ROM_SIZE);

  CpuCreate();

  RunProgram(stella);

  atexit(cleanup);

  /* Exit the program */

  exit(0);
}

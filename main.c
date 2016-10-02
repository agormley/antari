#include "includes.h"

char *UsageString = "Usage: atari <rom-name>";

int
RunProgram(STELLA* tia){
  long long counter = 0;
  
  f = fopen("x.log", "a+"); // a+ (create + append) option will allow appending which is useful in a log file
  while(1) {
    if(gQuit)
      return 1;
    if(counter % 3 == 0)
      CpuCycle();
    
    TiaCycle();

    if(counter % (STELLA_VERTICAL_CLOCK_COUNTS*STELLA_HORIZONTAL_LINES) == 0){
      StellaPrintFrame(tia, framebuffer);
    }
    counter++;
    
  }
}


int main(int argc, char *argv[])
{
  //  int go;

  if (argc != 2){
    printf("%s\n", UsageString);
    return -1;
  }
  
  /* Start up SDL */
  STELLA* tia = NULL;
  tia = StellaCreate(); 
  
  printf("Stella created\n");

  MemoryCreate();
  TiaCreate();
  PiaCreate();
  // ClockCreate();
  //open rom
  int fd  = open(argv[1], O_RDWR);
  assert(fd >= 0);
  read(fd, memmap->rom, ROM_SIZE);
  
  CpuCreate();
  RunProgram(tia);
  
	
  atexit(cleanup);
	
  /* Exit the program */
	
  exit(0);
}

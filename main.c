/* This is a very basic tutorial. It will open a
** 640 x 480 screen and wait for the user to either close
** the window or press escape
*/

#include "includes.h"

char *UsageString = "Usage: atari <rom-name>";

int
RunProgram(STELLA* tia){
  long long counter = 0;

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
  ClockCreate();
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

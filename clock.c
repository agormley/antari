#include "includes.h"

long long clock_counter;

/*1190000 cycles/second*/
/* 0.0000008403 */

#include <unistd.h>

void*
ClockLoopThd(void *threadid)
{
  struct timespec t;  
  t.tv_sec = 0;        /* seconds */
  t.tv_nsec = 50000000;       /* nanoseconds [0 .. 999999999] */
  struct timespec rem;  
 

  clock_counter = 0;
  while(1)
    {
      nanosleep(&t, &rem);
      clock_counter++;
    }
}

int
ClockCreate()
{
  pthread_t thread;
  int rc;
  long t = 0;
  
  rc = pthread_create(&thread, NULL, ClockLoopThd, (void *)t);
  if (rc){
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
  }
 
  return 0;
}


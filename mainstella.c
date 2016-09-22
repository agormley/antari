#include "includes.h"


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

STELLA *tia;

int main(){
  int i = 0;
  int array[100];
  for ( ;i < 100; i++)
    {
      array[i] = i;
    }
  int j = 0;
  int (*twod)[10];
  twod = (int(*)[10])&array[0]; 
  for (i = 0 ;i < 10; i++)
    {
      for (j = 0 ;j < 10; j++)
	{
	  
	  printf("%d:%d:%d\n", i, j, twod[i][j]);
	}
    }
  
  
  /* tia = StellaCreate(); */

  /* printf("Stella created\n"); */

  /* printf("%sred\n", KRED); */
  /* printf("%sgreen\n", KGRN); */
  /* printf("%syellow\n", KYEL); */
  /* printf("%sblue\n", KBLU); */
  /* printf("%smagenta\n", KMAG); */
  /* printf("%scyan\n", KCYN); */
  /* printf("%swhite\n", KWHT); */
  /* printf("%snormal\n", KNRM); */

  /* StellaPrintColors(); */

  return 0;
}

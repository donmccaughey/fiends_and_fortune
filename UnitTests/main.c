#include <stdio.h>
#include <stdlib.h>


void diceTest(void);
void dungeonTest(void);
void pointTest(void);
void pointsTest(void);
void rangeTest(void);
void tilesTest(void);


int main (int argc, const char *argv[])
{
  FILE *out = stdout;
  fprintf(out, "Fiends and Fortune unit tests\n");
  
  diceTest();
  dungeonTest();
  pointTest();
  pointsTest();
  rangeTest();
  tilesTest();

  fprintf(out, "Okay\n");
  return EXIT_SUCCESS;
}

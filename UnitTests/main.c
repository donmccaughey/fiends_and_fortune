#include <stdio.h>
#include <stdlib.h>


void diceTest(void);
void dungeonTest(void);


int main (int argc, const char *argv[])
{
  FILE *out = stdout;
  fprintf(out, "Fiends and Fortune unit tests\n");
  
  diceTest();
  dungeonTest();

  return EXIT_SUCCESS;
}


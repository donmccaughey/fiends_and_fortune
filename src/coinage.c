#include "coinage.h"

#include "heap.h"


char *goldFormat_cp(int cp) {
  int gp = 0;
  if (cp >= 200) {
    gp = cp / CP_PER_GP;
    cp -= gp * CP_PER_GP;
  }
  
  char *string;
  if (gp && cp) {
    ASPRINTF_OR_DIE(&string, "%i gp, %i cp", gp, cp);
  } else if (gp) {
    ASPRINTF_OR_DIE(&string, "%i gp", gp);
  } else {
    ASPRINTF_OR_DIE(&string, "%i cp", cp);
  }
  return string;
}

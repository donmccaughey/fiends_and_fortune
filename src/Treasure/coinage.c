#include "coinage.h"

#include "alloc_or_die.h"


char *goldFormat_cp(int cp) {
  int gp = 0;
  if (cp >= 200) {
    gp = cp / CP_PER_GP;
    cp -= gp * CP_PER_GP;
  }
  
  char *string;
  if (gp && cp) {
    asprintf_or_die(&string, "%i gp, %i cp", gp, cp);
  } else if (gp) {
    asprintf_or_die(&string, "%i gp", gp);
  } else {
    asprintf_or_die(&string, "%i cp", cp);
  }
  return string;
}

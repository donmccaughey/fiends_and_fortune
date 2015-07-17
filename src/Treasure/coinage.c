#include "coinage.h"

#include "earmark.h"


char *goldFormat_cp(int cp) {
  int gp = 0;
  if (cp >= 200) {
    gp = cp / CP_PER_GP;
    cp -= gp * CP_PER_GP;
  }
  
  char *string;
  if (gp && cp) {
    em_asprintf(&string, "%i gp, %i cp", gp, cp);
  } else if (gp) {
    em_asprintf(&string, "%i gp", gp);
  } else {
    em_asprintf(&string, "%i cp", cp);
  }
  return string;
}

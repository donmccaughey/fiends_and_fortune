#include "heap.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unexpected.h"


char *strdupOrDie(char const *file, int line, char const *string) {
  errno = 0;
  char *duplicate = strdup(string);
  if ( ! duplicate) {
    fatalError(file, line, "strdup(\"%s\") failed", string);
  }
  return duplicate;
}

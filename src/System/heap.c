#include "heap.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unexpected.h"


void *mallocOrDie(char const *file, int line, size_t size)
{
    errno = 0;
    void *memory = malloc(size);
    if ( ! memory) {
        fatalError(file, line, "malloc(%li) failed", size);
    }
    return memory;
}


void *reallocOrDie(char const *file, int line, char const *firstArgument,
                   void *memory, size_t size)
{
  errno = 0;
  void *newMemory = realloc(memory, size);
  if ( ! newMemory) {
    if (firstArgument) {
      fatalError(file, line, "realloc(%s, %lu) failed", 
                 firstArgument, (unsigned long) size);
    } else {
      fatalError(file, line, "realloc(%p, %lu) failed", 
                 memory, (unsigned long) size);
    }
  }
  return newMemory;
}


char *strdupOrDie(char const *file, int line, char const *string) {
  errno = 0;
  char *duplicate = strdup(string);
  if ( ! duplicate) {
    fatalError(file, line, "strdup(\"%s\") failed", string);
  }
  return duplicate;
}

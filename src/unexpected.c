#include "unexpected.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void fatalError(char const *file, int line, char const *format, ...) {  
  va_list arguments;
  va_start(arguments, format);
  vfatalError(file, line, format, arguments);
  va_end(arguments);
}


void unexpected(char const *file, int line, char const *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  vfatalError(file, line, format, arguments);
  va_end(arguments);
}


void vfatalError(char const *file, int line, 
                 char const *format, va_list arguments)
{
  if (file) {
    fprintf(stderr, "%s:%i:", file, line);
  }

  fprintf(stderr, "FATAL ERROR: ");
  vfprintf(stderr, format, arguments);
  fprintf(stderr, "\n");
  
  if (errno) {
    fprintf(stderr, "\tlast error was %i: %s\n", errno, strerror(errno));
  }
  
  exit(EXIT_FAILURE);
}

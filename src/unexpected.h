#ifndef UNEXPECTED_H_INCLUDED
#define UNEXPECTED_H_INCLUDED


#include <stdarg.h>


#ifdef NDEBUG


#   define UNEXPECTED( FORMAT, ...) unexpected(NULL, 0, (FORMAT), __VA_ARGS__)


#else


#   define UNEXPECTED( FORMAT, ...) \
        unexpected(__FILE__, __LINE__, (FORMAT), __VA_ARGS__)


#endif /* #ifdef NDEBUG */


void fatalError(char const *file, int line, char const *format, ...);

void unexpected(char const *file, int line, char const *format, ...);

void vfatalError(char const *file, int line, 
                 char const *format, va_list arguments);

#endif /* #ifndef UNEXPECTED_H_INCLUDED */

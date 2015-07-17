#ifndef SYSTEM_HEAP_H_INCLUDED
#define SYSTEM_HEAP_H_INCLUDED


#include <stddef.h>


#ifdef NDEBUG

#   define STRDUP_OR_DIE( STRING ) strdupOrDie(NULL, 0, (STRING))

#else

#   define STRDUP_OR_DIE( STRING ) strdupOrDie(__FILE__, __LINE__, (STRING))

#endif /* #ifdef NDEBUG */

char *strdupOrDie(char const *file, int line, char const *string);


#endif /* #ifndef SYSTEM_HEAP_H_INCLUDED */

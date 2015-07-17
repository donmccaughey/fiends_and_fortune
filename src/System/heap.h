#ifndef SYSTEM_HEAP_H_INCLUDED
#define SYSTEM_HEAP_H_INCLUDED


#include <stddef.h>


#ifdef NDEBUG

#   define REALLOC_OR_DIE( MEMORY, SIZE ) \
        reallocOrDie(NULL, 0, NULL, (MEMORY), (SIZE))

#   define STRDUP_OR_DIE( STRING ) strdupOrDie(NULL, 0, (STRING))

#else

#   define REALLOC_OR_DIE( MEMORY, SIZE ) \
        reallocOrDie(__FILE__, __LINE__, #MEMORY, (MEMORY), (SIZE))

#   define STRDUP_OR_DIE( STRING ) strdupOrDie(__FILE__, __LINE__, (STRING))

#endif /* #ifdef NDEBUG */

void *reallocOrDie(char const *file, int line, char const *firstArgument,
                   void *memory, size_t size);

char *strdupOrDie(char const *file, int line, char const *string);


#endif /* #ifndef SYSTEM_HEAP_H_INCLUDED */

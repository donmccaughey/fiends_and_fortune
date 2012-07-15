#ifndef SYSTEM_HEAP_H_INCLUDED
#define SYSTEM_HEAP_H_INCLUDED


#include <stddef.h>


#ifdef NDEBUG

#   define ASPRINTF_OR_DIE( NEW_STRING, ...) \
        asprintfOrDie(NULL, 0, NULL, NULL, (NEW_STRING), __VA_ARGS__);

#   define CALLOC_OR_DIE( COUNT, SIZE ) callocOrDie(NULL, 0, (COUNT), (SIZE))

#   define MALLOC_OR_DIE( SIZE ) MallocOrDie(NULL, 0, (SIZE))

#   define REALLOC_OR_DIE( MEMORY, SIZE ) \
        reallocOrDie(NULL, 0, NULL, (MEMORY), (SIZE))

#   define STRDUP_OR_DIE( STRING ) strdupOrDie(NULL, 0, (STRING))

#else

#   define ASPRINTF_OR_DIE( NEW_STRING, ...) \
        asprintfOrDie(__FILE__, __LINE__, #NEW_STRING, #__VA_ARGS__, \
                      (NEW_STRING), __VA_ARGS__);

#   define CALLOC_OR_DIE( COUNT, SIZE ) \
        callocOrDie(__FILE__, __LINE__, (COUNT), (SIZE))

#   define MALLOC_OR_DIE( SIZE ) \
        mallocOrDie(__FILE__, __LINE__, (SIZE))

#   define REALLOC_OR_DIE( MEMORY, SIZE ) \
        reallocOrDie(__FILE__, __LINE__, #MEMORY, (MEMORY), (SIZE))

#   define STRDUP_OR_DIE( STRING ) strdupOrDie(__FILE__, __LINE__, (STRING))

#endif /* #ifdef NDEBUG */


int asprintfOrDie(char const *file, int line, 
                  char const *firstArgument, char const *otherArguments,
                  char **newString, char const *format, ...);

void *callocOrDie(char const *file, int line, size_t count, size_t size);

void *mallocOrDie(char const *file, int line, size_t size);

void *reallocOrDie(char const *file, int line, char const *firstArgument,
                   void *memory, size_t size);

char *strdupOrDie(char const *file, int line, char const *string);


#endif /* #ifndef SYSTEM_HEAP_H_INCLUDED */

#ifndef FNF_CHARACTER_LANGUAGE_H_INCLUDED
#define FNF_CHARACTER_LANGUAGE_H_INCLUDED


#include <stddef.h>


struct rnd;


char const *
determine_language(struct rnd *rnd,
                   char const *exclude[],
                   size_t exclude_count);


#endif

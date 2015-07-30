#ifndef FNF_COMMON_LANGUAGE_H_INCLUDED
#define FNF_COMMON_LANGUAGE_H_INCLUDED


#include <stddef.h>


struct rnd;


char const *
language_determine(struct rnd *rnd,
                   char const *exclude[],
                   size_t exclude_count);


#endif

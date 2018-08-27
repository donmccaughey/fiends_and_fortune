#ifndef FNF_CHARACTER_CHARACTERISTICS_H_INCLUDED
#define FNF_CHARACTER_CHARACTERISTICS_H_INCLUDED


#include <stddef.h>
#include <stdint.h>

#include <character/characteristic_flag.h>
#include <character/characteristic_generation_method.h>


struct rnd;


int *
characteristics_alloc(struct rnd *rnd,
                      enum characteristic_generation_method method,
                      enum characteristic_flag flags);

void
characteristics_free(int *characteristics);


#endif

#ifndef FNF_CHARACTER_CHARACTERISTICS_H_INCLUDED
#define FNF_CHARACTER_CHARACTERISTICS_H_INCLUDED


#include <character/characteristic_flag.h>
#include <character/characteristic_generation_method.h>


struct rnd;


struct characteristics {
    int strength;
    int intelligence;
    int wisdom;
    int dexterity;
    int constitution;
    int charisma;
};


int *
characteristics_alloc(struct rnd *rnd,
                      enum characteristic_generation_method method,
                      enum characteristic_flag flags);

struct characteristics *
characteristics_alloc_general_NPC(struct rnd *rnd);

struct characteristics *
characteristics_alloc_method_3(struct rnd *rnd);

struct characteristics *
characteristics_alloc_simple_method(struct rnd *rnd);

struct characteristics *
characteristics_alloc_special_NPC(struct rnd *rnd,
                                  enum characteristic_flag flags);

void
characteristics_free(int *characteristics);

void
characteristics_struct_free(struct characteristics *characteristics);


#endif

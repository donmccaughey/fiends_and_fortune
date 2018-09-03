#ifndef FNF_CHARACTER_CHARACTERISTICS_H_INCLUDED
#define FNF_CHARACTER_CHARACTERISTICS_H_INCLUDED


#include <character/ability_flag.h>


struct rnd;


struct abilities {
    int strength;
    int intelligence;
    int wisdom;
    int dexterity;
    int constitution;
    int charisma;
};


struct abilities *
abilities_alloc_general_NPC(struct rnd *rnd);

struct abilities *
abilities_alloc_method_3(struct rnd *rnd);

struct abilities *
abilities_alloc_simple_method(struct rnd *rnd);

struct abilities *
abilities_alloc_special_NPC(struct rnd *rnd,
                            enum ability_flag flags);

int
abilities_compare(struct abilities const *first,
                  struct abilities const *second);

void
abilities_free(struct abilities *abilities);

int
abilities_total(struct abilities const *abilities);


#endif

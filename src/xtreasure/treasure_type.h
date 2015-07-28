#ifndef FNF_TREASURE_TREASURE_TYPE_H_INCLUDED
#define FNF_TREASURE_TREASURE_TYPE_H_INCLUDED


#include <stdbool.h>
#include <stddef.h>


struct rnd;
struct Treasure;
struct treasure_type;


char *
treasure_type_alloc_description(struct treasure_type *treasure_type,
                                bool include_header);

void
treasure_generate(struct treasure_type *treasure_type,
                  struct Treasure *treasure,
                  struct rnd *rnd,
                  int individual_count);

struct treasure_type *
treasure_type_by_letter(char letter);


#endif

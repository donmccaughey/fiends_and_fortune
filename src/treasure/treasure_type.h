#ifndef FNF_TREASURE_TREASURE_TYPE_H_INCLUDED
#define FNF_TREASURE_TREASURE_TYPE_H_INCLUDED


#include <stdbool.h>


struct rnd;
struct treasure;
struct treasure_type;


char *
treasure_type_alloc_description(struct treasure_type *treasure_type,
                                bool include_header);

char *
treasure_type_alloc_name(struct treasure_type *treasure_type);

void
treasure_type_generate(struct treasure_type *treasure_type,
                       struct rnd *rnd,
                       struct treasure *treasure);

struct treasure_type *
treasure_type_by_letter(char letter);


#endif

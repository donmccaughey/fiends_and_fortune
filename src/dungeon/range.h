#ifndef FNF_DUNGEON_RANGE_H_INCLUDED
#define FNF_DUNGEON_RANGE_H_INCLUDED


#include <stdbool.h>


struct range {
    int begin;
    int end;
};


bool
range_contains(struct range range, int value);

bool
range_is_equal(struct range range, struct range other);

struct range
range_expand(struct range range, int amount);

struct range
range_extend_to_include_value(struct range range, int value);

bool
range_is_empty(struct range range);

int
range_length(struct range range);

struct range
range_make(int begin, int end);

struct range
range_join(struct range range, struct range other);


#endif

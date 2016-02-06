#ifndef FNF_DUNGEON_REVERSE_RANGE_H_INCLUDED
#define FNF_DUNGEON_REVERSE_RANGE_H_INCLUDED


#include "range.h"


struct reverse_range {
    int top;
    int bottom;
};


bool
reverse_range_is_equal(struct reverse_range reverse_range,
                       struct reverse_range other);

struct reverse_range
reverse_range_expand(struct reverse_range reverse_range, int amount);

struct reverse_range
reverse_range_extend_to_include_value(struct reverse_range reverse_range,
                                      int value);

bool
reverse_range_is_empty(struct reverse_range reverse_range);

int
reverse_range_length(struct reverse_range reverse_range);

struct range
range_from_reverse_range(struct reverse_range reverse_range);

struct reverse_range
reverse_range_make(int top, int bottom);

struct reverse_range
reverse_range_from_range(struct range range);


#endif

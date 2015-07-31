#ifndef FNF_DUNGEON_REVERSE_RANGE_H_INCLUDED
#define FNF_DUNGEON_REVERSE_RANGE_H_INCLUDED


#include "range.h"


struct reverse_range {
    int32_t top;
    int32_t bottom;
};


bool
reverse_range_is_equal(struct reverse_range reverse_range1,
                       struct reverse_range reverse_range2);

struct reverse_range
reverse_range_expand(struct reverse_range reverse_range, int32_t amount);

struct reverse_range
reverse_range_extend_to_include_value(struct reverse_range reverse_range,
                                      int32_t value);

bool
reverse_range_is_empty(struct reverse_range reverse_range);

uint32_t
reverse_range_length(struct reverse_range reverse_range);

struct range
range_from_reverse_range(struct reverse_range reverse_range);

struct reverse_range
reverse_range_make(int32_t top, int32_t bottom);

struct reverse_range
reverse_range_from_range(struct range range);


#endif

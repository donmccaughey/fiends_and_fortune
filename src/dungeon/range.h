#ifndef FNF_DUNGEON_RANGE_H_INCLUDED
#define FNF_DUNGEON_RANGE_H_INCLUDED


#include <stdbool.h>
#include <stdint.h>


struct range {
    int32_t begin;
    int32_t end;
};


bool
range_contains(struct range range, int32_t value);

bool
range_is_equal(struct range range1, struct range range2);

struct range
range_expand(struct range range, int32_t amount);

struct range
range_extend_to_include_value(struct range range, int32_t value);

bool
range_is_empty(struct range range);

uint32_t
range_length(struct range range);

struct range
range_make(int32_t begin, int32_t end);

struct range
range_join(struct range range1, struct range range2);


#endif

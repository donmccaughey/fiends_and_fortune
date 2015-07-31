#ifndef FNF_DUNGEON_RANGE_H_INCLUDED
#define FNF_DUNGEON_RANGE_H_INCLUDED


#include <stdbool.h>
#include <stdint.h>


struct range {
    int32_t begin;
    int32_t end;
};

struct reverse_range {
    int32_t top;
    int32_t bottom;
};


bool
range_is_equal(struct range range1, struct range range2);

bool
reverse_range_is_equal(struct reverse_range reverse_range1,
                       struct reverse_range reverse_range2);

struct range
range_expand(struct range range, int32_t amount);

struct reverse_range
reverse_range_expand(struct reverse_range reverse_range, int32_t amount);

struct range
range_extend_to_include_value(struct range range, int32_t value);

struct reverse_range
reverse_range_extend_to_include_value(struct reverse_range reverse_range,
                                      int32_t value);

bool
range_is_empty(struct range range);

bool
reverse_range_is_empty(struct reverse_range reverse_range);

uint32_t
range_length(struct range range);

uint32_t
reverse_range_length(struct reverse_range reverse_range);

struct range
range_make(int32_t begin, int32_t end);

struct range
range_from_reverse_range(struct reverse_range reverse_range);

struct reverse_range
reverse_range_make(int32_t top, int32_t bottom);

struct reverse_range
reverse_range_from_range(struct range range);


#endif

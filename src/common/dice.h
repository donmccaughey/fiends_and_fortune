#ifndef FNF_COMMON_DICE_H_INCLUDED
#define FNF_COMMON_DICE_H_INCLUDED


#include <assert.h>
#include <limits.h>
#include <stddef.h>


struct rnd;


struct dice {
    int count;
    int sides;
    int modifier;
    int multiplier;
};


char *
dice_alloc_base_range_description(struct dice dice);

char *
dice_alloc_description(struct dice dice);

char *
dice_alloc_range_description(struct dice dice);

struct dice
dice_make(int count, int sides);

struct dice
dice_make_plus(int count, int sides, int modifier);

struct dice
dice_make_plus_times(int count, int sides, int modifier, int multiplier);

int
dice_max_base_score(struct dice dice);

int
dice_max_score(struct dice dice);

int
dice_min_base_score(struct dice dice);

int
dice_min_score(struct dice dice);

struct dice
dice_parse(char const *dice_string);

int
dice_roll(struct dice dice, struct rnd *rnd, int die_scores[]);

int
dice_roll_with_average_scoring(struct dice dice, struct rnd *rnd);

int
dice_roll_and_adjust_upwards(struct dice dice, struct rnd *rnd);

int
dice_roll_and_drop_lowest(struct dice dice, struct rnd *rnd);

int
roll(char const *dice_string, struct rnd *rnd);


#endif /* #ifndef DICE_H_INCLUDED */

#ifndef FNF_MECHANICS_DICE_H_INCLUDED
#define FNF_MECHANICS_DICE_H_INCLUDED


#include <stdbool.h>


struct rnd;


struct xdice {
    int count;
    int sides;
    int modifier;
    int multiplier;
};


char *
xdice_alloc_base_range_description(struct xdice dice);

char *
xdice_alloc_description(struct xdice dice);

char *
xdice_alloc_range_description(struct xdice dice);

bool
xdice_has_constant_score(struct xdice dice);

bool
xdice_is_valid(struct xdice dice);

struct xdice
xdice_make(int count, int sides);

struct xdice
xdice_make_plus(int count, int sides, int modifier);

struct xdice
xdice_make_plus_times(int count, int sides, int modifier, int multiplier);

int
xdice_max_base_score(struct xdice dice);

int
xdice_max_score(struct xdice dice);

int
xdice_min_base_score(struct xdice dice);

int
xdice_min_score(struct xdice dice);

struct xdice
xdice_parse(char const *dice_string);

int
xdice_roll(struct xdice dice, struct rnd *rnd, int die_scores[]);

int
xdice_roll_and_adjust_upwards(struct xdice dice, struct rnd *rnd);

int
xdice_roll_and_drop_lowest(struct xdice dice, struct rnd *rnd);

int
xdice_roll_with_average_scoring(struct xdice dice, struct rnd *rnd);

int
xroll(char const *dice_string, struct rnd *rnd);


#endif

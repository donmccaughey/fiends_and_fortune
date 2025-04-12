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

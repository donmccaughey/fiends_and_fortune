#include "xdice.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <base/base.h>

#include "astr.h"


static inline double
max_possible_total(struct xdice dice);


static int
compare_die_scores(void const *item1, void const *item2)
{
    int const *score1 = item1;
    int const *score2 = item2;
    if (*score1 < *score2) return -1;
    if (*score1 > *score2) return 1;
    return 0;
}


bool
xdice_is_valid(struct xdice dice)
{
    if (dice.count < 0) return false;
    if (dice.sides < 1) return false;
    if (max_possible_total(dice) > (double)INT_MAX) return false;
    return true;
}


struct xdice
xdice_make(int count, int sides)
{
    return xdice_make_plus(count, sides, 0);
}


struct xdice
xdice_make_plus(int count, int sides, int modifier)
{
    return xdice_make_plus_times(count, sides, modifier, 1);
}


struct xdice
xdice_make_plus_times(int count, int sides, int modifier, int multiplier)
{
    return (struct xdice){
        .count=count,
        .sides=sides,
        .modifier=modifier,
        .multiplier=multiplier,
    };
}


int
xdice_max_base_score(struct xdice dice)
{
    assert(xdice_is_valid(dice));
    return (dice.count * dice.sides) + dice.modifier;
}


int
xdice_max_score(struct xdice dice)
{
    assert(xdice_is_valid(dice));
    return xdice_max_base_score(dice) * dice.multiplier;
}


struct xdice
xdice_parse(char const *dice_string)
{
    assert(dice_string);
    assert(dice_string[0]);

    struct xdice dice = {
        .count=0,
        .sides=1,
        .modifier=0,
        .multiplier=1,
    };
    
    int base = 10;
    char *end;
    errno = 0;
    long count = strtol(dice_string, &end, base);
    assert(!errno);
    
    assert(count >= 0 && count <= INT_MAX);
    dice.count = (int)count;
    
    if (end[0] == 'D' || end[0] == 'd') {
        long sides = strtol(end + 1, &end, base);
        assert(!errno);
        
        assert(sides > 1 && sides <= INT_MAX);
        dice.sides = (int)sides;
    } else {
        dice.sides = 1;
    }
    
    if (end[0] == '+' || end[0] == '-') {
        long modifier = strtol(end, &end, base);
        assert(!errno);
        
        assert(modifier >= INT_MIN && modifier <= INT_MAX);
        dice.modifier = (int)modifier;
    }
    
    if (end[0] == '*' || end[0] == 'X' || end[0] == 'x') {
        long multiplier = strtol(end + 1, &end, base);
        assert(!errno);
        
        assert(multiplier >= INT_MIN && multiplier <= INT_MAX);
        dice.multiplier = (int)multiplier;
    }

    assert(xdice_is_valid(dice));
    return dice;
}


int
xdice_roll(struct xdice dice, struct rnd *rnd, int die_scores[])
{
    assert(xdice_is_valid(dice));
    assert(rnd);
    
    if (dice.count == 0) {
        return dice.modifier * dice.multiplier;
    } else if (dice.sides == 1) {
        return (dice.count + dice.modifier) * dice.multiplier;
    } else {
        int score = dice.modifier;
        for (int i = 0; i < dice.count; ++i) {
            int die_score = (int)rnd_next_uniform_value_in_range(rnd,
                                                                 1,
                                                                 dice.sides);
            if (die_scores) die_scores[i] = die_score;
            score += die_score;
        }
        return score * dice.multiplier;
    }
}


int
xdice_roll_and_adjust_upwards(struct xdice dice, struct rnd *rnd)
{
    assert(xdice_is_valid(dice));
    assert(rnd);

    int die_scores[dice.count];
    xdice_roll(dice, rnd, die_scores);

    int score = 0;
    for (int i = 0; i < dice.count; ++i) {
        if (die_scores[i] < 6) {
            score += die_scores[i] + 1;
        } else {
            score += die_scores[i];
        }
    }
    return score;
}


int
xdice_roll_and_drop_lowest(struct xdice dice, struct rnd *rnd)
{
    assert(xdice_is_valid(dice));
    assert(rnd);

    int die_scores[dice.count];
    xdice_roll(dice, rnd, die_scores);
    qsort(die_scores, (size_t)dice.count, sizeof die_scores[0], compare_die_scores);

    int score = 0;
    for (int i = 1; i < dice.count; ++i) {
        score += die_scores[i];
    }
    return score;
}


int
xdice_roll_with_average_scoring(struct xdice dice, struct rnd *rnd)
{
    assert(xdice_is_valid(dice));
    assert(rnd);

    int die_scores[dice.count];
    xdice_roll(dice, rnd, die_scores);

    int const high_roll = dice.sides;
    int const low_roll = 1;
    int const low_plus_high = high_roll + low_roll;
    int const low_average = low_plus_high / 2;
    int const high_average = low_average + (low_plus_high % 2);

    int score = 0;
    for (int i = 0; i < dice.count; ++i) {
        if (die_scores[i] == low_roll) {
            score += low_average;
        } else if (die_scores[i] == high_roll) {
            score += high_average;
        } else {
            score += die_scores[i];
        }
    }
    return score;
}


static inline double
max_possible_total(struct xdice dice)
{
    double max_possible_roll = (double)dice.count * (double)dice.sides;
    double max_possible_total = max_possible_roll + (double)dice.modifier;
    return max_possible_total * (double)dice.multiplier;
}


int
xroll(char const *dice_string, struct rnd *rnd)
{
    return xdice_roll(xdice_parse(dice_string), rnd, NULL);
}

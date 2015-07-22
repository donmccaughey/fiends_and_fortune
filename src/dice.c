#include "dice.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rnd.h"
#include "unexpected.h"


static int
compare_dice_scores(void const *score1, void const *score2)
{
    return *((int *)score1) - *((int *)score2);
}


int
dice_max_score(char const *dice_string)
{
    struct dice dice = dice_parse(dice_string);
    assert((double)dice.count * (double)dice.sides <= (double)INT_MAX);
    return dice.count * dice.sides;
}


int
dice_min_score(char const *dice_string)
{
    struct dice dice = dice_parse(dice_string);
    return dice.count;
}


struct dice
dice_parse(char const *dice_string)
{
    struct dice dice = {
        .count=1,
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
        assert(multiplier != 0 && multiplier != 1);
        dice.multiplier = (int)multiplier;
    }
    return dice;
}


int
roll(struct rnd *rnd, char const *dice_string)
{
    struct dice dice = dice_parse(dice_string);
    return dice_roll(dice, rnd, NULL);
}


int
rollDice(struct rnd *rnd, int count, int sides)
{
    return dice_roll(dice_make(count, sides), rnd, NULL);
}


int
rollDiceAndAdjustTowardsAverage(struct rnd *rnd, int count, int sides)
{
    int die_scores[count];
    dice_roll(dice_make(count, sides), rnd, die_scores);
    
    int const high_roll = sides;
    int const low_roll = 1;
    int const low_plus_high = high_roll + low_roll;
    int const low_average = low_plus_high / 2;
    int const high_average = low_average + (low_plus_high % 2);
    
    int score = 0;
    for (int i = 0; i < count; ++i) {
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


int
rollDiceAndAdjustUpwards(struct rnd *rnd, int count, int sides)
{
    int die_scores[count];
    dice_roll(dice_make(count, sides), rnd, die_scores);
    
    int score = 0;
    for (int i = 0; i < count; ++i) {
        if (die_scores[i] < 6) {
            score += die_scores[i] + 1;
        } else {
            score += die_scores[i];
        }
    }
    return score;
}


int
rollDiceAndDropLowest(struct rnd *rnd, int count, int sides)
{
    int die_scores[count];    
    dice_roll(dice_make(count, sides), rnd, die_scores);
    qsort(die_scores, (size_t)count, sizeof die_scores[0], compare_dice_scores);
    
    int score = 0;
    for (int i = 1; i < count; ++i) {
        score += die_scores[i];
    }
    return score;
}


int
rollDicePlus(struct rnd *rnd, int count, int sides, int modifier)
{
    struct dice dice = {
        .count=count,
        .sides=sides,
        .modifier=modifier,
        .multiplier=1,
    };
    return dice_roll(dice, rnd, NULL);
}


static inline double
max_possible_total(struct dice dice)
{
    double max_possible_roll = (double)dice.count * (double)dice.sides;
    double max_possible_total = max_possible_roll + (double)dice.modifier;
    return max_possible_total * (double)dice.multiplier;
}


int
dice_roll(struct dice dice, struct rnd *rnd, int die_scores[])
{
    assert(dice.count >= 0);
    assert(dice.sides > 0);
    assert(max_possible_total(dice) <= (double)INT_MAX);
    
    if (dice.count == 0) {
        return 0;
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

extern inline struct dice
dice_make(int count, int sides);

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
rollDieRoll(struct rnd *rnd,
            struct dice dice,
            int diceRolled[]);


static int
compareDieRolls(void const *die1, void const *die2)
{
    return *((int *)die1) - *((int *)die2);
}


int
maxDieRoll(char const *dice_string)
{
    struct dice dice = dice_parse(dice_string);
    return dice.count * dice.sides;
}


int
minDieRoll(char const *dice_string)
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
    return rollDieRoll(rnd, dice, NULL);
}


int
rollDice(struct rnd *rnd, int count, int sides)
{
    struct dice dice = {
        .count=count,
        .sides=sides,
        .modifier=0,
        .multiplier=1,
    };
    return rollDieRoll(rnd, dice, NULL);
}


int
rollDiceAndAdjustTowardsAverage(struct rnd *rnd, int count, int sides)
{
    struct dice dice = {
        .count=count,
        .sides=sides,
        .modifier=0,
        .multiplier=1,
    };
    int diceRolled[count];
    
    rollDieRoll(rnd, dice, diceRolled);
    
    int total = 0;
    int const highRoll = sides;
    int const lowRoll = 1;
    int const lowPlusHigh = highRoll + lowRoll;
    int const lowAverage = lowPlusHigh / 2;
    int const highAverage = lowAverage + (lowPlusHigh % 2);
    for (int i = 0; i < count; ++i) {
        if (diceRolled[i] == 1) {
            total += lowAverage;
        } else if (diceRolled[i] == sides) {
            total += highAverage;
        } else {
            total += diceRolled[i];
        }
    }
    return total;
}


int
rollDiceAndAdjustUpwards(struct rnd *rnd, int count, int sides)
{
    struct dice dice = {
        .count=count,
        .sides=sides,
        .modifier=0,
        .multiplier=1,
    };
    int diceRolled[count];
    
    rollDieRoll(rnd, dice, diceRolled);
    
    int total = 0;
    for (int i = 0; i < count; ++i) {
        if (diceRolled[i] < 6) {
            total += diceRolled[i] + 1;
        } else {
            total += diceRolled[i];
        }
    }
    return total;
}


int
rollDiceAndDropLowest(struct rnd *rnd, int count, int sides)
{
    struct dice dice = {
        .count=count,
        .sides=sides,
        .modifier=0,
        .multiplier=1,
    };
    int diceRolled[count];
    
    rollDieRoll(rnd, dice, diceRolled);
    qsort(diceRolled, (size_t)count, sizeof diceRolled[0], compareDieRolls);
    
    int total = 0;
    for (int i = 1; i < count; ++i) {
        total += diceRolled[i];
    }
    return total;
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
    return rollDieRoll(rnd, dice, NULL);
}


static inline double
max_possible_total(struct dice dice)
{
    double max_possible_roll = (double)dice.count * (double)dice.sides;
    double max_possible_total = max_possible_roll + (double)dice.modifier;
    return max_possible_total * (double)dice.multiplier;
}


static int
rollDieRoll(struct rnd *rnd,
            struct dice dice,
            int diceRolled[])
{
    assert(dice.count >= 0);
    assert(dice.sides > 0);
    assert(max_possible_total(dice) <= (double)INT_MAX);
    
    if (dice.count == 0) {
        return 0;
    } else if (dice.sides == 1) {
        return (dice.count + dice.modifier) * dice.multiplier;
    } else {
        int total = dice.modifier;
        for (int i = 0; i < dice.count; ++i) {
            int result = (int)rnd_next_uniform_value_in_range(rnd,
                                                              1,
                                                              dice.sides + 1);
            if (diceRolled) diceRolled[i] = result;
            total += result;
        }
        return total * dice.multiplier;
    }
}

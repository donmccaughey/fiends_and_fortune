#include "die_roll.h"

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rnd.h"
#include "unexpected.h"


static int compareDieRolls(void const *die1, void const *die2);

static int rollDieRoll(struct rnd *rnd,
                       struct die_roll die_roll,
                       int diceRolled[]);


static int compareDieRolls(void const *die1, void const *die2)
{
    return *((int *)die1) - *((int *)die2);
}


int maxDieRoll(char const *dieRollString)
{
    struct die_roll die_roll = parseDieRoll(dieRollString);
    return die_roll.count * die_roll.sides;
}


int minDieRoll(char const *dieRollString)
{
    struct die_roll die_roll = parseDieRoll(dieRollString);
    return die_roll.count;
}


struct die_roll parseDieRoll(char const *dieRollString)
{
    struct die_roll die_roll = {
        .count=1,
        .sides=1,
        .modifier=0,
        .multiplier=1,
    };
    
    int base = 10;
    char *end;
    errno = 0;
    long count = strtol(dieRollString, &end, base);
    assert(!errno);
    
    assert(count >= 0 && count <= INT_MAX);
    die_roll.count = (int)count;
    
    if (end[0] == 'D' || end[0] == 'd') {
        long sides = strtol(end + 1, &end, base);
        assert(!errno);
        
        assert(sides > 1 && sides <= INT_MAX);
        die_roll.sides = (int)sides;
    } else {
        die_roll.sides = 1;
    }
    
    if (end[0] == '+' || end[0] == '-') {
        long modifier = strtol(end, &end, base);
        assert(!errno);
        
        assert(modifier >= INT_MIN && modifier <= INT_MAX);
        die_roll.modifier = (int)modifier;
    }
    
    if (end[0] == '*' || end[0] == 'X' || end[0] == 'x') {
        long multiplier = strtol(end + 1, &end, base);
        assert(!errno);
        
        assert(multiplier >= INT_MIN && multiplier <= INT_MAX);
        assert(multiplier != 0 && multiplier != 1);
        die_roll.multiplier = (int)multiplier;
    }
    return die_roll;
}


int roll(struct rnd *rnd, char const *dieRollString)
{
    struct die_roll die_roll = parseDieRoll(dieRollString);
    return rollDieRoll(rnd, die_roll, NULL);
}


int rollDice(struct rnd *rnd, int count, int sides)
{
    struct die_roll die_roll = {
        .count=count,
        .sides=sides,
        .modifier=0,
        .multiplier=1,
    };
    return rollDieRoll(rnd, die_roll, NULL);
}


int rollDiceAndAdjustTowardsAverage(struct rnd *rnd, int count, int sides)
{
    struct die_roll die_roll = {
        .count=count,
        .sides=sides,
        .modifier=0,
        .multiplier=1,
    };
    int diceRolled[count];
    
    rollDieRoll(rnd, die_roll, diceRolled);
    
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


int rollDiceAndAdjustUpwards(struct rnd *rnd, int count, int sides)
{
    struct die_roll die_roll = {
        .count=count,
        .sides=sides,
        .modifier=0,
        .multiplier=1,
    };
    int diceRolled[count];
    
    rollDieRoll(rnd, die_roll, diceRolled);
    
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


int rollDiceAndDropLowest(struct rnd *rnd, int count, int sides)
{
    struct die_roll die_roll = {
        .count=count,
        .sides=sides,
        .modifier=0,
        .multiplier=1,
    };
    int diceRolled[count];
    
    rollDieRoll(rnd, die_roll, diceRolled);
    qsort(diceRolled, (size_t)count, sizeof diceRolled[0], compareDieRolls);
    
    int total = 0;
    for (int i = 1; i < count; ++i) {
        total += diceRolled[i];
    }
    return total;
}


int rollDicePlus(struct rnd *rnd, int count, int sides, int modifier)
{
    struct die_roll die_roll = {
        .count=count,
        .sides=sides,
        .modifier=modifier,
        .multiplier=1,
    };
    return rollDieRoll(rnd, die_roll, NULL);
}


static inline double
max_possible_total(struct die_roll die_roll)
{
    double max_possible_roll = (double)die_roll.count * (double)die_roll.sides;
    double max_possible_total = max_possible_roll + (double)die_roll.modifier;
    return max_possible_total * (double)die_roll.multiplier;
}


static int rollDieRoll(struct rnd *rnd,
                       struct die_roll die_roll,
                       int diceRolled[])
{
    assert(die_roll.count >= 0);
    assert(die_roll.sides > 0);
    assert(max_possible_total(die_roll) <= (double)INT_MAX);
    
    if (die_roll.count == 0) {
        return 0;
    } else if (die_roll.sides == 1) {
        return (die_roll.count + die_roll.modifier) * die_roll.multiplier;
    } else {
        int total = die_roll.modifier;
        for (int i = 0; i < die_roll.count; ++i) {
            int result = (int)rnd_next_uniform_value_in_range(rnd,
                                                              1,
                                                              die_roll.sides + 1);
            if (diceRolled) diceRolled[i] = result;
            total += result;
        }
        return total * die_roll.multiplier;
    }
}

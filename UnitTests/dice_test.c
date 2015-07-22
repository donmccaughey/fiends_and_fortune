#include <assert.h>

#include "dice.h"
#include "rnd.h"


void
dice_test(void);


static void
dice_parse_test(void)
{
    struct dice dice;
    
    dice = dice_parse("0");
    assert(0 == dice.count);
    assert(1 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = dice_parse("1");
    assert(1 == dice.count);
    assert(1 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = dice_parse("5");
    assert(5 == dice.count);
    assert(1 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = dice_parse("1d6");
    assert(1 == dice.count);
    assert(6 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = dice_parse("2D6");
    assert(2 == dice.count);
    assert(6 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = dice_parse("10+1");
    assert(10 == dice.count);
    assert(1 == dice.sides);
    assert(1 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = dice_parse("1d6+1");
    assert(1 == dice.count);
    assert(6 == dice.sides);
    assert(1 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = dice_parse("2d4-1");
    assert(2 == dice.count);
    assert(4 == dice.sides);
    assert(-1 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = dice_parse("5*10");
    assert(5 == dice.count);
    assert(1 == dice.sides);
    assert(0 == dice.modifier);
    assert(10 == dice.multiplier);
    
    dice = dice_parse("1d6*10");
    assert(1 == dice.count);
    assert(6 == dice.sides);
    assert(0 == dice.modifier);
    assert(10 == dice.multiplier);
    
    dice = dice_parse("5d4*-5");
    assert(5 == dice.count);
    assert(4 == dice.sides);
    assert(0 == dice.modifier);
    assert(-5 == dice.multiplier);
    
    dice = dice_parse("2d10+4*100");
    assert(2 == dice.count);
    assert(10 == dice.sides);
    assert(4 == dice.modifier);
    assert(100 == dice.multiplier);
}


static void
rollTest(void)
{
    struct rnd *always_one = rnd_alloc_fake_fixed(0);
    struct rnd *always_two = rnd_alloc_fake_fixed(1);
    int score;
    
    /* "roll" a constant number */
    score = roll("0", always_one);
    assert(0 == score);
    
    score = roll("3", always_one);
    assert(3 == score);
    
    score = roll("1+1", always_two);
    assert(2 == score);
    
    score = roll("5-1", always_two);
    assert(4 == score);
    
    score = roll("5*10", always_two);
    assert(50 == score);
    
    /* roll dice that always roll 1's */
    score = roll("0d4+1", always_one);
    assert(0 == score);
    
    score = roll("1D6+2", always_one);
    assert(3 == score);
    
    score = roll("2d10+1", always_one);
    assert(3 == score);
    
    score = roll("3d4+5", always_one);
    assert(8 == score);
    
    score = roll("1d12*5", always_one);
    assert(5 == score);
    
    score = roll("2d20+1*10", always_one);
    assert(30 == score);
    
    /* roll dice that always roll 2's */
    score = roll("1D6-2", always_two);
    assert(0 == score);
    
    score = roll("2d10-2", always_two);
    assert(2 == score);
    
    score = roll("3d4-3", always_two);
    assert(3 == score);
    
    score = roll("2d8*5", always_two);
    assert(20 == score);
    
    score = roll("2d20+1*10", always_two);
    assert(50 == score);
    
    rnd_free(always_one);
    rnd_free(always_two);
}


static void
dice_roll_with_average_scoring_test(void)
{
    struct rnd *ascending = rnd_alloc_fake_ascending(0);
    struct rnd *always_one = rnd_alloc_fake_fixed(0);
    struct rnd *always_two = rnd_alloc_fake_fixed(1);
    int score;
    
    score = dice_roll_with_average_scoring(dice_make(3, 6), ascending);
    assert(8 == score);
    
    score = dice_roll_with_average_scoring(dice_make(3, 6), always_one);
    assert(9 == score);
    
    score = dice_roll_with_average_scoring(dice_make(3, 6), always_two);
    assert(6 == score);
    
    rnd_free(ascending);
    rnd_free(always_one);
    rnd_free(always_two);
}


static void
dice_roll_and_drop_lowest_test(void)
{
    struct rnd *ascending = rnd_alloc_fake_ascending(0);
    struct rnd *always_one = rnd_alloc_fake_fixed(0);
    struct rnd *always_two = rnd_alloc_fake_fixed(1);
    int score;
    
    score = dice_roll_and_drop_lowest(dice_make(4, 6), ascending);
    assert(9 == score);
    
    score = dice_roll_and_drop_lowest(dice_make(3, 6), always_one);
    assert(2 == score);
    
    score = dice_roll_and_drop_lowest(dice_make(5, 4), always_two);
    assert(8 == score);
    
    rnd_free(ascending);
    rnd_free(always_one);
    rnd_free(always_two);
}


void
dice_test(void)
{
    dice_parse_test();
    rollTest();
    dice_roll_with_average_scoring_test();
    dice_roll_and_drop_lowest_test();
}

#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <base/base.h>
#include <mechanics/mechanics.h>


void
xdice_test(void);


static void
xdice_is_valid_test(void)
{
    assert( !xdice_is_valid(xdice_make(-1, 6)));
    assert( !xdice_is_valid(xdice_make(2, 0)));
    assert( !xdice_is_valid(xdice_make(INT_MAX / 6 + 1, 6)));

    assert(xdice_is_valid(xdice_make(1, 2)));
    assert(xdice_is_valid(xdice_make(2, 6)));
    assert(xdice_is_valid(xdice_make(INT_MAX / 6, 6)));
}


static void
xdice_make_test(void)
{
    struct xdice dice = xdice_make(2, 6);

    assert(2 == dice.count);
    assert(6 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);
}


static void
xdice_make_plus_test(void)
{
    struct xdice dice = xdice_make_plus(2, 6, 2);

    assert(2 == dice.count);
    assert(6 == dice.sides);
    assert(2 == dice.modifier);
    assert(1 == dice.multiplier);
}


static void
xdice_make_plus_times_test(void)
{
    struct xdice dice = xdice_make_plus_times(2, 6, 2, 10);

    assert(2 == dice.count);
    assert(6 == dice.sides);
    assert(2 == dice.modifier);
    assert(10 == dice.multiplier);
}


static void
xdice_max_base_score_test(void)
{
    struct xdice dice = xdice_make_plus(2, 4, 10);
    assert(18 == xdice_max_base_score(dice));
}


static void
xdice_max_score_test(void)
{
    struct xdice dice = xdice_make_plus_times(2, 4, 10, 100);
    assert(1800 == xdice_max_score(dice));
}


static void
xdice_parse_test(void)
{
    struct xdice dice;

    dice = xdice_parse("0");
    assert(0 == dice.count);
    assert(1 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);

    dice = xdice_parse("1");
    assert(1 == dice.count);
    assert(1 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = xdice_parse("5");
    assert(5 == dice.count);
    assert(1 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = xdice_parse("0d4+2");
    assert(0 == dice.count);
    assert(4 == dice.sides);
    assert(2 == dice.modifier);
    assert(1 == dice.multiplier);

    dice = xdice_parse("1d6");
    assert(1 == dice.count);
    assert(6 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);

    dice = xdice_parse("2D6");
    assert(2 == dice.count);
    assert(6 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = xdice_parse("10+1");
    assert(10 == dice.count);
    assert(1 == dice.sides);
    assert(1 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = xdice_parse("1d6+1");
    assert(1 == dice.count);
    assert(6 == dice.sides);
    assert(1 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = xdice_parse("2d4-1");
    assert(2 == dice.count);
    assert(4 == dice.sides);
    assert(-1 == dice.modifier);
    assert(1 == dice.multiplier);
    
    dice = xdice_parse("5*10");
    assert(5 == dice.count);
    assert(1 == dice.sides);
    assert(0 == dice.modifier);
    assert(10 == dice.multiplier);
    
    dice = xdice_parse("1d6*10");
    assert(1 == dice.count);
    assert(6 == dice.sides);
    assert(0 == dice.modifier);
    assert(10 == dice.multiplier);
    
    dice = xdice_parse("5D4X-5");
    assert(5 == dice.count);
    assert(4 == dice.sides);
    assert(0 == dice.modifier);
    assert(-5 == dice.multiplier);
    
    dice = xdice_parse("2d10+4x100");
    assert(2 == dice.count);
    assert(10 == dice.sides);
    assert(4 == dice.modifier);
    assert(100 == dice.multiplier);
}


static void
xdice_roll_test(void)
{
    struct rnd *always_one = rnd_alloc_fake_fixed(0);
    struct rnd *always_two = rnd_alloc_fake_fixed(1);
    int score;

    score = xdice_roll(xdice_make(0, 1), always_one, NULL);
    assert(0 == score);

    score = xdice_roll(xdice_make(3, 1), always_two, NULL);
    assert(3 == score);

    score = xdice_roll(xdice_make_plus(1, 1, 1), always_one, NULL);
    assert(2 == score);

    score = xdice_roll(xdice_make_plus(5, 1, -1), always_two, NULL);
    assert(4 == score);

    score = xdice_roll(xdice_make_plus_times(5, 1, 0, 10), always_one, NULL);
    assert(50 == score);

    score = xdice_roll(xdice_make_plus(0, 4, 1), always_two, NULL);
    assert(1 == score);

    rnd_free(always_one);
    rnd_free(always_two);
}


static void
xdice_roll_and_adjust_upwards_test(void)
{
    struct rnd *ascending = rnd_alloc_fake_ascending(3);
    struct rnd *always_five = rnd_alloc_fake_fixed(4);
    struct rnd *always_six = rnd_alloc_fake_fixed(5);
    int score;

    score = xdice_roll_and_adjust_upwards(xdice_make(3, 6), ascending);
    assert(17 == score);

    score = xdice_roll_and_adjust_upwards(xdice_make(3, 6), always_five);
    assert(18 == score);

    score = xdice_roll_and_adjust_upwards(xdice_make(3, 6), always_six);
    assert(18 == score);

    rnd_free(ascending);
    rnd_free(always_five);
    rnd_free(always_six);
}


static void
xdice_roll_and_drop_lowest_test(void)
{
    struct rnd *ascending = rnd_alloc_fake_ascending(0);
    struct rnd *always_one = rnd_alloc_fake_fixed(0);
    struct rnd *always_two = rnd_alloc_fake_fixed(1);
    int score;
    
    score = xdice_roll_and_drop_lowest(xdice_make(4, 6), ascending);
    assert(9 == score);
    
    score = xdice_roll_and_drop_lowest(xdice_make(3, 6), always_one);
    assert(2 == score);
    
    score = xdice_roll_and_drop_lowest(xdice_make(5, 4), always_two);
    assert(8 == score);
    
    rnd_free(ascending);
    rnd_free(always_one);
    rnd_free(always_two);
}


static void
xdice_roll_with_average_scoring_test(void)
{
    struct rnd *ascending = rnd_alloc_fake_ascending(0);
    struct rnd *always_one = rnd_alloc_fake_fixed(0);
    struct rnd *always_six = rnd_alloc_fake_fixed(5);
    int score;

    score = xdice_roll_with_average_scoring(xdice_make(3, 6), ascending);
    assert(8 == score);

    score = xdice_roll_with_average_scoring(xdice_make(3, 6), always_one);
    assert(9 == score);

    score = xdice_roll_with_average_scoring(xdice_make(3, 6), always_six);
    assert(12 == score);

    rnd_free(ascending);
    rnd_free(always_one);
    rnd_free(always_six);
}


static void
xroll_test(void)
{
    struct rnd *always_one = rnd_alloc_fake_fixed(0);
    struct rnd *always_two = rnd_alloc_fake_fixed(1);
    int score;

    /* "roll" a constant number */
    score = xroll("0", always_one);
    assert(0 == score);

    score = xroll("3", always_one);
    assert(3 == score);

    score = xroll("1+1", always_two);
    assert(2 == score);

    score = xroll("5-1", always_two);
    assert(4 == score);

    score = xroll("5*10", always_two);
    assert(50 == score);

    /* roll dice that always roll 1's */
    score = xroll("0d4+1", always_one);
    assert(1 == score);

    score = xroll("1D6+2", always_one);
    assert(3 == score);

    score = xroll("2d10+1", always_one);
    assert(3 == score);

    score = xroll("3d4+5", always_one);
    assert(8 == score);

    score = xroll("1d12*5", always_one);
    assert(5 == score);

    score = xroll("2d20+1*10", always_one);
    assert(30 == score);

    /* roll dice that always roll 2's */
    score = xroll("1D6-2", always_two);
    assert(0 == score);

    score = xroll("2d10-2", always_two);
    assert(2 == score);

    score = xroll("3d4-3", always_two);
    assert(3 == score);

    score = xroll("2d8*5", always_two);
    assert(20 == score);

    score = xroll("2d20+1*10", always_two);
    assert(50 == score);

    rnd_free(always_one);
    rnd_free(always_two);
}


void
xdice_test(void)
{
    xdice_is_valid_test();
    xdice_make_test();
    xdice_make_plus_test();
    xdice_make_plus_times_test();
    xdice_max_base_score_test();
    xdice_max_score_test();
    xdice_parse_test();
    xdice_roll_test();
    xdice_roll_and_adjust_upwards_test();
    xdice_roll_and_drop_lowest_test();
    xdice_roll_with_average_scoring_test();
    xroll_test();
}

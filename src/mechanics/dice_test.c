#include <assert.h>
#include <limits.h>
#include <string.h>

#include <base/base.h>
#include <mechanics/mechanics.h>


void
dice_test(void);


static void
dice_alloc_base_range_description_test(void)
{
    char *description = dice_alloc_base_range_description(dice_make(0, 6));
    assert(0 == strcmp("0", description));
    free_or_die(description);

    description = dice_alloc_base_range_description(dice_make_plus(0, 6, 2));
    assert(0 == strcmp("2", description));
    free_or_die(description);

    description = dice_alloc_base_range_description(dice_make_plus_times(0, 4, 4, 10));
    assert(0 == strcmp("4", description));
    free_or_die(description);

    description = dice_alloc_base_range_description(dice_make(3, 1));
    assert(0 == strcmp("3", description));
    free_or_die(description);

    description = dice_alloc_base_range_description(dice_make(3, 6));
    assert(0 == strcmp("3-18", description));
    free_or_die(description);

    description = dice_alloc_base_range_description(dice_make_plus(2, 8, 1));
    assert(0 == strcmp("3-17", description));
    free_or_die(description);

    description = dice_alloc_base_range_description(dice_make_plus(2, 4, -1));
    assert(0 == strcmp("1-7", description));
    free_or_die(description);

    description = dice_alloc_base_range_description(dice_make_plus_times(1, 10, 0, 10));
    assert(0 == strcmp("1-10", description));
    free_or_die(description);

    description = dice_alloc_base_range_description(dice_make_plus_times(1, 4, 1, 10000));
    assert(0 == strcmp("2-5", description));
    free_or_die(description);
}


static void
dice_alloc_description_test(void)
{
    char *description = dice_alloc_description(dice_make(0, 6));
    assert(0 == strcmp("0", description));
    free_or_die(description);

    description = dice_alloc_description(dice_make(5, 1));
    assert(0 == strcmp("5", description));
    free_or_die(description);

    description = dice_alloc_description(dice_make(3, 6));
    assert(0 == strcmp("3d6", description));
    free_or_die(description);

    description = dice_alloc_description(dice_make_plus(2, 8, 1));
    assert(0 == strcmp("2d8+1", description));
    free_or_die(description);

    description = dice_alloc_description(dice_make_plus(2, 4, -1));
    assert(0 == strcmp("2d4-1", description));
    free_or_die(description);

    description = dice_alloc_description(dice_make_plus_times(1, 10, 0, 10));
    assert(0 == strcmp("1d10x10", description));
    free_or_die(description);

    description = dice_alloc_description(dice_make_plus_times(1, 4, 1, 10000));
    assert(0 == strcmp("1d4+1x10000", description));
    free_or_die(description);
}


static void
dice_alloc_range_description_test(void)
{
    char *description = dice_alloc_range_description(dice_make(0, 6));
    assert(0 == strcmp("0", description));
    free_or_die(description);

    description = dice_alloc_range_description(dice_make_plus(0, 6, 2));
    assert(0 == strcmp("2", description));
    free_or_die(description);

    description = dice_alloc_range_description(dice_make_plus_times(0, 4, 4, 10));
    assert(0 == strcmp("40", description));
    free_or_die(description);

    description = dice_alloc_range_description(dice_make(3, 1));
    assert(0 == strcmp("3", description));
    free_or_die(description);

    description = dice_alloc_range_description(dice_make(3, 6));
    assert(0 == strcmp("3-18", description));
    free_or_die(description);

    description = dice_alloc_range_description(dice_make_plus(2, 8, 1));
    assert(0 == strcmp("3-17", description));
    free_or_die(description);

    description = dice_alloc_range_description(dice_make_plus(2, 4, -1));
    assert(0 == strcmp("1-7", description));
    free_or_die(description);

    description = dice_alloc_range_description(dice_make_plus_times(1, 10, 0, 10));
    assert(0 == strcmp("10-100", description));
    free_or_die(description);

    description = dice_alloc_range_description(dice_make_plus_times(1, 4, 1, 10000));
    assert(0 == strcmp("20000-50000", description));
    free_or_die(description);
}


static void
dice_has_constant_score_test(void)
{
    assert( ! dice_has_constant_score(dice_make(2, 6)));

    assert(dice_has_constant_score(dice_make(0, 6)));
    assert(dice_has_constant_score(dice_make(2, 1)));
}


static void
dice_is_valid_test(void)
{
    assert( ! dice_is_valid(dice_make(-1, 6)));
    assert( ! dice_is_valid(dice_make(2, 0)));
    assert( ! dice_is_valid(dice_make(INT_MAX / 6 + 1, 6)));

    assert(dice_is_valid(dice_make(1, 2)));
    assert(dice_is_valid(dice_make(2, 6)));
    assert(dice_is_valid(dice_make(INT_MAX / 6, 6)));
}


static void
dice_make_test(void)
{
    struct dice dice = dice_make(2, 6);

    assert(2 == dice.count);
    assert(6 == dice.sides);
    assert(0 == dice.modifier);
    assert(1 == dice.multiplier);
}


static void
dice_make_plus_test(void)
{
    struct dice dice = dice_make_plus(2, 6, 2);

    assert(2 == dice.count);
    assert(6 == dice.sides);
    assert(2 == dice.modifier);
    assert(1 == dice.multiplier);
}


static void
dice_make_plus_times_test(void)
{
    struct dice dice = dice_make_plus_times(2, 6, 2, 10);

    assert(2 == dice.count);
    assert(6 == dice.sides);
    assert(2 == dice.modifier);
    assert(10 == dice.multiplier);
}


static void
dice_max_base_score_test(void)
{
    struct dice dice = dice_make_plus(2, 4, 10);
    assert(18 == dice_max_base_score(dice));
}


static void
dice_max_score_test(void)
{
    struct dice dice = dice_make_plus_times(2, 4, 10, 100);
    assert(1800 == dice_max_score(dice));
}


static void
dice_min_base_score_test(void)
{
    struct dice dice = dice_make_plus(2, 4, 10);
    assert(12 == dice_min_base_score(dice));
}


static void
dice_min_score_test(void)
{
    struct dice dice = dice_make_plus_times(2, 4, 10, 100);
    assert(1200 == dice_min_score(dice));
}


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
    
    dice = dice_parse("0d4+2");
    assert(0 == dice.count);
    assert(4 == dice.sides);
    assert(2 == dice.modifier);
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
    
    dice = dice_parse("5D4X-5");
    assert(5 == dice.count);
    assert(4 == dice.sides);
    assert(0 == dice.modifier);
    assert(-5 == dice.multiplier);
    
    dice = dice_parse("2d10+4x100");
    assert(2 == dice.count);
    assert(10 == dice.sides);
    assert(4 == dice.modifier);
    assert(100 == dice.multiplier);
}


static void
dice_roll_test(void)
{
    struct rnd *always_one = rnd_alloc_fake_fixed(0);
    struct rnd *always_two = rnd_alloc_fake_fixed(1);
    int score;

    score = dice_roll(dice_make(0, 1), always_one, NULL);
    assert(0 == score);

    score = dice_roll(dice_make(3, 1), always_two, NULL);
    assert(3 == score);

    score = dice_roll(dice_make_plus(1, 1, 1), always_one, NULL);
    assert(2 == score);

    score = dice_roll(dice_make_plus(5, 1, -1), always_two, NULL);
    assert(4 == score);

    score = dice_roll(dice_make_plus_times(5, 1, 0, 10), always_one, NULL);
    assert(50 == score);

    score = dice_roll(dice_make_plus(0, 4, 1), always_two, NULL);
    assert(1 == score);

    rnd_free(always_one);
    rnd_free(always_two);
}


static void
dice_roll_and_adjust_upwards_test(void)
{
    struct rnd *ascending = rnd_alloc_fake_ascending(3);
    struct rnd *always_five = rnd_alloc_fake_fixed(4);
    struct rnd *always_six = rnd_alloc_fake_fixed(5);
    int score;

    score = dice_roll_and_adjust_upwards(dice_make(3, 6), ascending);
    assert(17 == score);

    score = dice_roll_and_adjust_upwards(dice_make(3, 6), always_five);
    assert(18 == score);

    score = dice_roll_and_adjust_upwards(dice_make(3, 6), always_six);
    assert(18 == score);

    rnd_free(ascending);
    rnd_free(always_five);
    rnd_free(always_six);
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
roll_test(void)
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
    assert(1 == score);

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


void
dice_test(void)
{
    dice_alloc_base_range_description_test();
    dice_alloc_description_test();
    dice_alloc_range_description_test();
    dice_has_constant_score_test();
    dice_is_valid_test();
    dice_make_test();
    dice_make_plus_test();
    dice_make_plus_times_test();
    dice_max_base_score_test();
    dice_max_score_test();
    dice_min_base_score_test();
    dice_min_score_test();
    dice_parse_test();
    dice_roll_test();
    dice_roll_and_adjust_upwards_test();
    dice_roll_and_drop_lowest_test();
    dice_roll_with_average_scoring_test();
    roll_test();
}

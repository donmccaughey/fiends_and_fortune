#include <stdlib.h>
#include <base/base.h>
#include <character/character.h>


void
abilities_test(void);

void
ability_scores_test(void);

void
ability_sets_test(void);

void
characteristic_generation_method_test(void);

void
characteristic_rolls_test(void);


int
main(int argc, char *argv[])
{
    abilities_test();
    ability_scores_test();
    ability_sets_test();
    characteristic_generation_method_test();
    characteristic_rolls_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}

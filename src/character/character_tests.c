#include <stdlib.h>
#include <base/base.h>
#include <character/character.h>


void
abilities_test(void);

void
characteristic_generation_method_test(void);

void
characteristic_rolls_test(void);

void
characteristic_scores_test(void);

void
characteristics_sets_test(void);


int
main(int argc, char *argv[])
{
    abilities_test();
    characteristic_generation_method_test();
    characteristic_rolls_test();
    characteristic_scores_test();
    characteristics_sets_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}

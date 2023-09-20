#include <stdlib.h>
#include <base/base.h>
#include <character/character.h>


void
abilities_test(void);

void
ability_score_generation_method_test(void);

void
ability_scores_test(void);

void
ability_sets_test(void);


int
main(int argc, char *argv[])
{
    abilities_test();
    ability_scores_test();
    ability_sets_test();
    ability_score_generation_method_test();
    return EXIT_SUCCESS;
}

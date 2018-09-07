#include <stdlib.h>
#include <base/base.h>
#include <mechanics/mechanics.h>
#include <treasure/treasure.h>


void
coins_test(void);

void
gem_test(void);

void
jewelry_test(void);

void
magic_item_test(void);

void
treasure_test(void);


int
main(int argc, char *argv[])
{
    coins_test();
    gem_test();
    jewelry_test();
    magic_item_test();
    treasure_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}

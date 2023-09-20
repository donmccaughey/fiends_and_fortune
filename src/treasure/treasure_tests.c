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

void
treasure_map_test(void);

void
treasure_type_test(void);


int
main(int argc, char *argv[])
{
    coins_test();
    gem_test();
    jewelry_test();
    magic_item_test();
    treasure_test();
    treasure_map_test();
    treasure_type_test();
    return EXIT_SUCCESS;
}

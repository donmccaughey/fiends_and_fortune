#include <stdlib.h>
#include <base/base.h>


void
box_test(void);

void
dungeon_test(void);

void
generator_test(void);

void
point_test(void);

void
size_test(void);

void
text_rectangle_test(void);

void
tile_test(void);


int
main(int argc, char *argv[])
{
    box_test();
    dungeon_test();
    generator_test();
    point_test();
    size_test();
    text_rectangle_test();
    tile_test();
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}

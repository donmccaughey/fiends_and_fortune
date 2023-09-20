#include <stdlib.h>
#include <base/base.h>


void
area_test(void);

void
box_test(void);

void
digger_test(void);

void
dungeon_test(void);

void
generator_test(void);

void
level_map_test(void);

void
point_test(void);

void
size_test(void);

void
text_rectangle_test(void);

void
tile_test(void);

void
tiles_thumbnail_test(void);


int
main(int argc, char *argv[])
{
    area_test();
    box_test();
    digger_test();
    dungeon_test();
    generator_test();
    level_map_test();
    point_test();
    size_test();
    text_rectangle_test();
    tile_test();
    tiles_thumbnail_test();
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>

#include "common/alloc_or_die.h"

void
dice_test(void);

void
dungeon_test(void);

void
point_test(void);

void
points_test(void);

void
range_test(void);

void
reverse_range_test(void);

void
str_test(void);

void
tiles_test(void);


int main (int argc, const char *argv[])
{
    FILE *out = stdout;
    fprintf(out, "Fiends and Fortune unit tests\n");
    
    dice_test();
    dungeon_test();
    point_test();
    points_test();
    range_test();
    reverse_range_test();
    str_test();
    tiles_test();
    
    fprintf(out, "Okay\n");
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}

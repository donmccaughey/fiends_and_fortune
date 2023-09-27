#include "area.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


void
area_test(void);


static void
area_alloc_test(void)
{
    struct box box = box_make(point_make(1, 2, 3), size_make(4, 5, 1));
    struct area *area = area_alloc(area_type_room, direction_north, box);

    assert(1 == area->box.origin.x);
    assert(2 == area->box.origin.y);
    assert(3 == area->box.origin.z);

    assert(4 == area->box.size.width);
    assert(5 == area->box.size.length);
    assert(1 == area->box.size.height);

    assert(direction_north == area->direction);
    assert(area_features_none == area->features);
    assert(area_type_room == area->type);

    area_free(area);
}


static void
area_alloc_description_test(void)
{
    struct box box = box_make(point_make(5, 6, 7), size_make(3, 4, 1));
    struct area *area = area_alloc(area_type_chamber, direction_north, box);
    char *description;

    // chamber

    area->type = area_type_chamber;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' x 40' chamber", description));
    free(description);

    // intersection

    area->type = area_type_intersection;
    description = area_alloc_description(area);
    assert(0 == strcmp("intersection", description));
    free(description);

    // passages

    area->type = area_type_passage;
    description = area_alloc_description(area);
    assert(0 == strcmp("40' passage north to south", description));
    free(description);

    area->direction = direction_south;
    description = area_alloc_description(area);
    assert(0 == strcmp("40' passage north to south", description));
    free(description);

    area->direction = direction_east;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' passage east to west", description));
    free(description);

    area->direction = direction_west;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' passage east to west", description));
    free(description);

    // room

    area->type = area_type_room;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' x 40' room", description));
    free(description);

    // stairs down

    area->type = area_type_stairs_down;
    description = area_alloc_description(area);
    assert(0 == strcmp("stairs down to level 8", description));
    free(description);

    // stairs up

    area->type = area_type_stairs_up;
    description = area_alloc_description(area);
    assert(0 == strcmp("stairs up to level 6", description));
    free(description);

    area->box.origin.z = 1;
    description = area_alloc_description(area);
    assert(0 == strcmp("stairs up to surface", description));
    free(description);

    // unknown

    area->type = area_type_unknown;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' x 40' area", description));
    free(description);

    // with chimney up

    area->features = area_features_chimney_up;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' x 40' area, chimney up to surface", description));
    free(description);

    area->box.origin.z = 7;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' x 40' area, chimney up to level 6", description));
    free(description);

    // with chimney down

    area->features = area_features_chimney_down;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' x 40' area, chimney down to level 8", description));
    free(description);

    // with chute entrance

    area->features = area_features_chute_entrance;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' x 40' area, chute down to level 8", description));
    free(description);

    // with chute exit

    area->features = area_features_chute_exit;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' x 40' area, chute down from level 6", description));
    free(description);

    area->box.origin.z = 1;
    description = area_alloc_description(area);
    assert(0 == strcmp("30' x 40' area, chute down from surface", description));
    free(description);

    // with all features

    area->features = area_features_chimney_up | area_features_chimney_down
            | area_features_chute_entrance | area_features_chute_exit;
    description = area_alloc_description(area);
    char const *expected = "30' x 40' area, "
                           "chimney up to surface, "
                           "chimney down to level 2, "
                           "chute down to level 2, "
                           "chute down from surface";
    assert(0 == strcmp(expected, description));
    free(description);

    area_free(area);
}


static void
area_is_chamber_or_room_test(void)
{
    struct box box = box_make(point_make(5, 6, 7), size_make(3, 4, 1));
    struct area *area = area_alloc(area_type_chamber, direction_north, box);

    assert(area_is_chamber_or_room(area));

    area->type = area_type_room;
    assert(area_is_chamber_or_room(area));

    area->type = area_type_passage;
    assert( ! area_is_chamber_or_room(area));

    area_free(area);
}


static void
area_is_interesting_test(void)
{
    struct box box = box_make(point_make(5, 6, 7), size_make(3, 4, 1));
    struct area *area = area_alloc(area_type_chamber, direction_north, box);

    assert(area_is_interesting(area));

    area->type = area_type_room;
    assert(area_is_interesting(area));

    area->type = area_type_stairs_down;
    assert(area_is_interesting(area));

    area->type = area_type_stairs_up;
    assert(area_is_interesting(area));

    area->type = area_type_passage;
    assert( ! area_is_chamber_or_room(area));

    area->features = area_features_chimney_up;
    assert(area_is_interesting(area));

    area_free(area);
}


static void
area_is_level_transition_test(void)
{
    struct box box = box_make(point_make(5, 6, 7), size_make(3, 4, 1));
    struct area *area = area_alloc(area_type_stairs_down, direction_north, box);

    assert(area_is_level_transition(area));

    area->type = area_type_stairs_up;
    assert(area_is_level_transition(area));

    area->type = area_type_chamber;
    assert( ! area_is_level_transition(area));

    area->features = area_features_chimney_up;
    assert(area_is_level_transition(area));

    area_free(area);
}


static void
area_center_point_test(void)
{
    struct box box = box_make(point_make(10, 20, 30), size_make(5, 5, 1));
    struct area *area = area_alloc(area_type_chamber, direction_north, box);

    assert(point_equals(point_make(12, 22, 30), area_center_point(area)));

    area->box.size.width = 4;
    area->box.size.length = 6;
    area->box.size.height = 2;
    assert(point_equals(point_make(11, 22, 30), area_center_point(area)));

    area->box.size.width = 3;
    area->box.size.length = 7;
    area->box.size.height = 3;
    assert(point_equals(point_make(11, 23, 31), area_center_point(area)));

    area->box.origin.x = 20;
    area->box.origin.y = 30;
    area->box.origin.z = 40;
    assert(point_equals(point_make(21, 33, 41), area_center_point(area)));

    area_free(area);
}


void
area_test(void)
{
    area_alloc_test();
    area_alloc_description_test();
    area_is_chamber_or_room_test();
    area_is_interesting_test();
    area_is_level_transition_test();
    area_center_point_test();
}

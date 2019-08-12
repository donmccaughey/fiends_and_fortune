#include "tiles_thumbnail.h"

#include <base/base.h>
#include "box.h"
#include "text_rectangle.h"
#include "tile.h"


char *
tiles_thumbnail_types_alloc(struct tile **tiles, int tiles_count)
{
    if (!tiles_count) return str_alloc_empty();

    struct box box = box_make_unit(tiles[0]->point);
    for (int i = 0; i < tiles_count; ++i) {
        box = box_extend_to_include_point(box, tiles[i]->point);
    }

    int column_count = 3 * (1 + box.size.width);
    int row_count = 1 + box.size.length;
    struct text_rectangle *text_rectangle = text_rectangle_alloc(column_count, row_count);

    for (int i = 0; i < box.size.width; ++i) {
        int column_index = 3 * (1 + i);
        int row_index = 0;
        text_rectangle_move_to(text_rectangle, column_index, row_index);
        int x = box.origin.x + i;
        text_rectangle_print_format(text_rectangle, "%2i ", x);
    }

    for (int j = 0; j < box.size.length; ++j) {
        int column_index = 0;
        int row_index = 1 + j;
        text_rectangle_move_to(text_rectangle, column_index, row_index);
        int y = box.origin.y + box.size.length - 1 - j;
        text_rectangle_print_format(text_rectangle, "%2i ", y);
    }

    for (int i = 0; i < tiles_count; ++i) {
        struct tile *tile = tiles[i];
        int column_index = 3 * (1 + tile->point.x - box.origin.x);
        int row_index = 1 + (box.size.length - 1) - (tile->point.y - box.origin.y);
        text_rectangle_move_to(text_rectangle, column_index, row_index);
        char type;
        switch (tile->type) {
            case tile_type_filled:      type = ':'; break;
            case tile_type_empty:       type = '.'; break;
            case tile_type_stairs_down: type = 'v'; break;
            case tile_type_stairs_up:   type = '^'; break;
            default:                    type = ' '; break;
        }
        text_rectangle_print_format(text_rectangle, " %c ", type);
    }

    char *thumbnail = strdup_or_die(text_rectangle->chars);
    text_rectangle_free(text_rectangle);
    return thumbnail;
}

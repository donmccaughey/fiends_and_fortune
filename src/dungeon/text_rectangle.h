#ifndef FNF_DUNGEON_TEXT_RECTANGLE_H_INCLUDED
#define FNF_DUNGEON_TEXT_RECTANGLE_H_INCLUDED


struct text_rectangle {
    char *chars;
    int column_count;
    int row_count;
    struct {
        int column_index;
        int row_index;
    } caret;
};


struct text_rectangle *
text_rectangle_alloc(int column_count, int row_count);

void
text_rectangle_free(struct text_rectangle *text_rectangle);

void
text_rectangle_clear(struct text_rectangle *text_rectangle);

void
text_rectangle_home(struct text_rectangle *text_rectangle);

void
text_rectangle_move_to(struct text_rectangle *text_rectangle,
                       int column_index,
                       int row_index);

void
text_rectangle_next_row(struct text_rectangle *text_rectangle);

void
text_rectangle_print_format(struct text_rectangle *text_rectangle,
                            char const *format,
                            ...);

char *
text_rectangle_row_at(struct text_rectangle *text_rectangle, int row_index);

char *
text_rectangle_row_end_at(struct text_rectangle *text_rectangle, int row_index);


#endif

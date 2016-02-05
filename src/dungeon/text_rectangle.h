#ifndef FNF_DUNGEON_TEXT_RECTANGLE_H_INCLUDED
#define FNF_DUNGEON_TEXT_RECTANGLE_H_INCLUDED


struct text_rectangle {
    char *chars;
    int column_count;
    int row_count;
};


struct text_rectangle *
text_rectangle_alloc(int column_count, int row_count);

void
text_rectangle_free(struct text_rectangle *text_rectangle);

void
text_rectangle_clear(struct text_rectangle *text_rectangle);

int
text_rectangle_print_format_at(struct text_rectangle *text_rectangle,
                               int column_index,
                               int row_index,
                               char const *format,
                               ...);

char *
text_rectangle_row_at(struct text_rectangle *text_rectangle, int row_index);

char *
text_rectangle_row_end_at(struct text_rectangle *text_rectangle, int row_index);


#endif

#include "text_rectangle.h"

#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <base/base.h>


static int
horizontal_char_count(int column_count)
{
    return column_count + 1;
}


static size_t
chars_size(int column_count, int row_count)
{
    return array_size_or_die(horizontal_char_count(column_count), row_count)
         + 1;
}


struct text_rectangle *
text_rectangle_alloc(int column_count, int row_count)
{
    struct text_rectangle *text_rectangle = calloc_or_die(1, sizeof(struct text_rectangle));
    text_rectangle->chars = xmalloc(chars_size(column_count, row_count));
    text_rectangle->column_count = column_count;
    text_rectangle->row_count = row_count;
    text_rectangle_clear(text_rectangle);
    return text_rectangle;
}


void
text_rectangle_clear(struct text_rectangle *text_rectangle)
{
    text_rectangle_fill(text_rectangle, ' ');
}


void
text_rectangle_fill(struct text_rectangle *text_rectangle, char ch)
{
    size_t size = chars_size(text_rectangle->column_count,
                             text_rectangle->row_count);
    memset(text_rectangle->chars, ch, size - 1);

    assert(size - 1 < INT_MAX);
    int last_index = (int)(size - 1);
    text_rectangle->chars[last_index] = '\0';

    for (int i = 0; i < text_rectangle->row_count; ++i) {
        *text_rectangle_row_end_at(text_rectangle, i) = '\n';
    }

    text_rectangle_home(text_rectangle);
}


void
text_rectangle_free(struct text_rectangle *text_rectangle)
{
    if (text_rectangle) {
        free(text_rectangle->chars);
        free(text_rectangle);
    }
}


void
text_rectangle_home(struct text_rectangle *text_rectangle)
{
    text_rectangle_move_to(text_rectangle, 0, 0);
}


void
text_rectangle_move_to(struct text_rectangle *text_rectangle,
                       int column_index,
                       int row_index)
{
    assert(column_index >= 0 && column_index < text_rectangle->column_count);
    assert(row_index >= 0 && row_index < text_rectangle->row_count);
    text_rectangle->caret.column_index = column_index;
    text_rectangle->caret.row_index = row_index;
}


void
text_rectangle_next_row(struct text_rectangle *text_rectangle)
{
    text_rectangle_move_to(text_rectangle, 0, text_rectangle->caret.row_index + 1);
}


void
text_rectangle_print_format(struct text_rectangle *text_rectangle,
                            char const *format,
                            ...)
{
    int chars_available = text_rectangle->column_count - text_rectangle->caret.column_index;
    if (chars_available <= 0) return;

    char *buffer;
    va_list arguments;
    va_start(arguments, format);
    int chars_printed = vasprintf_or_die(&buffer, format, arguments);
    va_end(arguments);
    
    int chars_to_copy = chars_printed > chars_available ? chars_available : chars_printed;
    char *chars = text_rectangle_row_at(text_rectangle, text_rectangle->caret.row_index)
                + text_rectangle->caret.column_index;
    memcpy(chars, buffer, chars_to_copy);
    free(buffer);

    text_rectangle->caret.column_index += chars_printed;
}


char *
text_rectangle_row_at(struct text_rectangle *text_rectangle, int row_index)
{
    assert(row_index >= 0 && row_index <= text_rectangle->row_count);
    return text_rectangle->chars
         + (horizontal_char_count(text_rectangle->column_count) * row_index);
}


char *
text_rectangle_row_end_at(struct text_rectangle *text_rectangle, int row_index)
{
    assert(row_index >= 0 && row_index <= text_rectangle->row_count);
    return text_rectangle->chars
         + (horizontal_char_count(text_rectangle->column_count) * row_index)
         + text_rectangle->column_count;
}

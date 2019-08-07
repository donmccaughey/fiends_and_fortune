#include "text_rectangle.h"

#include <assert.h>


void
text_rectangle_test(void);


static void
text_rectangle_alloc_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 20);
    assert(text_rectangle);
    assert(text_rectangle->chars);
    assert(10 == text_rectangle->column_count);
    assert(20 == text_rectangle->row_count);
    assert(0 == text_rectangle->caret.column_index);
    assert(0 == text_rectangle->caret.row_index);
    text_rectangle_free(text_rectangle);
}


void
text_rectangle_test(void)
{
    text_rectangle_alloc_test();
}

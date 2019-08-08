#include "text_rectangle.h"

#include <assert.h>
#include <string.h>

void
text_rectangle_test(void);


static void
text_rectangle_alloc_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    assert(text_rectangle);
    assert(text_rectangle->chars);
    assert(0 == strcmp("          \n          \n          \n", text_rectangle->chars));
    assert(10 == text_rectangle->column_count);
    assert(3 == text_rectangle->row_count);
    assert(0 == text_rectangle->caret.column_index);
    assert(0 == text_rectangle->caret.row_index);

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_clear_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    text_rectangle_print_format(text_rectangle, "Hello, text_rectangle!");
    text_rectangle_next_row(text_rectangle);
    text_rectangle_next_row(text_rectangle);
    text_rectangle_print_format(text_rectangle, "Goodbye, text_rectangle!");
    assert(0 == strcmp("Hello, tex\n          \nGoodbye, t\n", text_rectangle->chars));

    text_rectangle_clear(text_rectangle);

    assert(0 == strcmp("          \n          \n          \n", text_rectangle->chars));

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_home_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    text_rectangle_print_format(text_rectangle, "Hello!");
    assert(0 == strcmp("Hello!    \n          \n          \n", text_rectangle->chars));
    assert(6 == text_rectangle->caret.column_index);
    assert(0 == text_rectangle->caret.row_index);

    text_rectangle_next_row(text_rectangle);
    assert(0 == text_rectangle->caret.column_index);
    assert(1 == text_rectangle->caret.row_index);

    text_rectangle_print_format(text_rectangle, "World!");
    assert(0 == strcmp("Hello!    \nWorld!    \n          \n", text_rectangle->chars));
    assert(6 == text_rectangle->caret.column_index);
    assert(1 == text_rectangle->caret.row_index);

    text_rectangle_home(text_rectangle);
    assert(0 == text_rectangle->caret.column_index);
    assert(0 == text_rectangle->caret.row_index);

    text_rectangle_print_format(text_rectangle, "Goodbye!");
    assert(0 == strcmp("Goodbye!  \nWorld!    \n          \n", text_rectangle->chars));
    assert(8 == text_rectangle->caret.column_index);
    assert(0 == text_rectangle->caret.row_index);

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_move_to_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    assert(0 == strcmp("          \n          \n          \n", text_rectangle->chars));

    text_rectangle_move_to(text_rectangle, 6, 1);
    text_rectangle_print_format(text_rectangle, "1");
    text_rectangle_move_to(text_rectangle, 3, 2);
    text_rectangle_print_format(text_rectangle, "2");
    text_rectangle_move_to(text_rectangle, 9, 0);
    text_rectangle_print_format(text_rectangle, "3");

    assert(0 == strcmp("         3\n      1   \n   2      \n", text_rectangle->chars));
    assert(10 == text_rectangle->caret.column_index);
    assert(0 == text_rectangle->caret.row_index);

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_next_row_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    assert(0 == strcmp("          \n          \n          \n", text_rectangle->chars));

    text_rectangle_move_to(text_rectangle, 4, 0);
    text_rectangle_print_format(text_rectangle, "1");

    text_rectangle_next_row(text_rectangle);
    text_rectangle_print_format(text_rectangle, "2");

    text_rectangle_next_row(text_rectangle);
    text_rectangle_print_format(text_rectangle, "3");

    assert(0 == strcmp("    1     \n2         \n3         \n", text_rectangle->chars));
    assert(1 == text_rectangle->caret.column_index);
    assert(2 == text_rectangle->caret.row_index);

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_print_format_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    assert(0 == strcmp("          \n          \n          \n", text_rectangle->chars));

    text_rectangle_print_format(text_rectangle, "%x", 255);
    assert(0 == strcmp("ff        \n          \n          \n", text_rectangle->chars));

    text_rectangle_print_format(text_rectangle, "%s", "--foo--");
    assert(0 == strcmp("ff--foo-- \n          \n          \n", text_rectangle->chars));

    text_rectangle_print_format(text_rectangle, "%i", 12345);
    assert(0 == strcmp("ff--foo--1\n          \n          \n", text_rectangle->chars));

    text_rectangle_print_format(text_rectangle, "past the end of the line");
    assert(0 == strcmp("ff--foo--1\n          \n          \n", text_rectangle->chars));

    text_rectangle_move_to(text_rectangle, 9, 0);
    text_rectangle_print_format(text_rectangle, "%c", 'X');
    assert(0 == strcmp("ff--foo--X\n          \n          \n", text_rectangle->chars));

    text_rectangle_print_format(text_rectangle, "");
    assert(0 == strcmp("ff--foo--X\n          \n          \n", text_rectangle->chars));

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_row_at_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    text_rectangle_print_format(text_rectangle, "-- row 1 -");
    text_rectangle_next_row(text_rectangle);
    text_rectangle_print_format(text_rectangle, "== row 2 =");
    text_rectangle_next_row(text_rectangle);
    text_rectangle_print_format(text_rectangle, "~~ row 3 ~");
    assert(0 == strcmp("-- row 1 -\n== row 2 =\n~~ row 3 ~\n", text_rectangle->chars));

    char *row0 = text_rectangle_row_at(text_rectangle, 0);
    assert(0 == strncmp("-- row 1 -\n", row0, 11));

    char *row1 = text_rectangle_row_at(text_rectangle, 1);
    assert(0 == strncmp("== row 2 =\n", row1, 11));

    char *row2 = text_rectangle_row_at(text_rectangle, 2);
    assert(0 == strncmp("~~ row 3 ~\n", row2, 11));

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_row_end_at_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    text_rectangle_print_format(text_rectangle, "-- row 1 -");
    text_rectangle_next_row(text_rectangle);
    text_rectangle_print_format(text_rectangle, "== row 2 =");
    text_rectangle_next_row(text_rectangle);
    text_rectangle_print_format(text_rectangle, "~~ row 3 ~");
    assert(0 == strcmp("-- row 1 -\n== row 2 =\n~~ row 3 ~\n", text_rectangle->chars));

    char *row_end0 = text_rectangle_row_end_at(text_rectangle, 0);
    assert(0 == strncmp("\n== row 2 =", row_end0, 11));

    char *row_end1 = text_rectangle_row_end_at(text_rectangle, 1);
    assert(0 == strncmp("\n~~ row 3 ~", row_end1, 11));

    char *row_end2 = text_rectangle_row_end_at(text_rectangle, 2);
    assert(0 == strcmp("\n", row_end2));

    text_rectangle_free(text_rectangle);
}


void
text_rectangle_test(void)
{
    text_rectangle_alloc_test();
    text_rectangle_clear_test();
    text_rectangle_home_test();
    text_rectangle_move_to_test();
    text_rectangle_next_row_test();
    text_rectangle_print_format_test();
    text_rectangle_row_at_test();
    text_rectangle_row_end_at_test();
}

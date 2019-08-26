#include "text_rectangle.h"

#include <assert.h>
#include <string.h>
#include <base/base.h>


void
text_rectangle_test(void);


static void
text_rectangle_alloc_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    char const *expected;
    
    assert(text_rectangle);
    assert(text_rectangle->chars);
    expected =
            "          \n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));
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
    char const *expected;

    expected =
            "Hello, tex\n"
            "          \n"
            "Goodbye, t\n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_clear(text_rectangle);

    expected =
            "          \n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_fill_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    char const *expected;

    expected =
            "          \n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_fill(text_rectangle, '.');

    expected =
            "..........\n"
            "..........\n"
            "..........\n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_home_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    text_rectangle_print_format(text_rectangle, "Hello!");
    char const *expected;
    
    expected =
            "Hello!    \n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));
    assert(6 == text_rectangle->caret.column_index);
    assert(0 == text_rectangle->caret.row_index);

    text_rectangle_next_row(text_rectangle);
    assert(0 == text_rectangle->caret.column_index);
    assert(1 == text_rectangle->caret.row_index);

    text_rectangle_print_format(text_rectangle, "World!");
    expected = 
            "Hello!    \n"
            "World!    \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));
    assert(6 == text_rectangle->caret.column_index);
    assert(1 == text_rectangle->caret.row_index);

    text_rectangle_home(text_rectangle);
    assert(0 == text_rectangle->caret.column_index);
    assert(0 == text_rectangle->caret.row_index);

    text_rectangle_print_format(text_rectangle, "Goodbye!");
    expected =
            "Goodbye!  \n"
            "World!    \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));
    assert(8 == text_rectangle->caret.column_index);
    assert(0 == text_rectangle->caret.row_index);

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_move_to_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    char const *expected;

    expected = 
            "          \n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_move_to(text_rectangle, 6, 1);
    text_rectangle_print_format(text_rectangle, "1");
    text_rectangle_move_to(text_rectangle, 3, 2);
    text_rectangle_print_format(text_rectangle, "2");
    text_rectangle_move_to(text_rectangle, 9, 0);
    text_rectangle_print_format(text_rectangle, "3");

    expected =
            "         3\n"
            "      1   \n"
            "   2      \n";
    assert(str_eq(expected, text_rectangle->chars));
    assert(10 == text_rectangle->caret.column_index);
    assert(0 == text_rectangle->caret.row_index);

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_next_row_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    char const *expected;

    expected = 
            "          \n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_move_to(text_rectangle, 4, 0);
    text_rectangle_print_format(text_rectangle, "1");

    text_rectangle_next_row(text_rectangle);
    text_rectangle_print_format(text_rectangle, "2");

    text_rectangle_next_row(text_rectangle);
    text_rectangle_print_format(text_rectangle, "3");

    expected =
            "    1     \n"
            "2         \n"
            "3         \n";
    assert(str_eq(expected, text_rectangle->chars));
    assert(1 == text_rectangle->caret.column_index);
    assert(2 == text_rectangle->caret.row_index);

    text_rectangle_free(text_rectangle);
}


static void
text_rectangle_print_format_test(void)
{
    struct text_rectangle *text_rectangle = text_rectangle_alloc(10, 3);
    char const *expected;

    expected = 
            "          \n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_print_format(text_rectangle, "%x", 255);
    expected = 
            "ff        \n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_print_format(text_rectangle, "%s", "--foo--");
    expected = 
            "ff--foo-- \n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_print_format(text_rectangle, "%i", 12345);
    expected = 
            "ff--foo--1\n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_print_format(text_rectangle, "past the end of the line");
    expected = 
            "ff--foo--1\n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_move_to(text_rectangle, 9, 0);
    text_rectangle_print_format(text_rectangle, "%c", 'X');
    expected = 
            "ff--foo--X\n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

    text_rectangle_print_format(text_rectangle, "");
    expected =
            "ff--foo--X\n"
            "          \n"
            "          \n";
    assert(str_eq(expected, text_rectangle->chars));

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
    char const *expected;

    expected =
            "-- row 1 -\n"
            "== row 2 =\n"
            "~~ row 3 ~\n";
    assert(str_eq(expected, text_rectangle->chars));

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
    char const *expected;

    expected =
            "-- row 1 -\n"
            "== row 2 =\n"
            "~~ row 3 ~\n";
    assert(str_eq(expected, text_rectangle->chars));

    char *row_end0 = text_rectangle_row_end_at(text_rectangle, 0);
    assert(0 == strncmp("\n== row 2 =", row_end0, 11));

    char *row_end1 = text_rectangle_row_end_at(text_rectangle, 1);
    assert(0 == strncmp("\n~~ row 3 ~", row_end1, 11));

    char *row_end2 = text_rectangle_row_end_at(text_rectangle, 2);
    assert(str_eq("\n", row_end2));

    text_rectangle_free(text_rectangle);
}


void
text_rectangle_test(void)
{
    text_rectangle_alloc_test();
    text_rectangle_clear_test();
    text_rectangle_fill_test();
    text_rectangle_home_test();
    text_rectangle_move_to_test();
    text_rectangle_next_row_test();
    text_rectangle_print_format_test();
    text_rectangle_row_at_test();
    text_rectangle_row_end_at_test();
}

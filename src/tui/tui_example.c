#include <base/base.h>
#include <tui/tui.h>


int
main(int argc, char *argv[])
{
    struct app *app = app_alloc();

    struct view *menu_view = menu_view_alloc("Select an Item");
    menu_view_add_item(menu_view, "Try Item One");
    menu_view_add_item(menu_view, "Item Two Is Better");
    menu_view_add_item(menu_view, "You'll Love Item Three");

    app_add_view(app, menu_view);

    struct result result = app_run(app);
    if (!result_is_success(result)) {
        result_print_error(result);
    }

    app_free(app);

    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}

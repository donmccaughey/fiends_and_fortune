#include <stdlib.h>
#include <base/base.h>
#include "game.h"


int
main(int argc, char *argv[])
{
    struct game *game = game_alloc_or_die(global_rnd);
    struct result result = game_show(game);
    if (result_is_success(result)) result = game_run(game);
    game_hide(game);
    if (!result_is_success(result)) result_print_error(result);
    game_free_or_die(game);
}

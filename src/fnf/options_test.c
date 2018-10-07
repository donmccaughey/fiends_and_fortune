#include <assert.h>
#include <stddef.h>
#include <base/base.h>
#include "options.h"


static void
options_alloc_with_no_args_test(void)
{
    char *argv[] = {
            "/usr/local/bin/fnf",
    };
    int argc = ARRAY_COUNT(argv);
    struct options *options = options_alloc(argc, argv);

    assert(action_none == options->action);
    assert(str_eq("fnf", options->command_name));
    assert( ! options->debug);
    assert( ! options->dungeon_options);
    assert(options->error);
    assert( ! options->help);
    assert(output_format_text == options->output_format);
    assert(options->rnd);
    assert( ! options->verbose);

    options_free(options);
}


void
options_test(void)
{
    options_alloc_with_no_args_test();
}

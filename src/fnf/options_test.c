#include <assert.h>
#include <stddef.h>
#include <base/base.h>
#include <character/character.h>
#include "options.h"


static void
reset_getopt(void)
{
    optreset = 1;
    optind = 1;
}


static void
options_alloc_with_no_args_test(void)
{
    reset_getopt();
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


static void
options_alloc_with_no_action_test(void)
{
    reset_getopt();
    char *argv[] = {
        "/usr/local/bin/fnf",
        "-d",
        "-j", "12345",
        "--format=json",
        "-v",
    };
    int argc = ARRAY_COUNT(argv);
    struct options *options = options_alloc(argc, argv);

    assert(action_none == options->action);
    assert(str_eq("fnf", options->command_name));
    assert(options->debug);
    assert( ! options->dungeon_options);
    assert(options->error);
    assert( ! options->help);
    assert(output_format_json == options->output_format);
    assert(options->rnd);
    assert(options->verbose);

    options_free(options);
}


static void
options_alloc_with_character_action_test(void)
{
    reset_getopt();
    char *argv[] = {
        "/usr/local/bin/fnf",
        "character",
    };
    int argc = ARRAY_COUNT(argv);
    struct options *options = options_alloc(argc, argv);

    assert(action_character == options->action);
    assert(ability_score_generation_method_simple == options->character_method);

    assert(str_eq("fnf", options->command_name));
    assert( ! options->debug);
    assert( ! options->dungeon_options);
    assert( ! options->error);
    assert( ! options->help);
    assert(output_format_text == options->output_format);
    assert(options->rnd);
    assert( ! options->verbose);

    options_free(options);
}


static void
options_alloc_with_check_action_test(void)
{
    reset_getopt();
    char *argv[] = {
        "/usr/local/bin/fnf",
        "check",
    };
    int argc = ARRAY_COUNT(argv);
    struct options *options = options_alloc(argc, argv);

    assert(action_check == options->action);
    assert(0 == options->check_constant);

    assert(str_eq("fnf", options->command_name));
    assert( ! options->debug);
    assert( ! options->dungeon_options);
    assert( ! options->error);
    assert( ! options->help);
    assert(output_format_text == options->output_format);
    assert(options->rnd);
    assert( ! options->verbose);

    options_free(options);
}


static void
options_alloc_with_dungeon_action_test(void)
{
    reset_getopt();
    char *argv[] = {
        "/usr/local/bin/fnf",
        "dungeon",
    };
    int argc = ARRAY_COUNT(argv);
    struct options *options = options_alloc(argc, argv);

    assert(action_dungeon == options->action);
    assert( ! options->dungeon_type_small);

    assert(str_eq("fnf", options->command_name));
    assert( ! options->debug);
    assert(options->dungeon_options);
    assert( ! options->error);
    assert( ! options->help);
    assert(output_format_text == options->output_format);
    assert(options->rnd);
    assert( ! options->verbose);

    options_free(options);
}


static void
options_alloc_with_each_action_test(void)
{
    reset_getopt();
    char *argv[] = {
        "/usr/local/bin/fnf",
        "each",
    };
    int argc = ARRAY_COUNT(argv);
    struct options *options = options_alloc(argc, argv);

    assert(action_each == options->action);

    assert(str_eq("fnf", options->command_name));
    assert( ! options->debug);
    assert( ! options->dungeon_options);
    assert( ! options->error);
    assert( ! options->help);
    assert(output_format_text == options->output_format);
    assert(options->rnd);
    assert( ! options->verbose);

    options_free(options);
}


static void
options_alloc_with_magic_action_test(void)
{
    reset_getopt();
    char *argv[] = {
        "/usr/local/bin/fnf",
        "magic",
    };
    int argc = ARRAY_COUNT(argv);
    struct options *options = options_alloc(argc, argv);

    assert(action_magic == options->action);
    assert(10 == options->magic_count);

    assert(str_eq("fnf", options->command_name));
    assert( ! options->debug);
    assert( ! options->dungeon_options);
    assert( ! options->error);
    assert( ! options->help);
    assert(output_format_text == options->output_format);
    assert(options->rnd);
    assert( ! options->verbose);

    options_free(options);
}


static void
options_alloc_with_map_action_test(void)
{
    reset_getopt();
    char *argv[] = {
        "/usr/local/bin/fnf",
        "map",
    };
    int argc = ARRAY_COUNT(argv);
    struct options *options = options_alloc(argc, argv);

    assert(action_map == options->action);

    assert(str_eq("fnf", options->command_name));
    assert( ! options->debug);
    assert( ! options->dungeon_options);
    assert( ! options->error);
    assert( ! options->help);
    assert(output_format_text == options->output_format);
    assert(options->rnd);
    assert( ! options->verbose);

    options_free(options);
}


static void
options_alloc_with_table_action_test(void)
{
    reset_getopt();
    char *argv[] = {
        "/usr/local/bin/fnf",
        "table",
    };
    int argc = ARRAY_COUNT(argv);
    struct options *options = options_alloc(argc, argv);

    assert(action_table == options->action);

    assert(str_eq("fnf", options->command_name));
    assert( ! options->debug);
    assert( ! options->dungeon_options);
    assert( ! options->error);
    assert( ! options->help);
    assert(output_format_text == options->output_format);
    assert(options->rnd);
    assert( ! options->verbose);

    options_free(options);
}


static void
options_alloc_with_treasure_type_action_test(void)
{
    reset_getopt();
    char *argv[] = {
        "/usr/local/bin/fnf",
        "u",
    };
    int argc = ARRAY_COUNT(argv);
    struct options *options = options_alloc(argc, argv);

    assert(action_treasure == options->action);
    assert('U' == options->treasure_type);

    assert(str_eq("fnf", options->command_name));
    assert( ! options->debug);
    assert( ! options->dungeon_options);
    assert( ! options->error);
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
    options_alloc_with_no_action_test();
    options_alloc_with_character_action_test();
    options_alloc_with_check_action_test();
    options_alloc_with_dungeon_action_test();
    options_alloc_with_each_action_test();
    options_alloc_with_magic_action_test();
    options_alloc_with_map_action_test();
    options_alloc_with_table_action_test();
}

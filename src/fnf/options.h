#ifndef FNF_OPTIONS_H_INCLUDED
#define FNF_OPTIONS_H_INCLUDED


#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#include "action.h"

#include <character/character.h>


enum output_format {
    output_format_text = 0,
    output_format_json,
};


enum option_value {
    option_value_none = 0,

    option_value_debug = 'd',
    option_value_help = 'h',
    option_value_jrand48 = 'j',
    option_value_verbose = 'v',

    option_value_long_only = CHAR_MAX,
    option_value_format,
};


struct dungeon_options;
struct rnd;


struct options {
    enum action action;
    union {
        enum ability_score_generation_method character_method;
        uint32_t check_constant;
        bool dungeon_type_small;
        int magic_count;
        char treasure_type;
    };
    char *command_name;
    bool debug;
    struct dungeon_options *dungeon_options;
    bool error;
    bool help;
    enum output_format output_format;
    struct rnd *rnd;
    bool verbose;
};


struct options *
options_alloc(int argc, char *argv[]);

void
options_free(struct options *options);

void
options_print_usage(struct options const *options);


#endif

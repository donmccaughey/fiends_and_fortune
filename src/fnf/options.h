#ifndef FNF_OPTIONS_H_INCLUDED
#define FNF_OPTIONS_H_INCLUDED


#include <stdbool.h>
#include <stdint.h>

#include "action.h"

#include "character/method.h"


struct rnd;


struct options {
    enum action action;
    union {
        enum characteristic_generation_method character_method;
        uint32_t check_constant;
        bool dungeon_type_small;
        int magic_count;
        char treasure_type;
    };
    char *command_name;
    bool debug;
    bool error;
    bool help;
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

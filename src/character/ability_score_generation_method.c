#include "ability_score_generation_method.h"

#include <string.h>


static struct {
    enum ability_score_generation_method method;
    enum ability_score_generation_type type;
    char const *name;
    char const *description;
} const generation_method[] = {
        {
                .method=ability_score_generation_method_invalid,
                .type=ability_score_generation_type_abilities,
                .name="invalid",
                .description="invalid ability generation method",
        },
        {
                .method=ability_score_generation_method_simple,
                .type=ability_score_generation_type_abilities,
                .name="simple",
                .description="roll 3d6 once per ability",
        },
        {
                .method=ability_score_generation_method_1,
                .type=ability_score_generation_type_scores,
                .name="method1",
                .description="roll 4d6 six times and drop the lowest die from each roll; player assigns rolls to abilities",
        },
        {
                .method=ability_score_generation_method_2,
                .type=ability_score_generation_type_scores,
                .name="method2",
                .description="roll 3d6 twelve times and drop the lowest six rolls; player assigns rolls to abilities",
        },
        {
                .method=ability_score_generation_method_3,
                .type=ability_score_generation_type_abilities,
                .name="method3",
                .description="roll 3d6 six times per ability and keep the highest roll",
        },
        {
                .method=ability_score_generation_method_4,
                .type=ability_score_generation_type_twelve_ability_sets,
                .name="method4",
                .description="generate 12 ability sets using the simple method; player chooses one of the twelve",
        },
        {
                .method=ability_score_generation_method_general_NPC,
                .type=ability_score_generation_type_abilities,
                .name="generalnpc",
                .description="roll 3d6 once per ability; change any die of 1 to 3 or 6 to 4",
        },
        {
                .method=ability_score_generation_method_special_NPC,
                .type=ability_score_generation_type_abilities,
                .name="specialnpc",
                .description="roll 3d6 once per normal ability; for profession abilities, add 1 to each die less than 6",
        },
};
static int generation_method_count = sizeof generation_method
                                   / sizeof generation_method[0];


char const *
ability_score_generation_method_description(enum ability_score_generation_method method)
{
    for (int i = 0; i < generation_method_count; ++i) {
        if (method == generation_method[i].method) {
            return generation_method[i].description;
        }
    }
    return generation_method[0].description;
}


char const *
ability_score_generation_method_name(enum ability_score_generation_method method)
{
    for (int i = 0; i < generation_method_count; ++i) {
        if (method == generation_method[i].method) {
            return generation_method[i].name;
        }
    }
    return generation_method[0].name;
}


enum ability_score_generation_method
ability_score_generation_method_from_string(char const *string)
{
    for (int i = 0; i < generation_method_count; ++i) {
        if (0 == strcasecmp(string, generation_method[i].name)) {
            return generation_method[i].method;
        }
    }
    return generation_method[0].method;
}


enum ability_score_generation_type
ability_score_generation_method_type(enum ability_score_generation_method method)
{
    for (int i = 0; i < generation_method_count; ++i) {
        if (method == generation_method[i].method) {
            return generation_method[i].type;
        }
    }
    return generation_method[0].type;
}

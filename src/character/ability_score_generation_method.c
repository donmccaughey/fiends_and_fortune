#include "ability_score_generation_method.h"

#include <string.h>


static struct {
    enum ability_score_generation_method method;
    char const *string;
} method_strings[] = {
    { ability_score_generation_method_invalid, "invalid" },
    { ability_score_generation_method_simple, "simple" },
    { ability_score_generation_method_1, "method1" },
    { ability_score_generation_method_2, "method2" },
    { ability_score_generation_method_3, "method3" },
    { ability_score_generation_method_4, "method4" },
    { ability_score_generation_method_general_NPC, "generalnpc" },
    { ability_score_generation_method_special_NPC, "specialnpc" },
};
static int method_strings_count = sizeof method_strings
                                / sizeof method_strings[0];


char const *
ability_score_generation_method_description(enum ability_score_generation_method method)
{
    switch (method) {
        case ability_score_generation_method_invalid:
            return "invalid ability generation method";
        case ability_score_generation_method_simple:
            return "roll 3d6 once per ability";
        case ability_score_generation_method_1:
            return "roll 4d6 six times and drop the lowest die from each roll; player assigns rolls to abilities";
        case ability_score_generation_method_2:
            return "roll 3d6 twelve times and drop the lowest six rolls; player assigns rolls to abilities";
        case ability_score_generation_method_3:
            return "roll 3d6 six times per ability and keep the highest roll";
        case ability_score_generation_method_4:
            return "generate 12 ability sets using the simple method; player chooses one of the twelve";
        case ability_score_generation_method_general_NPC:
            return "roll 3d6 once per ability; change any die of 1 to 3 or 6 to 4";
        case ability_score_generation_method_special_NPC:
            return "roll 3d6 once per normal ability; for profession abilities, add 1 to each die less than 6";
    }
    return "invalid ability generation method";
}


char const *
ability_score_generation_method_name(enum ability_score_generation_method method)
{
    for (int i = 0; i < method_strings_count; ++i) {
        if (method == method_strings[i].method) {
            return method_strings[i].string;
        }
    }
    return "invalid";
}


enum ability_score_generation_method
ability_score_generation_method_from_string(char const *string)
{
    for (int i = 0; i < method_strings_count; ++i) {
        if (0 == strcasecmp(string, method_strings[i].string)) {
            return method_strings[i].method;
        }
    }
    return ability_score_generation_method_invalid;
}

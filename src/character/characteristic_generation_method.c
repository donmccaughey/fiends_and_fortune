#include "characteristic_generation_method.h"

#include <string.h>


static struct {
    enum characteristic_generation_method method;
    char const *string;
} method_strings[] = {
    { characteristic_generation_method_invalid, "invalid" },
    { characteristic_generation_method_simple, "simple" },
    { characteristic_generation_method_1, "method1" },
    { characteristic_generation_method_2, "method2" },
    { characteristic_generation_method_3, "method3" },
    { characteristic_generation_method_4, "method4" },
    { characteristic_generation_method_general_NPC, "generalnpc" },
    { characteristic_generation_method_special_NPC, "specialnpc" },
};
static int method_strings_count = sizeof method_strings
                                / sizeof method_strings[0];


char const *
characteristic_generation_method_description(enum characteristic_generation_method method)
{
    switch (method) {
        case characteristic_generation_method_simple:
            return "roll 3d6 once per characteristic";
        case characteristic_generation_method_1:
            return "roll 4d6 six times and drop the lowest die from each roll; player assigns rolls to characteristics";
        case characteristic_generation_method_2:
            return "roll 3d6 twelve times and drop the lowest six rolls; player assigns rolls to characteristics";
        case characteristic_generation_method_3:
            return "roll 3d6 six times per characteristic and keep the highest roll";
        case characteristic_generation_method_4:
            return "generate 12 characters using the simple method; player chooses one of the twelve";
        case characteristic_generation_method_general_NPC:
            return "roll 3d6 once per characteristic; change any die of 1 to 3 or 6 to 4";
        case characteristic_generation_method_special_NPC:
            return "roll 3d6 once per normal characteristic; for profession characteristic(s), add 1 to each die less than 6";
        default: return "unknown character generation method";
    }
}


char const *
characteristic_generation_method_name(enum characteristic_generation_method method)
{
    for (int i = 0; i < method_strings_count; ++i) {
        if (method == method_strings[i].method) {
            return method_strings[i].string;
        }
    }
    return "invalid";
}


enum characteristic_generation_method
characteristic_generation_method_from_string(char const *string)
{
    for (int i = 0; i < method_strings_count; ++i) {
        if (0 == strcasecmp(string, method_strings[i].string)) {
            return method_strings[i].method;
        }
    }
    return characteristic_generation_method_invalid;
}

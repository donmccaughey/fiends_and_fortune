#ifndef FNF_CHARACTERISTIC_GENERATION_METHOD_H_INCLUDED
#define FNF_CHARACTERISTIC_GENERATION_METHOD_H_INCLUDED


enum characteristic_generation_method {
    characteristic_generation_method_invalid=-1,
    characteristic_generation_method_simple,
    characteristic_generation_method_1,
    characteristic_generation_method_2,
    characteristic_generation_method_3,
    characteristic_generation_method_4,
    characteristic_generation_method_general_NPC,
    characteristic_generation_method_special_NPC
};


char const *
characteristic_generation_method_description(enum characteristic_generation_method method);

char const *
characteristic_generation_method_name(enum characteristic_generation_method method);

enum characteristic_generation_method
characteristic_generation_method_from_string(char const *string);


#endif

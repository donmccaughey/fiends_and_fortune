#ifndef FNF_ABILITY_SCORE_GENERATION_METHOD_H_INCLUDED
#define FNF_ABILITY_SCORE_GENERATION_METHOD_H_INCLUDED


enum ability_score_generation_method {
    ability_score_generation_method_invalid=-1,
    ability_score_generation_method_simple,
    ability_score_generation_method_1,
    ability_score_generation_method_2,
    ability_score_generation_method_3,
    ability_score_generation_method_4,
    ability_score_generation_method_general_NPC,
    ability_score_generation_method_special_NPC
};


char const *
ability_score_generation_method_description(enum ability_score_generation_method method);

char const *
ability_score_generation_method_name(enum ability_score_generation_method method);

enum ability_score_generation_method
ability_score_generation_method_from_string(char const *string);


#endif

#include <assert.h>
#include <base/base.h>
#include <character/character.h>


void
ability_score_generation_method_test(void);


static void
ability_score_generation_method_description_test(void)
{
    char const *description = ability_score_generation_method_description(ability_score_generation_method_simple);
    assert(str_eq("roll 3d6 once per ability", description));

    description = ability_score_generation_method_description(-42);
    assert(str_eq("invalid ability generation method", description));
}


static void
ability_score_generation_method_name_test(void)
{
    char const *name = ability_score_generation_method_name(ability_score_generation_method_1);
    assert(str_eq("method1", name));

    name = ability_score_generation_method_name(-42);
    assert(str_eq("invalid", name));
}


static void
ability_score_generation_method_from_string_test(void)
{
    enum ability_score_generation_method method = ability_score_generation_method_from_string("Simple");
    assert(ability_score_generation_method_simple == method);

    method = ability_score_generation_method_from_string("generalNPC");
    assert(ability_score_generation_method_general_NPC == method);

    method = ability_score_generation_method_from_string("foobar");
    assert(ability_score_generation_method_invalid == method);
}


static void
ability_score_generation_method_type_test(void)
{
    enum ability_score_generation_type type;

    type = ability_score_generation_method_type(ability_score_generation_method_invalid);
    assert(ability_score_generation_type_abilities == type);

    type = ability_score_generation_method_type(ability_score_generation_method_simple);
    assert(ability_score_generation_type_abilities == type);

    type = ability_score_generation_method_type(ability_score_generation_method_1);
    assert(ability_score_generation_type_scores == type);

    type = ability_score_generation_method_type(ability_score_generation_method_2);
    assert(ability_score_generation_type_scores == type);

    type = ability_score_generation_method_type(ability_score_generation_method_3);
    assert(ability_score_generation_type_abilities == type);

    type = ability_score_generation_method_type(ability_score_generation_method_4);
    assert(ability_score_generation_type_twelve_ability_sets == type);

    type = ability_score_generation_method_type(ability_score_generation_method_general_NPC);
    assert(ability_score_generation_type_abilities == type);

    type = ability_score_generation_method_type(ability_score_generation_method_special_NPC);
    assert(ability_score_generation_type_abilities == type);
}


void
ability_score_generation_method_test(void)
{
    ability_score_generation_method_description_test();
    ability_score_generation_method_name_test();
    ability_score_generation_method_from_string_test();
    ability_score_generation_method_type_test();
}

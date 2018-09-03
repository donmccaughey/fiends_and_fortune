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


void
ability_score_generation_method_test(void)
{
    ability_score_generation_method_description_test();
    ability_score_generation_method_name_test();
    ability_score_generation_method_from_string_test();
}

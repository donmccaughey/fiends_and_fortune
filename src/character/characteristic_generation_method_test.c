#include <assert.h>
#include <base/base.h>
#include <character/character.h>


void
characteristic_generation_method_test(void);


static void
characteristic_generation_method_description_test(void)
{
    char const *description = characteristic_generation_method_description(characteristic_generation_method_simple);
    assert(str_eq("roll 3d6 once per characteristic", description));

    description = characteristic_generation_method_description(-42);
    assert(str_eq("unknown character generation method", description));
}


static void
characteristic_generation_method_name_test(void)
{
    char const *name = characteristic_generation_method_name(characteristic_generation_method_1);
    assert(str_eq("method1", name));

    name = characteristic_generation_method_name(-42);
    assert(str_eq("invalid", name));
}


static void
characteristic_generation_method_from_string_test(void)
{
    enum characteristic_generation_method method = characteristic_generation_method_from_string("Simple");
    assert(characteristic_generation_method_simple == method);

    method = characteristic_generation_method_from_string("generalNPC");
    assert(characteristic_generation_method_general_NPC == method);

    method = characteristic_generation_method_from_string("foobar");
    assert(characteristic_generation_method_invalid == method);
}


void
characteristic_generation_method_test(void)
{
    characteristic_generation_method_description_test();
    characteristic_generation_method_name_test();
    characteristic_generation_method_from_string_test();
}

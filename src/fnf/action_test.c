#include <assert.h>
#include "action.h"


static void
action_from_string_test(void)
{
    assert(action_invalid == action_from_string(""));
    assert(action_invalid == action_from_string("foobar"));

    assert(action_character == action_from_string("character"));
    assert(action_character == action_from_string("CHARACTER"));

    assert(action_treasure == action_from_string("a"));
    assert(action_treasure == action_from_string("A"));

    assert(action_treasure == action_from_string("z"));
    assert(action_treasure == action_from_string("Z"));
}


void
action_test(void)
{
    action_from_string_test();
}

#include <assert.h>
#include <base/base.h>
#include <magic/magic.h>

#include "astr.h"


void
spell_test(void);


static void
spell_determine_test(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(5);

    char const *spell = spell_determine(rnd, spell_type_clerical, 1);
    assert(astr_eq("detect magic", spell));

    spell = spell_determine(rnd, spell_type_magic_user, 5);
    assert(astr_eq("conjure elemental", spell));

    spell = spell_determine(rnd, spell_type_illusionist, 7);
    assert(astr_eq("alter reality", spell));

    rnd_free(rnd);
}


void
spell_test(void)
{
    spell_determine_test();
}

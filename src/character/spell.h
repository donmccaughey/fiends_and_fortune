#ifndef FNF_CHARACTER_SPELL_H_INCLUDED
#define FNF_CHARACTER_SPELL_H_INCLUDED


struct rnd;


enum spell_type {
    spell_type_magic_user,
    spell_type_clerical,
    spell_type_illusionist,
    spell_type_drudical
};


char const *
spell_determine(struct rnd *rnd, enum spell_type spell_type, int spell_level);


#endif

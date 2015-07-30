#ifndef FNF_TREASURE_GEM_H_INCLUDED
#define FNF_TREASURE_GEM_H_INCLUDED


struct rnd;


enum gem_size {
    gem_size_very_small = -2,
    gem_size_small,
    gem_size_average,
    gem_size_large,
    gem_size_very_large,
    gem_size_huge,
};


enum gem_type {
    gem_type_fake = 0,
    gem_type_ornamental_stone,
    gem_type_semi_precious_stone,
    gem_type_fancy_stone,
    gem_type_precious_stone,
    gem_type_gem_stone,
    gem_type_jewel_stone,
};


struct gem {
    enum gem_size size;
    enum gem_type type;
    int value_percent_modifier;
    int value_rank_modifier;
    char *true_description;
};


void
gem_finalize(struct gem *gem);

int
gem_value_in_cp(struct gem *gem);

void
gem_generate(struct gem *gem, struct rnd *rnd);

void
gem_initialize(struct gem *gem);


#endif

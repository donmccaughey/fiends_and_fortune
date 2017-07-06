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
    gem_type_unknown = 0,
    gem_type_ornamental_stone,
    gem_type_semi_precious_stone,
    gem_type_fancy_stone,
    gem_type_precious_stone,
    gem_type_gem_stone,
    gem_type_jewel_stone,
};


enum gem_kind {
    gem_kind_unknown = 0,
    /* ornamental */
    gem_kind_azurite,
    gem_kind_banded_agate,
    gem_kind_blue_quartz,
    gem_kind_eye_agate,
    gem_kind_hematite,
    gem_kind_lapis_lazuli,
    gem_kind_malachite,
    gem_kind_moss_agate,
    gem_kind_obsidian,
    gem_kind_rhodochrosite,
    gem_kind_tiger_eye,
    gem_kind_turquoise,
    /* semi-precious */
    gem_kind_bloodstone,
    gem_kind_carnelian,
    gem_kind_chalcedony,
    gem_kind_chrysoprase,
    gem_kind_citrine,
    gem_kind_jasper,
    gem_kind_moonstone,
    gem_kind_onyx,
    gem_kind_rock_crystal,
    gem_kind_sardonyx,
    gem_kind_smoky_quartz,
    gem_kind_star_rose_quartz,
    gem_kind_zircon,
    /* fancy */
    gem_kind_amber,
    gem_kind_alexandrite,
    gem_kind_amethyst,
    gem_kind_chrysoberyl,
    gem_kind_coral,
    gem_kind_jade,
    gem_kind_jet,
    gem_kind_tourmaline,
    /* fancy or precious */
    gem_kind_garnet,
    gem_kind_pearl,
    gem_kind_spinel,
    /* precious */
    gem_kind_aquamarine,
    gem_kind_peridot,
    gem_kind_topaz,
    /* gem */
    gem_kind_black_opal,
    gem_kind_emerald,
    gem_kind_fire_opal,
    gem_kind_opal,
    gem_kind_oriental_amethyst,
    gem_kind_oriental_topaz,
    gem_kind_sapphire,
    gem_kind_star_ruby,
    gem_kind_star_sapphire,
    /* jewel */
    gem_kind_black_sapphire,
    gem_kind_diamond,
    gem_kind_jacinth,
    gem_kind_oriental_emerald,
    gem_kind_ruby,
};

enum gem_opacity {
    gem_opacity_transparent = 0,
    gem_opacity_translucent,
    gem_opacity_opaque,
};


struct gem {
    enum gem_size size;
    enum gem_type type;
    enum gem_kind kind;
    char *colors;
    int value_percent_modifier;
    int value_rank_modifier;
    char *true_description;
    char *visible_description;
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

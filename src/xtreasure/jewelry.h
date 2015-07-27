#ifndef FNF_TREASURE_JEWELRY_H_INCLUDED
#define FNF_TREASURE_JEWELRY_H_INCLUDED


#include <stdbool.h>


struct rnd;


enum jewelry_form {
    jewelry_form_anklet = 0,
    jewelry_form_armband,
    jewelry_form_belt,
    jewelry_form_box,
    jewelry_form_bracelet,
    jewelry_form_brooch,
    jewelry_form_buckle,
    jewelry_form_chain,
    jewelry_form_chalice,
    jewelry_form_choker,
    jewelry_form_clasp,
    jewelry_form_coffer,
    jewelry_form_collar,
    jewelry_form_comb,
    jewelry_form_coronet,
    jewelry_form_crown,
    jewelry_form_decanter,
    jewelry_form_diadem,
    jewelry_form_earring,
    jewelry_form_fob,
    jewelry_form_goblet,
    jewelry_form_headband,
    jewelry_form_idol,
    jewelry_form_locket,
    jewelry_form_medal,
    jewelry_form_medallion,
    jewelry_form_necklace,
    jewelry_form_pendant,
    jewelry_form_pin,
    jewelry_form_orb,
    jewelry_form_ring,
    jewelry_form_sceptre,
    jewelry_form_seal,
    jewelry_form_statuette,
    jewelry_form_tiara
};


enum jewelry_material {
    jewelry_material_fake = 0,
    jewelry_material_ivory,
    jewelry_material_silver,
    jewelry_material_silver_and_gold,
    jewelry_material_gold,
    jewelry_material_jade,
    jewelry_material_coral,
    jewelry_material_platinum,
    jewelry_material_silver_with_gems,
    jewelry_material_gold_with_gems,
    jewelry_material_platinum_with_gems
};


struct jewelry {
    bool has_gems;
    enum jewelry_form form;
    enum jewelry_material material;
    int workmanship_bonus;
    int exceptional_stone_bonus;
    int value_in_cp;
    char *true_description;
};


void
jewelry_finalize(struct jewelry *jewelry);

void
jewelry_generate(struct jewelry *jewelry, struct rnd *rnd);

void
jewelry_initialize(struct jewelry *jewelry);

int
jewelry_value_in_cp(struct jewelry *jewelry);


#endif

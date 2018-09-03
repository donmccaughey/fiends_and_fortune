#ifndef FNF_TREASURE_COINS_H_INCLUDED
#define FNF_TREASURE_COINS_H_INCLUDED


#include <stdbool.h>


struct coins {
    int pp;
    int gp;
    int ep;
    int sp;
    int cp;
};


char *
coins_alloc_description(struct coins coins);

char *
coins_alloc_gp_cp_description(int cp);

bool
coins_is_zero(struct coins coins);

struct coins
coins_make(int pp, int gp, int ep, int sp, int cp);

struct coins
coins_make_from_cp(int cp);

struct coins
coins_make_from_gp_cp(int gp, int cp);

struct coins
coins_make_zero(void);

struct coins
coins_normalize(struct coins coins);

struct coins
coins_normalize_to_gp_cp(struct coins coins);

int
coins_to_cp(struct coins coins);


///// core upward conversions

struct coins
coins_cp_to_sp(struct coins coins);

inline struct coins
coins_sp_to_ep(struct coins coins);

struct coins
coins_ep_to_gp(struct coins coins);

struct coins
coins_gp_to_pp(struct coins coins);


///// core downward conversions

struct coins
coins_pp_to_gp(struct coins coins);

inline struct coins
coins_gp_to_ep(struct coins coins);

struct coins
coins_ep_to_sp(struct coins coins);

struct coins
coins_sp_to_cp(struct coins coins);


#endif

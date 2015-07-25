#ifndef COINAGE_H_INCLUDED
#define COINAGE_H_INCLUDED


#include <stdbool.h>


///// core downward conversions

#define pp_to_gp(pp) (5 * (pp))
#define gp_to_ep(gp) (2 * (gp))
#define ep_to_sp(ep) (10 * (ep))
#define sp_to_cp(sp) (10 * (sp))


///// derived downward conversions

#define pp_to_ep(pp) (gp_to_ep(pp_to_gp((pp))))
#define pp_to_sp(pp) (ep_to_sp(pp_to_ep((pp))))
#define pp_to_cp(pp) (sp_to_cp(pp_to_sp((pp))))

#define gp_to_sp(gp) (ep_to_sp(gp_to_ep((gp))))
#define gp_to_cp(gp) (sp_to_cp(gp_to_sp((gp))))

#define ep_to_cp(ep) (sp_to_cp(ep_to_sp((ep))))


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

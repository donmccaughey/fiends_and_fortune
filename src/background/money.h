#ifndef FNF_BACKGROUND_MONEY_H
#define FNF_BACKGROUND_MONEY_H


///// core downward conversions
///// 200cp == 20sp == 2ep == 1gp == 1/5pp
///// 1000cp == 100sp == 10ep == 5gp == 1pp

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


#endif

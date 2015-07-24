#ifndef COINAGE_H_INCLUDED
#define COINAGE_H_INCLUDED


#define CP_PER_GP  200


char *goldFormat_cp(int cp);


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


#endif

#ifndef FNF_CHARACTER_ABILITY_SCORES_H_INCLUDED
#define FNF_CHARACTER_ABILITY_SCORES_H_INCLUDED


struct rnd;


struct ability_scores {
    int values[6];
};


struct ability_scores *
ability_scores_alloc_method_1(struct rnd *rnd);

struct ability_scores *
ability_scores_alloc_method_2(struct rnd *rnd);

void
ability_scores_free(struct ability_scores *scores);


#endif

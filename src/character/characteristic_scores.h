#ifndef FNF_CHARACTER_CHARACTERISTIC_SCORES_H_INCLUDED
#define FNF_CHARACTER_CHARACTERISTIC_SCORES_H_INCLUDED


struct rnd;


struct characteristic_scores {
    int values[6];
};


struct characteristic_scores *
characteristic_scores_alloc_method_1(struct rnd *rnd);

struct characteristic_scores *
characteristic_scores_alloc_method_2(struct rnd *rnd);

void
characteristic_scores_free(struct characteristic_scores *scores);


#endif


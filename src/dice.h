#ifndef DICE_H_INCLUDED
#define DICE_H_INCLUDED


struct rnd;


struct dice {
    int count;
    int sides;
    int modifier;
    int multiplier;
};


inline struct dice
dice_make(int count, int sides)
{
    return (struct dice){
        .count=count,
        .sides=sides,
        .modifier=0,
        .multiplier=1,
    };
}

int
dice_max_score(char const *dice_string);

int
dice_min_score(char const *dice_string);

struct dice
dice_parse(char const *dice_string);

int
dice_roll(struct dice dice, struct rnd *rnd, int die_scores[]);

int
roll(struct rnd *rnd, char const *dice_string);

int
dice_roll_with_average_scoring(struct dice dice, struct rnd *rnd);

int
rollDiceAndAdjustUpwards(struct rnd *rnd, int count, int sides);

int
rollDiceAndDropLowest(struct rnd *rnd, int count, int sides);


#endif /* #ifndef DICE_H_INCLUDED */

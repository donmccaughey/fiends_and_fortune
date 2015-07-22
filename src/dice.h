#ifndef DICE_H_INCLUDED
#define DICE_H_INCLUDED


struct rnd;


struct dice {
    int count;
    int sides;
    int modifier;
    int multiplier;
};


int
maxDieRoll(char const *dieRollString);

int
minDieRoll(char const *dieRollString);

struct dice
parseDieRoll(char const *dieRollString);

int
roll(struct rnd *rnd, char const *dieRollString);

int
rollDice(struct rnd *rnd, int count, int sides);

int
rollDiceAndAdjustTowardsAverage(struct rnd *rnd, int count, int sides);

int
rollDiceAndAdjustUpwards(struct rnd *rnd, int count, int sides);

int
rollDiceAndDropLowest(struct rnd *rnd, int count, int sides);

int
rollDicePlus(struct rnd *rnd, int count, int sides, int modifier);


#endif /* #ifndef DICE_H_INCLUDED */

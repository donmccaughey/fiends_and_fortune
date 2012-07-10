#ifndef DICE_H_INCLUDED
#define DICE_H_INCLUDED


#include <stdint.h>


struct Dice {
  unsigned short state[3];
  uint32_t (*nextRandomNumber)(struct Dice *dice);
};


struct DieRoll {
  int count;
  int sides;
  int modifier;
  int multiplier;
};


void initializeDice(struct Dice *dice);

int maxDieRoll(char const *dieRollString);

int minDieRoll(char const *dieRollString);

struct DieRoll parseDieRoll(char const *dieRollString);

int roll(struct Dice *dice, char const *dieRollString);

int rollDice(struct Dice *dice, int count, int sides);

int rollDiceAndAdjustTowardsAverage(struct Dice *dice, int count, int sides);

int rollDiceAndAdjustUpwards(struct Dice *dice, int count, int sides);

int rollDiceAndDropLowest(struct Dice *dice, int count, int sides);

int rollDicePlus(struct Dice *dice, int count, int sides, int modifier);


#endif /* #ifndef DICE_H_INCLUDED */

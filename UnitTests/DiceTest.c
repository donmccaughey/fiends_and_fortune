#include <assert.h>
#include <Dice.h>


void diceTest(void);
static uint32_t nextAscending(struct Dice *dice);
static uint32_t nextOne(struct Dice *dice);
static uint32_t nextZero(struct Dice *dice);
static void parseDieRollTest(void);
static void rollTest(void);
static void rollDiceTest(void);
static void rollDiceAndAdjustTowardsAverageTest(void);
static void rollDiceAndDropLowestTest(void);
static void rollDicePlusTest(void);


void diceTest(void) {
  parseDieRollTest();
  rollTest();
  rollDiceTest();
  rollDiceAndAdjustTowardsAverageTest();
  rollDiceAndDropLowestTest();
  rollDicePlusTest();
}


static uint32_t nextAscending(struct Dice *dice)
{
  return (uint32_t) dice->state[0]++;
}


static uint32_t nextOne(struct Dice *dice)
{
  return 1;
}


static uint32_t nextZero(struct Dice *dice)
{
  return 0;
}


static void parseDieRollTest(void)
{
  struct DieRoll dieRoll;
  
  dieRoll = parseDieRoll("0");
  assert(0 == dieRoll.count);
  assert(1 == dieRoll.sides);
  assert(0 == dieRoll.modifier);
  assert(1 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("1");
  assert(1 == dieRoll.count);
  assert(1 == dieRoll.sides);
  assert(0 == dieRoll.modifier);
  assert(1 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("5");
  assert(5 == dieRoll.count);
  assert(1 == dieRoll.sides);
  assert(0 == dieRoll.modifier);
  assert(1 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("1d6");
  assert(1 == dieRoll.count);
  assert(6 == dieRoll.sides);
  assert(0 == dieRoll.modifier);
  assert(1 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("2D6");
  assert(2 == dieRoll.count);
  assert(6 == dieRoll.sides);
  assert(0 == dieRoll.modifier);
  assert(1 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("10+1");
  assert(10 == dieRoll.count);
  assert(1 == dieRoll.sides);
  assert(1 == dieRoll.modifier);
  assert(1 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("1d6+1");
  assert(1 == dieRoll.count);
  assert(6 == dieRoll.sides);
  assert(1 == dieRoll.modifier);
  assert(1 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("2d4-1");
  assert(2 == dieRoll.count);
  assert(4 == dieRoll.sides);
  assert(-1 == dieRoll.modifier);
  assert(1 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("5*10");
  assert(5 == dieRoll.count);
  assert(1 == dieRoll.sides);
  assert(0 == dieRoll.modifier);
  assert(10 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("1d6*10");
  assert(1 == dieRoll.count);
  assert(6 == dieRoll.sides);
  assert(0 == dieRoll.modifier);
  assert(10 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("5d4*-5");
  assert(5 == dieRoll.count);
  assert(4 == dieRoll.sides);
  assert(0 == dieRoll.modifier);
  assert(-5 == dieRoll.multiplier);
  
  dieRoll = parseDieRoll("2d10+4*100");
  assert(2 == dieRoll.count);
  assert(10 == dieRoll.sides);
  assert(4 == dieRoll.modifier);
  assert(100 == dieRoll.multiplier);
}


static void rollTest(void)
{
  struct Dice alwaysRollOnes;
  alwaysRollOnes.nextRandomNumber = nextZero;
  struct Dice alwaysRollTwos;
  alwaysRollTwos.nextRandomNumber = nextOne;
  int dieRoll;
  
  /* "roll" a constant number */
  dieRoll = roll(&alwaysRollOnes, "0");
  assert(0 == dieRoll);
  
  dieRoll = roll(&alwaysRollOnes, "3");
  assert(3 == dieRoll);
  
  dieRoll = roll(&alwaysRollTwos, "1+1");
  assert(2 == dieRoll);
  
  dieRoll = roll(&alwaysRollTwos, "5-1");
  assert(4 == dieRoll);
  
  dieRoll = roll(&alwaysRollTwos, "5*10");
  assert(50 == dieRoll);
  
  /* roll dice that always roll 1's */
  dieRoll = roll(&alwaysRollOnes, "0d4+1");
  assert(0 == dieRoll);
  
  dieRoll = roll(&alwaysRollOnes, "1D6+2");
  assert(3 == dieRoll);
  
  dieRoll = roll(&alwaysRollOnes, "2d10+1");
  assert(3 == dieRoll);
  
  dieRoll = roll(&alwaysRollOnes, "3d4+5");
  assert(8 == dieRoll);
  
  dieRoll = roll(&alwaysRollOnes, "1d12*5");
  assert(5 == dieRoll);
  
  dieRoll = roll(&alwaysRollOnes, "2d20+1*10");
  assert(30 == dieRoll);
  
  /* roll dice that always roll 2's */
  dieRoll = roll(&alwaysRollTwos, "1D6-2");
  assert(0 == dieRoll);
  
  dieRoll = roll(&alwaysRollTwos, "2d10-2");
  assert(2 == dieRoll);
  
  dieRoll = roll(&alwaysRollTwos, "3d4-3");
  assert(3 == dieRoll);
  
  dieRoll = roll(&alwaysRollTwos, "2d8*5");
  assert(20 == dieRoll);
  
  dieRoll = roll(&alwaysRollTwos, "2d20+1*10");
  assert(50 == dieRoll);
}


static void rollDiceTest(void)
{
  struct Dice alwaysRollOnes;
  alwaysRollOnes.nextRandomNumber = nextZero;
  struct Dice alwaysRollTwos;
  alwaysRollTwos.nextRandomNumber = nextOne;
  int dieRoll;
  
  /* "roll" a constant number */
  dieRoll = rollDice(&alwaysRollTwos, 1, 1);
  assert(1 == dieRoll);
  
  dieRoll = rollDice(&alwaysRollTwos, 5, 1);
  assert(5 == dieRoll);
  
  /* roll dice that always roll 1's */
  dieRoll = rollDice(&alwaysRollOnes, 1, 6);
  assert(1 == dieRoll);
  
  dieRoll = rollDice(&alwaysRollOnes, 2, 10);
  assert(2 == dieRoll);
  
  dieRoll = rollDice(&alwaysRollOnes, 3, 4);
  assert(3 == dieRoll);
  
  /* roll dice that always roll 2's */
  dieRoll = rollDice(&alwaysRollTwos, 1, 6);
  assert(2 == dieRoll);
  
  dieRoll = rollDice(&alwaysRollTwos, 2, 10);
  assert(4 == dieRoll);
  
  dieRoll = rollDice(&alwaysRollTwos, 3, 4);
  assert(6 == dieRoll);
}


static void rollDiceAndAdjustTowardsAverageTest(void)
{
  struct Dice ascendingRolls = {
    .nextRandomNumber = nextAscending
  };
  struct Dice alwaysRollOnes = {
    .nextRandomNumber = nextZero
  };
  struct Dice alwaysRollTwos = {
    .nextRandomNumber = nextOne
  };
  int dieRoll;
  
  dieRoll = rollDiceAndAdjustTowardsAverage(&ascendingRolls, 3, 6);
  assert(8 == dieRoll);
  
  dieRoll = rollDiceAndAdjustTowardsAverage(&alwaysRollOnes, 3, 6);
  assert(9 == dieRoll);
  
  dieRoll = rollDiceAndAdjustTowardsAverage(&alwaysRollTwos, 3, 6);
  assert(6 == dieRoll);
}


static void rollDiceAndDropLowestTest(void)
{
  struct Dice ascendingRolls = {
    .nextRandomNumber = nextAscending
  };
  struct Dice alwaysRollOnes = {
    .nextRandomNumber = nextZero
  };
  struct Dice alwaysRollTwos = {
    .nextRandomNumber = nextOne
  };
  int dieRoll;
  
  dieRoll = rollDiceAndDropLowest(&ascendingRolls, 4, 6);
  assert(9 == dieRoll);
  
  dieRoll = rollDiceAndDropLowest(&alwaysRollOnes, 3, 6);
  assert(2 == dieRoll);
  
  dieRoll = rollDiceAndDropLowest(&alwaysRollTwos, 5, 4);
  assert(8 == dieRoll);
}


static void rollDicePlusTest(void)
{
  struct Dice alwaysRollOnes;
  alwaysRollOnes.nextRandomNumber = nextZero;
  struct Dice alwaysRollTwos;
  alwaysRollTwos.nextRandomNumber = nextOne;
  int dieRoll;
  
  /* "roll" a constant number */
  dieRoll = rollDicePlus(&alwaysRollTwos, 1, 1, 1);
  assert(2 == dieRoll);
  
  dieRoll = rollDicePlus(&alwaysRollTwos, 5, 1, -1);
  assert(4 == dieRoll);
  
  /* roll dice that always roll 1's */
  dieRoll = rollDicePlus(&alwaysRollOnes, 1, 6, 2);
  assert(3 == dieRoll);
  
  dieRoll = rollDicePlus(&alwaysRollOnes, 2, 10, 1);
  assert(3 == dieRoll);
  
  dieRoll = rollDicePlus(&alwaysRollOnes, 3, 4, 5);
  assert(8 == dieRoll);
  
  /* roll dice that always roll 2's */
  dieRoll = rollDicePlus(&alwaysRollTwos, 1, 6, -2);
  assert(0 == dieRoll);
  
  dieRoll = rollDicePlus(&alwaysRollTwos, 2, 10, -2);
  assert(2 == dieRoll);
  
  dieRoll = rollDicePlus(&alwaysRollTwos, 3, 4, -3);
  assert(3 == dieRoll);
}

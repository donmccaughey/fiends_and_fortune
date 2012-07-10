#include "Gem.h"

#include <stdlib.h>
#include <string.h>

#include "coinage.h"
#include "Dice.h"
#include "heap.h"


static char const *const gemSizeNames[] = {
  "very small",
  "small",
  "average",
  "large",
  "very large",
  "huge"
};

static char const *const gemTypeNames[] = {
  "fake stone",
  "ornamental stone",
  "semi-precious stone",
  "fancy stone",
  "precious stone",
  "gem stone",
  "jewel stone"
};


static int const gemValuesByRank_cp[] = {
  0, 
  1 * CP_PER_SP,
  5 * CP_PER_SP,
  10 * CP_PER_SP,
  1 * CP_PER_GP,
  5 * CP_PER_GP,
  10 * CP_PER_GP,
  50 * CP_PER_GP,
  100 * CP_PER_GP,
  500 * CP_PER_GP,
  1000 * CP_PER_GP,
  5000 * CP_PER_GP,
  10000 * CP_PER_GP,
  25000 * CP_PER_GP,
  50000 * CP_PER_GP,
  100000 * CP_PER_GP,
  250000 * CP_PER_GP,
  500000 * CP_PER_GP,
  1000000 * CP_PER_GP
};

static size_t const gemValuesByRank_cpCount = sizeof gemValuesByRank_cp 
                                            / sizeof(gemValuesByRank_cp[0]);


static char *describeGem(struct Gem *gem);

static char *describeGemModifiers(struct Gem *gem);

static char const *sizeNameForGem(struct Gem *gem);

static char const *typeNameForGem(struct Gem *gem);


static char *describeGem(struct Gem *gem)
{
  char *description;
  ASPRINTF_OR_DIE(&description, "%s %s", 
                  sizeNameForGem(gem), typeNameForGem(gem))
  return description;
}


static char *describeGemModifiers(struct Gem *gem)
{
  char *rankModifier = NULL;
  char *percentModifier = NULL;
  
  if (gem->valueRankModifier) {
    ASPRINTF_OR_DIE(&rankModifier, "rank %+i", gem->valueRankModifier);
  }
  if (gem->valuePercentModifier) {
    ASPRINTF_OR_DIE(&percentModifier, "%+i%%", gem->valuePercentModifier - 100);
  }
  
  char *description;
  if (rankModifier && percentModifier) {
    ASPRINTF_OR_DIE(&description, "%s, %s", rankModifier, percentModifier);
  } else if (rankModifier || percentModifier) {
    ASPRINTF_OR_DIE(&description, "%s",
                    rankModifier ? rankModifier : percentModifier);
  } else {
    description = STRDUP_OR_DIE("");
  }
  
  free(percentModifier);
  free(rankModifier);
  return description;
}


void finalizeGem(struct Gem *gem)
{
  free(gem->trueDescription);
}


int gemValue_cp(struct Gem *gem)
{
  int rank = gem->type + gem->size + gem->valueRankModifier;
  if (rank < 1) {
    rank = 1;
  } else if (rank >= gemValuesByRank_cpCount) {
    rank = (int) gemValuesByRank_cpCount - 1;
  }
  
  int value_cp = gemValuesByRank_cp[rank];
  if (gem->valuePercentModifier) {
    value_cp = (value_cp * gem->valuePercentModifier) / 100;
  }
  return value_cp;
}


void generateGem(struct Gem *gem, struct Dice *dice)
{
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 25) {
    gem->type = OrnamentalStoneGemType;
  } else if (dieRoll <= 50) {
    gem->type = SemiPreciousStoneGemType;
  } else if (dieRoll <= 70) {
    gem->type = FancyStoneGemType;
  } else if (dieRoll <= 90) {
    gem->type = PreciousStoneGemType;
  } else if (dieRoll <= 99) {
    gem->type = GemStoneGemType;
  } else {
    gem->type = JewelStoneGemType;
  }
  
  dieRoll = roll(dice, "1d100");
  if (dieRoll <= 25) {
    gem->size = VerySmallGemSize;
  } else if (dieRoll <= 50) {
    gem->size = SmallGemSize;
  } else if (dieRoll <= 70) {
    gem->size = AverageGemSize;
  } else if (dieRoll <= 90) {
    gem->size = LargeGemSize;
  } else if (dieRoll <= 99) {
    gem->size = VeryLargeGemSize;
  } else {
    gem->size = HugeGemSize;
  }
  
  do {
    dieRoll = roll(dice, "1d10");
    if (dieRoll == 1) {
      if (gem->valueRankModifier >= 0 && gem->valueRankModifier < 7) {
        ++gem->valueRankModifier;
      }
    } else if (dieRoll == 2) {
      gem->valuePercentModifier = 200;
    } else if (dieRoll == 3) {
      gem->valuePercentModifier = 100 + (roll(dice, "1d6") * 10);
    } else if (dieRoll <= 8) {
      /* no change */
    } else if (dieRoll == 9) {
      gem->valuePercentModifier = 100 - (roll(dice, "1d4") * 10);
    } else {
      if (gem->valueRankModifier > -5 && gem->valueRankModifier <= 0) {
        --gem->valueRankModifier;
      }
    }
  } while (   (dieRoll == 1 || dieRoll == 10) 
           && gem->valueRankModifier > -5 && gem->valueRankModifier < 7);
  
  char *description = describeGem(gem);
  char *modifiers = describeGemModifiers(gem);
  int value_cp = gemValue_cp(gem);
  char *value_gp = goldFormat_cp(value_cp);
  
  char const *separator = modifiers[0] ? ": " : "";
  ASPRINTF_OR_DIE(&gem->trueDescription, "%s (%s%s%s)", 
                  description, modifiers, separator, value_gp);
  
  free(value_gp);
  free(modifiers);
  free(description);
}


void intializeGem(struct Gem *gem)
{
  memset(gem, 0, sizeof(struct Gem));
}


static char const *sizeNameForGem(struct Gem *gem)
{
  int index = gem->size - VerySmallGemSize;
  return gemSizeNames[index];
}


static char const *typeNameForGem(struct Gem *gem)
{
  int index = gem->type - FakeGemType;
  return gemTypeNames[index];
}

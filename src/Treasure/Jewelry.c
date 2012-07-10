#include "Jewelry.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coinage.h"
#include "Dice.h"
#include "heap.h"


struct JewelryFormTable {
  int dieRoll;
  enum JewelryForm form;
  char const *name;
};


struct JewelryRank {
  char const *baseValue;
  int baseValueMultiplier;
  Boolean hasGems;
  enum JewelryMaterial materials[3];
  int materialsCount;
};


static struct JewelryFormTable jewelryFormTable[] = {
  { 2, AnkletJewelryForm, "anklet" },
  { 6, ArmBandJewelryForm, "arm band" },
  { 9, BeltJewelryForm, "belt" },
  { 12, BoxJewelryForm, "small box" },
  { 16, BraceletJewelryForm, "bracelet" },
  { 19, BroochJewelryForm, "brooch" },
  { 21, BuckleJewelryForm, "buckle" },
  { 25, ChainJewelryForm, "chain" },
  { 26, ChaliceJewelryForm, "chalice" },
  { 27, ChokerJewelryForm, "choker" },
  { 30, ClaspJewelryForm, "clasp" },
  { 32, CofferJewelryForm, "coffer" },
  { 33, CollarJewelryForm, "collar" },
  { 35, CombJewelryForm, "comb" },
  { 36, CoronetJewelryForm, "coronet" },
  { 37, CrownJewelryForm, "crown" },
  { 39, DecanterJewelryForm, "decanter" },
  { 40, DiademJewelryForm, "diadem" },
  { 45, EarringJewelryForm, "earring" },
  { 47, FobJewelryForm, "fob" },
  { 52, GobletJewelryForm, "goblet" },
  { 54, HeadbandJewelryForm, "headband" },
  { 57, IdolJewelryForm, "idol" },
  { 59, LocketJewelryForm, "locket" },
  { 62, MedalJewelryForm, "medal" },
  { 68, MedallionJewelryForm, "medallion" },
  { 75, NecklaceJewelryForm, "necklace" },
  { 78, PendantJewelryForm, "pendant" },
  { 83, PinJewelryForm, "pin" },
  { 84, OrbJewelryForm, "orb" },
  { 93, RingJewelryForm, "ring" },
  { 94, SceptreJewelryForm, "sceptre" },
  { 96, SealJewelryForm, "seal" },
  { 99, StatuetteJewelryForm, "statuette" },
  { 100, TiaraJewelryForm, "tiara" }
};

static size_t jewelryFormTableCount = sizeof jewelryFormTable 
                                    / sizeof jewelryFormTable[0];


static char const *const jewelryMaterialFormats[] = {
  "fake %s",
  "ivory %s",
  "silver %s",
  "silver and gold %s",
  "gold %s",
  "jade %s",
  "coral %s",
  "platinum %s",
  "silver %s with gems",
  "gold %s with gems",
  "platinum %s with gems"
};


static struct JewelryRank const jewelryRanks[] = {
  {
    "", 0, FALSE, {}, 0
  },
  {
    "1D10", 100 * CP_PER_GP, FALSE, 
    { 
      IvoryJewelryMaterial, 
      SilverJewelryMaterial
    }, 
    2
  },
  {
    "2D6", 100 * CP_PER_GP, FALSE, 
    { 
      SilverAndGoldJewelryMaterial
    }, 
    1
  },
  {
    "3D6", 100 * CP_PER_GP, FALSE, 
    { 
      GoldJewelryMaterial
    }, 
    1
  },
  {
    "5D6", 100 * CP_PER_GP, FALSE, 
    { 
      JadeJewelryMaterial, 
      CoralJewelryMaterial,
      PlatinumJewelryMaterial
    }, 
    3
  },
  {
    "1D6", 1000 * CP_PER_GP, TRUE, 
    { 
      SilverWithGemsJewelryMaterial
    }, 
    1
  },
  {
    "2D4", 1000 * CP_PER_GP, TRUE, 
    { 
      GoldWithGemsJewelryMaterial
    }, 
    1
  },
  {
    "2D6", 1000 * CP_PER_GP, TRUE, 
    { 
      PlatinumWithGemsJewelryMaterial
    }, 
    1
  }
};

static size_t const jewelryRankCount = sizeof jewelryRanks 
                                     / sizeof jewelryRanks[0];

static int const jewelryMinRank = 1;

static int const jewelryMaxRank = (int) (jewelryRankCount) - 1;


static char *describeJewelry(struct Jewelry *jewelry);

static char *describeJewelryModifiers(struct Jewelry *jewelry);


static char *describeJewelry(struct Jewelry *jewelry)
{
  char const *format = jewelryMaterialFormats[jewelry->material];
  char const *name = jewelryFormTable[jewelry->form].name;
  char *description;
  ASPRINTF_OR_DIE(&description, format, name);
  return description;
}


static char *describeJewelryModifiers(struct Jewelry *jewelry)
{
  char *workmanshipBonus = NULL;
  char *exceptionalStoneBonus = NULL;
  
  if (jewelry->workmanshipBonus) {
    ASPRINTF_OR_DIE(&workmanshipBonus, "workmanship %+i", 
                    jewelry->workmanshipBonus);
  }
  if (jewelry->exceptionalStoneBonus) {
    ASPRINTF_OR_DIE(&exceptionalStoneBonus, "exceptional stone %+i", 
                    jewelry->exceptionalStoneBonus);
  }
  
  char *description;
  if (workmanshipBonus && exceptionalStoneBonus) {
    ASPRINTF_OR_DIE(&description, "%s, %s", 
                    workmanshipBonus, exceptionalStoneBonus);
  } else if (workmanshipBonus || exceptionalStoneBonus) {
    ASPRINTF_OR_DIE(&description, "%s", 
                    workmanshipBonus ? workmanshipBonus : exceptionalStoneBonus);
  } else {
    description = STRDUP_OR_DIE("");
  }
  
  free(exceptionalStoneBonus);
  free(workmanshipBonus);
  return description;
}


void finalizeJewelry(struct Jewelry *jewelry)
{
  free(jewelry->trueDescription);
}


void generateJewelry(struct Jewelry *jewelry, struct Dice *dice)
{
  int rank;
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 10) {
    rank = 1;
  } else if (dieRoll <= 20) {
    rank = 2;
  } else if (dieRoll <= 40) {
    rank = 3;
  } else if (dieRoll <= 50) {
    rank = 4;
  } else if (dieRoll <= 70) {
    rank = 5;
  } else if (dieRoll <= 90) {
    rank = 6;
  } else {
    rank = 7;
  }
  assert(rank >= jewelryMinRank);
  assert(rank <= jewelryMaxRank);
  
  jewelry->hasGems = jewelryRanks[rank].hasGems;
  
  if (jewelryRanks[rank].materialsCount == 1) {
    jewelry->material = jewelryRanks[rank].materials[0];
  } else {
    dieRoll = rollDice(dice, 1, jewelryRanks[rank].materialsCount);
    jewelry->material = jewelryRanks[rank].materials[dieRoll - 1];
  }
  
  dieRoll = roll(dice, jewelryRanks[rank].baseValue);
  jewelry->value_cp = dieRoll * jewelryRanks[rank].baseValueMultiplier;
  
  dieRoll = roll(dice, "1d100");
  for (int i = 0; i < jewelryFormTableCount; ++i) {
    if (dieRoll <= jewelryFormTable[i].dieRoll) {
      jewelry->form = jewelryFormTable[i].form;
      break;
    }
  }
  
  int const maxWorkmanshipBonus = 12;
  do {
    dieRoll = roll(dice, "1d10");
    if (dieRoll == 1) {
      ++jewelry->workmanshipBonus;
    }
  } while (dieRoll == 1 && jewelry->workmanshipBonus < maxWorkmanshipBonus);
  for (int i = 0; i < jewelry->workmanshipBonus; ++i) {
    int maxValue_cp = maxDieRoll(jewelryRanks[rank].baseValue)
                    * jewelryRanks[rank].baseValueMultiplier;
    if (jewelry->value_cp < maxValue_cp) {
      jewelry->value_cp = maxValue_cp;
    } else if (rank < jewelryMaxRank) {
      ++rank;
      dieRoll = roll(dice, jewelryRanks[rank].baseValue);
      jewelry->value_cp = dieRoll * jewelryRanks[rank].baseValueMultiplier;
    }
  }
  
  int const maxExceptionalStoneBonus = 128;
  if (jewelry->hasGems) {
    dieRoll = roll(dice, "1d8");
    if (dieRoll == 1) {
      jewelry->exceptionalStoneBonus = 1;
      do {
        dieRoll = roll(dice, "1d6");
        if (dieRoll == 1) {
          jewelry->exceptionalStoneBonus *= 2;
        }
      } while (   dieRoll == 1 
               && jewelry->exceptionalStoneBonus < maxExceptionalStoneBonus);
    }
    jewelry->value_cp += jewelry->exceptionalStoneBonus * 5000 * CP_PER_GP;
  }
  
  char *description = describeJewelry(jewelry);
  char *modifiers = describeJewelryModifiers(jewelry);
  int value_cp = jewelryValue_cp(jewelry);
  char *value_gp = goldFormat_cp(value_cp);
  
  char const *separator = modifiers[0] ? ": " : "";
  ASPRINTF_OR_DIE(&jewelry->trueDescription, "%s (%s%s%s)", 
                  description, modifiers, separator, value_gp);
  
  free(modifiers);
  free(value_gp);
  free(description);
}


void initializeJewelry(struct Jewelry *jewelry)
{
  memset(jewelry, 0, sizeof(struct Jewelry));
}


int jewelryValue_cp(struct Jewelry *jewelry)
{
  return jewelry->value_cp;
}

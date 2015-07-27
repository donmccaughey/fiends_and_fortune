#include "Jewelry.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc_or_die.h"
#include "coins.h"
#include "dice.h"
#include "str.h"


struct JewelryFormTable {
  int dieRoll;
  enum JewelryForm form;
  char const *name;
};


struct JewelryRank {
  char const *baseValue;
  int baseValueMultiplier;
  bool hasGems;
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
    "", 0, false, {}, 0
  },
  {
    "1D10", gp_to_cp(100), false,
    { 
      IvoryJewelryMaterial, 
      SilverJewelryMaterial
    }, 
    2
  },
  {
    "2D6", gp_to_cp(100), false,
    { 
      SilverAndGoldJewelryMaterial
    }, 
    1
  },
  {
    "3D6", gp_to_cp(100), false,
    { 
      GoldJewelryMaterial
    }, 
    1
  },
  {
    "5D6", gp_to_cp(100), false,
    { 
      JadeJewelryMaterial, 
      CoralJewelryMaterial,
      PlatinumJewelryMaterial
    }, 
    3
  },
  {
    "1D6", gp_to_cp(1000), true,
    { 
      SilverWithGemsJewelryMaterial
    }, 
    1
  },
  {
    "2D4", gp_to_cp(1000), true,
    { 
      GoldWithGemsJewelryMaterial
    }, 
    1
  },
  {
    "2D6", gp_to_cp(1000), true,
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
  asprintf_or_die(&description, format, name);
  return description;
}


static char *describeJewelryModifiers(struct Jewelry *jewelry)
{
  char *workmanshipBonus = NULL;
  char *exceptionalStoneBonus = NULL;
  
  if (jewelry->workmanshipBonus) {
    asprintf_or_die(&workmanshipBonus, "workmanship %+i",
                    jewelry->workmanshipBonus);
  }
  if (jewelry->exceptionalStoneBonus) {
    asprintf_or_die(&exceptionalStoneBonus, "exceptional stone %+i",
                    jewelry->exceptionalStoneBonus);
  }
  
  char *description;
  if (workmanshipBonus && exceptionalStoneBonus) {
    asprintf_or_die(&description, "%s, %s",
                    workmanshipBonus, exceptionalStoneBonus);
  } else if (workmanshipBonus || exceptionalStoneBonus) {
    asprintf_or_die(&description, "%s",
                    workmanshipBonus ? workmanshipBonus : exceptionalStoneBonus);
  } else {
    description = strdup_or_die("");
  }
  
  free_or_die(exceptionalStoneBonus);
  free_or_die(workmanshipBonus);
  return description;
}


void finalizeJewelry(struct Jewelry *jewelry)
{
  free_or_die(jewelry->trueDescription);
}


void generateJewelry(struct Jewelry *jewelry, struct rnd *rnd)
{
  int rank;
  int score = roll("1d100", rnd);
  if (score <= 10) {
    rank = 1;
  } else if (score <= 20) {
    rank = 2;
  } else if (score <= 40) {
    rank = 3;
  } else if (score <= 50) {
    rank = 4;
  } else if (score <= 70) {
    rank = 5;
  } else if (score <= 90) {
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
    struct dice dice = dice_make(1, jewelryRanks[rank].materialsCount);
    score = dice_roll(dice, rnd, NULL);
    jewelry->material = jewelryRanks[rank].materials[score - 1];
  }
  
  score = roll(jewelryRanks[rank].baseValue, rnd);
  jewelry->value_cp = score * jewelryRanks[rank].baseValueMultiplier;
  
  score = roll("1d100", rnd);
  for (int i = 0; i < jewelryFormTableCount; ++i) {
    if (score <= jewelryFormTable[i].dieRoll) {
      jewelry->form = jewelryFormTable[i].form;
      break;
    }
  }
  
  int const maxWorkmanshipBonus = 12;
  do {
    score = roll("1d10", rnd);
    if (score == 1) {
      ++jewelry->workmanshipBonus;
    }
  } while (score == 1 && jewelry->workmanshipBonus < maxWorkmanshipBonus);
  for (int i = 0; i < jewelry->workmanshipBonus; ++i) {
    int maxValue_cp = dice_max_score(dice_parse(jewelryRanks[rank].baseValue))
                    * jewelryRanks[rank].baseValueMultiplier;
    if (jewelry->value_cp < maxValue_cp) {
      jewelry->value_cp = maxValue_cp;
    } else if (rank < jewelryMaxRank) {
      ++rank;
      score = roll(jewelryRanks[rank].baseValue, rnd);
      jewelry->value_cp = score * jewelryRanks[rank].baseValueMultiplier;
    }
  }
  
  int const maxExceptionalStoneBonus = 128;
  if (jewelry->hasGems) {
    score = roll("1d8", rnd);
    if (score == 1) {
      jewelry->exceptionalStoneBonus = 1;
      do {
        score = roll("1d6", rnd);
        if (score == 1) {
          jewelry->exceptionalStoneBonus *= 2;
        }
      } while (   score == 1 
               && jewelry->exceptionalStoneBonus < maxExceptionalStoneBonus);
    }
    jewelry->value_cp += gp_to_cp(jewelry->exceptionalStoneBonus * 5000);
  }
  
  char *description = describeJewelry(jewelry);
  char *modifiers = describeJewelryModifiers(jewelry);
  int value_cp = jewelryValue_cp(jewelry);
  char *value_gp = coins_alloc_gp_cp_description(value_cp);
  
  char const *separator = str_not_empty(modifiers) ? ": " : "";
  asprintf_or_die(&jewelry->trueDescription, "%s (%s%s%s)",
                  description, modifiers, separator, value_gp);
  
  free_or_die(modifiers);
  free_or_die(value_gp);
  free_or_die(description);
}


void initializeJewelry(struct Jewelry *jewelry)
{
  memset(jewelry, 0, sizeof(struct Jewelry));
}


int jewelryValue_cp(struct Jewelry *jewelry)
{
  return jewelry->value_cp;
}

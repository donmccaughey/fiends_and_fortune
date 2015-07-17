#include "MagicItem.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Character.h"
#include "coinage.h"
#include "Dice.h"
#include "earmark.h"
#include "heap.h"
#include "unexpected.h"


typedef void(*GenerateFunction)(struct MagicItem *magicItem, struct Dice *dice);


struct MiscMagicItem {
  int percent;
  char const *name;
  int experiencePoints;
  int saleValue_gp;
  ClassRestrictions classRestrictions;
  GenerateFunction generate;
};


static void generateArmorOrShield(struct MagicItem *magicItem, 
                                  struct Dice *dice);

static void generateArtifactOrRelic(struct MagicItem *magicItem, 
                                    struct Dice *dice);

static void generateBracersOfDefense(struct MagicItem *magicItem, 
                                     struct Dice *dice);

static void generateBucknardsEverfullPurse(struct MagicItem *magicItem, 
                                           struct Dice *dice);

static void generateCloakOfProtection(struct MagicItem *magicItem, 
                                      struct Dice *dice);

static void generateCrystalBall(struct MagicItem *magicItem, struct Dice *dice);

static void generateEyesOfPetrification(struct MagicItem *magicItem, 
                                        struct Dice *dice);

static void generateFigurineOfWondrousPower(struct MagicItem *magicItem, 
                                            struct Dice *dice);

static void generateGirdleOfGiantStrength(struct MagicItem *magicItem, 
                                          struct Dice *dice);

static void generateHornOfValhalla(struct MagicItem *magicItem, 
                                   struct Dice *dice);

static void generateIounStones(struct MagicItem *magicItem, struct Dice *dice);

static void generateInstrumentOfTheBards(struct MagicItem *magicItem, 
                                         struct Dice *dice);

static void generateJewelOfFlawlessness(struct MagicItem *magicItem, 
                                        struct Dice *dice);

static void generateMedallionOfESP(struct MagicItem *magicItem, 
                                   struct Dice *dice);

static void generateMiscMagicItemTable1(struct MagicItem *magicItem, 
                                        struct Dice *dice);

static void generateMiscMagicItemTable2(struct MagicItem *magicItem, 
                                        struct Dice *dice);

static void generateMiscMagicItemTable3(struct MagicItem *magicItem, 
                                        struct Dice *dice);

static void generateMiscMagicItemTable4(struct MagicItem *magicItem, 
                                        struct Dice *dice);

static void generateMiscMagicItemTable5(struct MagicItem *magicItem, 
                                        struct Dice *dice);

static void generateMiscWeapon(struct MagicItem *magicItem, struct Dice *dice);

static void generateNecklaceOfMissiles(struct MagicItem *magicItem, 
                                       struct Dice *dice);

static void generateNecklaceOfPrayerBeads(struct MagicItem *magicItem, 
                                          struct Dice *dice);

static void generateNolzursMarvelousPigments(struct MagicItem *magicItem, 
                                             struct Dice *dice);

static void generateOrbOfDragonkind(struct MagicItem *magicItem, 
                                    struct Dice *dice);

static void generatePearlOfPower(struct MagicItem *magicItem, 
                                 struct Dice *dice);

static void generatePotion(struct MagicItem *magicItem, struct Dice *dice);

static void generatePotionOfDragonControl(struct MagicItem *magicItem, 
                                          struct Dice *dice);

static void generatePotionOfGiantControl(struct MagicItem *magicItem, 
                                         struct Dice *dice);

static void generatePotionOfGiantStrength(struct MagicItem *magicItem, 
                                          struct Dice *dice);

static void generateQuaalsFeatherToken(struct MagicItem *magicItem, 
                                       struct Dice *dice);

static void generateRing(struct MagicItem *magicItem, struct Dice *dice);

static void generateRingOfProtection(struct MagicItem *magicItem, 
                                     struct Dice *dice);

static void generateRodStaffOrWand(struct MagicItem *magicItem, 
                                   struct Dice *dice);

static void generateScroll(struct MagicItem *magicItem, struct Dice *dice);

static void generateSword(struct MagicItem *magicItem, struct Dice *dice);

static void generateTeethOfDahlverNar(struct MagicItem *magicItem, 
                                      struct Dice *dice);


void finalizeMagicItem(struct MagicItem *magicItem)
{
  if (magicItem->trueDetails) {
    int i = 0;
    while (magicItem->trueDetails[i]) {
      free(magicItem->trueDetails[i]);
      ++i;
    }
    free(magicItem->trueDetails);
  }
  free(magicItem->trueDescription);
}


static void generateArmorOrShield(struct MagicItem *magicItem, 
                                  struct Dice *dice)
{
  static struct ArmorOrShield {
    int percent;
    char const *name;
    bool isArmor;
    int experiencePoints;
    int saleValue_gp;
  } const armorAndShieldTable[] = {
    {   5 -  0, "chain mail +1", true, 600, 3500 },
    {   9 -  5, "chain mail +2", true, 1200, 7500 },
    {  11 -  9, "chain mail +3", true, 2000, 12500 },
    {  19 - 11, "leather armor +1", true, 300, 2000 },
    {  26 - 19, "plate mail +1", true, 800, 5000 },
    {  32 - 26, "plate mail +2", true, 1750, 10500 },
    {  35 - 32, "plate mail +3", true, 2750, 15500 },
    {  37 - 35, "plate mail +4", true, 3500, 20500 },
    {  38 - 37, "plate mail +5", true, 4500, 27500 },
    {  39 - 38, "plate mail of etherealness", true, 5000, 30000 },
    {  44 - 39, "plate mail of vulnerability", true, 0, 1500 },
    {  50 - 44, "ring mail +1", true, 400, 2500 },
    {  55 - 50, "scale mail +1", true, 500, 3000 },
    {  59 - 55, "scale mail +2", true, 1100, 6750 },
    {  63 - 59, "splint mail +1", true, 700, 4000 },
    {  66 - 63, "splint mail +2", true, 1500, 8500 },
    {  68 - 66, "splint mail +3", true, 2250, 14500 },
    {  69 - 68, "splint mail +4", true, 3000, 19000 },
    {  75 - 69, "studded leather +1", true, 400, 2500 },
    {  84 - 75, "shield +1", false, 250, 25000 },
    {  89 - 84, "shield +2", false, 500, 5000 },
    {  93 - 89, "shield +3", false, 800, 8000 },
    {  95 - 93, "shield +4", false, 1200, 12000 },
    {  96 - 95, "shield +5", false, 1750, 17500 },
    {  97 - 96, "shield, large, +1, +4 vs missiles", false, 400, 4000 },
    { 100 - 97, "shield -1, missile attractor", false, 0, 750 }
  };
  static size_t const armorAndShieldTableCount = sizeof armorAndShieldTable 
                                               / sizeof armorAndShieldTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct ArmorOrShield const *armorOrShield = NULL;
  for (int i = 0; i < armorAndShieldTableCount; ++i) {
    range += armorAndShieldTable[i].percent;
    if (dieRoll <= range) {
      armorOrShield = &armorAndShieldTable[i];
      break;
    }
  }
  assert(armorOrShield);
  
  magicItem->experiencePoints = armorOrShield->experiencePoints;
  magicItem->trueValue_cp = armorOrShield->saleValue_gp * CP_PER_GP;
  
  if (armorOrShield->isArmor) {
    char const *armorSize;
    dieRoll = roll(dice, "1d100");
    if (dieRoll <= 65) {
      armorSize = "human sized";
    } else if (dieRoll <= 85) {
      armorSize = "elf sized";
    } else if (dieRoll <= 95) {
      armorSize = "dwarf sized";
    } else {
      armorSize = "gnome or halfling sized";
    }
    em_asprintf(&magicItem->trueDescription, "%s (%s)",
                armorOrShield->name, armorSize);
  } else {
    em_asprintf(&magicItem->trueDescription, "%s", armorOrShield->name);
  }
}


static void generateArtifactOrRelic(struct MagicItem *magicItem, 
                                    struct Dice *dice)
{
  static struct ArtifactOrRelic {
    int percent;
    char const *name;
    int saleValue_gp;
    GenerateFunction generate;
  } const artifactsAndRelicsTable[] = {
    {   1 -  0, "axe of the dwarvish lords", 55000 },
    {   2 -  1, "Baba Yaga's hut", 90000 },
    {   4 -  2, "codex of the infinite planes", 62500 },
    {  20 -  4, "crown of might", 50000 },
    {  21 - 20, "crystal of the ebon flame", 75000 },
    {  22 - 21, "cup and talisman of Al'Akbar", 85000 },
    {  24 - 22, "eye of Vecna", 35000 },
    {  25 - 24, "hand of Vecna", 60000 },
    {  26 - 25, "Heward's mystical organ", 25000 },
    {  27 - 26, "horn of change", 20000 },
    {  29 - 27, "invulnerable coat of Arnd", 47500 },
    {  31 - 29, "iron flask of Tuerny the Merciless", 50000 },
    {  32 - 31, "jacinth of inestimable beauty", 100000 },
    {  33 - 32, "Johydee's mask", 40000 },
    {  35 - 33, "Kuroth's quill", 27500 },
    {  37 - 35, "mace of Cuthbert", 35000 },
    {  38 - 37, "machine of Lum the Mad", 72500 },
    {  40 - 38, "mighty servant of Leuk-O", 185000 },
    {  47 - 40, "orb of the dragonkind", 10000, generateOrbOfDragonkind },
    {  63 - 47, "orb of might", 100000 },
    {  64 - 63, "Queen Ehlissa's marvelous nightengale", 112500 },
    {  66 - 64, "recorder of Ye'Cind", 80000 },
    {  68 - 66, "ring of Gaxx", 17500 },
    {  74 - 68, "rod of seven parts", 25000 },
    {  91 - 74, "sceptre of might", 150000 },
    {  92 - 91, "sword of Kas", 97000 },
    {  98 - 92, "teeth of Dahlver-Nar", 5000, generateTeethOfDahlverNar },
    {  99 - 98, "throne of the gods", 0 },
    { 100 - 99, "wand of Orcus", 10000 }
  };
  size_t const artifactsAndRelicsTableCount = sizeof artifactsAndRelicsTable 
                                            / sizeof artifactsAndRelicsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct ArtifactOrRelic const *artifactOrRelic = NULL;
  for (int i = 0; i < artifactsAndRelicsTableCount; ++i) {
    range += artifactsAndRelicsTable[i].percent;
    if (dieRoll <= range) {
      artifactOrRelic = &artifactsAndRelicsTable[i];
      break;
    }
  }
  assert(artifactOrRelic);
  
  if (artifactOrRelic->generate) {
    artifactOrRelic->generate(magicItem, dice);
  } else {
    magicItem->experiencePoints = 0;
    magicItem->trueValue_cp = artifactOrRelic->saleValue_gp * CP_PER_GP;
    em_asprintf(&magicItem->trueDescription, "%s", artifactOrRelic->name);
  }
}


static void generateBracersOfDefense(struct MagicItem *magicItem, 
                                     struct Dice *dice)
{
  int armorClass = 0;
  
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 5) {
    armorClass = 8;
  } else if (dieRoll <= 15) {
    armorClass = 7;
  } else if (dieRoll <= 35) {
    armorClass = 6;
  } else if (dieRoll <= 50) {
    armorClass = 5;
  } else if (dieRoll <= 70) {
    armorClass = 4;
  } else if (dieRoll <= 85) {
    armorClass = 3;
  } else {
    armorClass = 2;
  }
  
  int level = 10 - armorClass;
  magicItem->experiencePoints = level * 500;
  magicItem->trueValue_cp = level * 3000 * CP_PER_GP;
  em_asprintf(&magicItem->trueDescription,
              "bracers of defense AC %i", armorClass);
}


static void generateBucknardsEverfullPurse(struct MagicItem *magicItem, 
                                           struct Dice *dice)
{
  int type = 0;
  
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 50) {
    type = 1;
    magicItem->experiencePoints = 1500;
    magicItem->trueValue_cp = 15000 * CP_PER_GP;
  } else if (dieRoll <= 90) {
    type = 2;
    magicItem->experiencePoints = 2500;
    magicItem->trueValue_cp = 25000 * CP_PER_GP;
  } else {
    type = 3;
    magicItem->experiencePoints = 4000;
    magicItem->trueValue_cp = 40000 * CP_PER_GP;
  }
  
  em_asprintf(&magicItem->trueDescription,
              "Bucknards everfull purse (type %i)", type);
}


static void generateCloakOfProtection(struct MagicItem *magicItem, 
                                      struct Dice *dice)
{
  int plus = 0;
  
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 35) {
    plus = 1;
  } else if (dieRoll <= 65) {
    plus = 2;
  } else if (dieRoll <= 85) {
    plus = 3;
  } else if (dieRoll <= 95) {
    plus = 4;
  } else {
    plus = 5;
  }
  
  magicItem->experiencePoints = 1000 * plus;
  magicItem->trueValue_cp = 10000 * plus * CP_PER_GP;
  
  em_asprintf(&magicItem->trueDescription, "cloak of protection +%i", plus);
}


static void generateCrystalBall(struct MagicItem *magicItem, struct Dice *dice)
{
  char const *feature = "";
  
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 50) {
    magicItem->experiencePoints = 1000;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 75) {
    feature = " with clairaudience";
    magicItem->experiencePoints = 2000;
    magicItem->trueValue_cp = 10000 * CP_PER_GP;
  } else if (dieRoll <= 90) {
    feature = " with ESP";
    magicItem->experiencePoints = 2000;
    magicItem->trueValue_cp = 10000 * CP_PER_GP;
  } else {
    feature = " with telepathy (communication only)";
    magicItem->experiencePoints = 2000;
    magicItem->trueValue_cp = 10000 * CP_PER_GP;
  }
  
  em_asprintf(&magicItem->trueDescription, "crystal ball%s", feature);
}


static void generateEyesOfPetrification(struct MagicItem *magicItem, 
                                        struct Dice *dice)
{
  char const *type;
  
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 25) {
    type = "others";
    magicItem->experiencePoints = 12500;
    magicItem->trueValue_cp = 50000 * CP_PER_GP;
  } else {
    type = "self";
    magicItem->experiencePoints = 0;
    magicItem->trueValue_cp = 0;
  }
  
  em_asprintf(&magicItem->trueDescription, "eyes of petrification (%s)", type);
}


static void generateFigurineOfWondrousPower(struct MagicItem *magicItem, 
                                            struct Dice *dice)
{
  int hitDice;
  char *type;
  
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 15) {
    hitDice = 4;
    em_asprintf(&type, "ebony fly (%i hp)", roll(dice, "4d8+4"));
  } else if (dieRoll <= 30) {
    em_asprintf(&type, "two golden lions (%i/%i hp)",
                roll(dice, "5d8+2"), roll(dice, "5d8+2"));
    hitDice = 10;
  } else if (dieRoll <= 40) {
    em_asprintf(&type, "three ivory goats (24/96/48 hp)");
    hitDice = 4 + 16 + 8;
  } else if (dieRoll <= 55) {
    dieRoll = roll(dice, "1d100");
    if (dieRoll <= 50) {
      hitDice = 10;
      em_asprintf(&type, "marble elephant, asiatic (%i hp)",
                  roll(dice, "10d8"));
    } else if (dieRoll <= 90) {
      hitDice = 11;
      em_asprintf(&type, "marble elephant, african (%i hp)",
                  roll(dice, "11d8"));
    } else if (dieRoll <= 93) {
      hitDice = 13;
      em_asprintf(&type, "marble elephant, mammoth (%i hp)",
                  roll(dice, "13d8"));
    } else {
      hitDice = 12;
      em_asprintf(&type, "marble elephant, mastodon (%i hp)",
                  roll(dice, "12d8"));
    }
  } else if (dieRoll <= 65) {
    hitDice = 6;
    em_asprintf(&type, "obsidian steed (%i hp)", roll(dice, "6d8+6"));
  } else if (dieRoll <= 85) {
    hitDice = 2;
    em_asprintf(&type, "onyx dog (%i hp)", roll(dice, "2d8+2"));
  } else {
    hitDice = 4;
    em_asprintf(&type, "serpentine owl (%i hp)", roll(dice, "4d8"));
  }
  
  magicItem->experiencePoints = hitDice * 100;
  magicItem->trueValue_cp = hitDice * 1000 * CP_PER_GP;
  
  em_asprintf(&magicItem->trueDescription,
              "figurine of wondrous power: %s", type);
  free(type);
}


static void generateGirdleOfGiantStrength(struct MagicItem *magicItem, 
                                          struct Dice *dice)
{
  char const *type;
  
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 30) {
    type = "hill";
  } else if (dieRoll <= 50) {
    type ="stone";
  } else if (dieRoll <= 70) {
    type ="frost";
  } else if (dieRoll <= 85) {
    type ="fire";
  } else if (dieRoll <= 95) {
    type ="cloud";
  } else {
    type ="storm";
  }
  
  magicItem->experiencePoints = 200;
  magicItem->trueValue_cp = 2500 * CP_PER_GP;
  
  em_asprintf(&magicItem->trueDescription, "girdle of %s giant strength", type);
}


static void generateHornOfValhalla(struct MagicItem *magicItem, 
                                   struct Dice *dice)
{
  char const *type;
  int multiplier;
  
  int dieRoll = roll(dice, "1d20");
  if (dieRoll <= 8) {
    type = "silver";
    multiplier = 1;
  } else if (dieRoll <= 15) {
    type = "brass";
    multiplier = 1;
  } else if (dieRoll <= 18) {
    type = "bronze";
    multiplier = 2;
  } else {
    type = "iron";
    multiplier = 3;
  }
  
  magicItem->experiencePoints = 1000 * multiplier;
  magicItem->trueValue_cp = 15000 * CP_PER_GP * multiplier;
  
  char const *alignment = NULL;
  dieRoll = roll(dice, "1d100");
  if (dieRoll <= 50) {
    dieRoll = roll(dice, "1d8");
    switch (dieRoll) {
      case 1: alignment = "good"; break;
      case 2: alignment = "lawful good"; break;
      case 3: alignment = "lawful"; break;
      case 4: alignment = "lawful evil"; break;
      case 5: alignment = "evil"; break;
      case 6: alignment = "chaotic evil"; break;
      case 7: alignment = "chaotic"; break;
      case 8: alignment = "chaotic good"; break;
      default: UNEXPECTED("1d8 die roll is %i", dieRoll); break;
    }
    em_asprintf(&magicItem->trueDescription, "%s horn of Valhalla (%s)",
                type, alignment);
  } else {
    em_asprintf(&magicItem->trueDescription, "%s horn of Valhalla", type);
  }
}


static void generateIounStones(struct MagicItem *magicItem, struct Dice *dice)
{
  int quantity = roll(dice, "1d10");
  /* TODO: generate types of stones */
  
  magicItem->experiencePoints = quantity * 300;
  magicItem->trueValue_cp = quantity * 5000 * CP_PER_GP;
  
  char const *plural = (quantity == 1) ? "" : "s";
  em_asprintf(&magicItem->trueDescription, "(%i) ioun stone%s",
              quantity, plural);
}


static void generateInstrumentOfTheBards(struct MagicItem *magicItem, 
                                         struct Dice *dice)
{
  char const *name;
  int level;
  
  int dieRoll = roll(dice, "1d20");
  if (dieRoll <= 5) {
    name = "Fochlucan Bandore";
    level = 1;
  } else if (dieRoll <= 9) {
    name = "Mac-Fuirmidh Cittern";
    level = 5;
  } else if (dieRoll <= 12) {
    name = "Doss Lute";
    level = 8;
  } else if (dieRoll <= 15) {
    name = "Canaith Mandolin";
    level = 11;
  } else if (dieRoll <= 17) {
    name = "Cli Lyre";
    level = 14;
  } else if (dieRoll <= 19) {
    name = "Anstruth Harp";
    level = 17;
  } else {
    name = "Ollamh Harp";
    level = 20;
  }
  
  magicItem->experiencePoints = level * 1000;
  magicItem->trueValue_cp = level * 5000 * CP_PER_GP;
  
  em_asprintf(&magicItem->trueDescription, "instrument of the bards: %s", name);
}


static void generateJewelOfFlawlessness(struct MagicItem *magicItem, 
                                        struct Dice *dice)
{
  int facets = roll(dice, "10d10");
  
  magicItem->experiencePoints = 0;
  magicItem->trueValue_cp = facets * 1000 * CP_PER_GP;
  
  char const *plural = (facets == 1) ? "" : "s";
  em_asprintf(&magicItem->trueDescription,
              "jewel of flawlessness with %i facet%s", facets, plural);
}


void generateMagicItem(struct MagicItem *magicItem, 
                       struct Dice *dice,
                       PossibleMagicItems possibleMagicItems)
{
  struct {
    int percent;
    PossibleMagicItems possibleMagicItems;
    enum MagicItemType type;
    GenerateFunction generate;
  } const magicItemsTable[] = {
    {  20 -  0, POTION, PotionMagicItemType, generatePotion },
    {  35 - 20, SCROLL, ScrollMagicItemType, generateScroll },
    {  40 - 35, RING, RingMagicItemType, generateRing },
    {  45 - 40, ROD_STAFF_WAND, RodStaffOrWandMagicItemType, generateRodStaffOrWand },
    {  48 - 45, MISC_MAGIC, MiscMagicItemType, generateMiscMagicItemTable1 },
    {  51 - 48, MISC_MAGIC, MiscMagicItemType, generateMiscMagicItemTable2 },
    {  54 - 51, MISC_MAGIC, MiscMagicItemType, generateMiscMagicItemTable3 },
    {  57 - 54, MISC_MAGIC, MiscMagicItemType, generateMiscMagicItemTable4 },
    {  60 - 57, MISC_MAGIC, MiscMagicItemType, generateMiscMagicItemTable5 },
    {  75 - 60, ARMOR_SHIELD, ArmorOrShieldMagicItemType, generateArmorOrShield },
    {  86 - 75, SWORD, SwordMagicItemType, generateSword },
    { 100 - 86, MISC_WEAPON, MiscWeaponMagicItemType, generateMiscWeapon }
  };
  size_t const magicItemsTableCount = sizeof magicItemsTable 
                                    / sizeof magicItemsTable[0];
  
  int total = 0;
  for (int i = 0; i < magicItemsTableCount; ++i) {
    if (possibleMagicItems & magicItemsTable[i].possibleMagicItems) {
      total += magicItemsTable[i].percent;
    }
  }
  
  int dieRoll = rollDice(dice, 1, total);
  int range = 0;
  for (int i = 0; i < magicItemsTableCount; ++i) {
    if (possibleMagicItems & magicItemsTable[i].possibleMagicItems) {
      range += magicItemsTable[i].percent;
      if (dieRoll <= range) {
        magicItem->type = magicItemsTable[i].type;
        magicItemsTable[i].generate(magicItem, dice);
        return;
      }
    }
  }
  assert(false);
}


static void generateMedallionOfESP(struct MagicItem *magicItem, 
                                   struct Dice *dice)
{
  char const *type;
  
  int dieRoll = roll(dice, "1d20");
  if (15 <= dieRoll) {
    type = "30 ft range";
    magicItem->experiencePoints = 1000;
    magicItem->trueValue_cp = 10000 * CP_PER_GP;
  } else if (18 <= dieRoll) {
    type = "30 ft range with empathy";
    magicItem->experiencePoints = 1000;
    magicItem->trueValue_cp = 10000 * CP_PER_GP;
  } else if (19 <= dieRoll) {
    type = "60 ft range";
    magicItem->experiencePoints = 3000;
    magicItem->trueValue_cp = 30000 * CP_PER_GP;
  } else {
    type = "90 ft range";
    magicItem->experiencePoints = 3000;
    magicItem->trueValue_cp = 30000 * CP_PER_GP;
  }
  
  em_asprintf(&magicItem->trueDescription, "medallion of ESP, %s", type);
}


static void generateMiscMagicItemTable1(struct MagicItem *magicItem, 
                                        struct Dice *dice)
{
  static struct MiscMagicItem const miscMagicItemsTable[] = {
    {   2 -  0, "alchemy jug", 3000, 12000 },
    {   4 -  2, "amulet of inescapable location", 0, 1000 },
    {   5 -  4, "amulet of life protection", 5000, 20000 },
    {   7 -  5, "amulet of the planes", 6000, 30000 },
    {  11 -  7, "amulet of proof against detection and location", 4000, 15000 },
    {  13 - 11, "apparatus of kwalish", 8000, 35000 },
    {  16 - 13, "arrow of direction", 2500, 17500 },
    {  17 - 16, "artifact or relic", 0, 0, NO_CLASS_RESTRICTIONS, generateArtifactOrRelic },
    {  20 - 17, "bag of beans", 1000, 5000 },
    {  21 - 20, "bag of devouring", 0, 1500 },
    {  26 - 21, "bag of holding", 5000, 25000 },
    {  27 - 26, "bag of transmuting", 0, 500 },
    {  29 - 27, "bag of tricks", 2500, 15000 },
    {  31 - 29, "beaker of plentiful potions", 1500, 12500 },
    {  32 - 31, "folding boat", 10000, 25000 },
    {  33 - 32, "book of exalted deeds", 8000, 40000, CLERICS },
    {  34 - 33, "book of infinite spells", 9000, 50000 },
    {  35 - 34, "book of vile darkness", 8000, 40000, CLERICS },
    {  36 - 35, "boots of dancing", 0, 5000 },
    {  42 - 36, "boots of elvenkind", 1000, 5000 },
    {  47 - 42, "boots of levitation", 2000, 15000 },
    {  51 - 47, "boots of speed", 2500, 20000 },
    {  55 - 51, "boots of striding and springing", 2500, 20000 },
    {  58 - 55, "bowl commanding water elementals", 4000, 25000, MAGIC_USERS },
    {  59 - 58, "bowl of watery death", 0, 1000 },
    {  79 - 59, "bracers of defense", 500, 3000, NO_CLASS_RESTRICTIONS, generateBracersOfDefense },
    {  81 - 79, "bracers of defenselessness", 0, 2000 },
    {  84 - 81, "brazier commanding fire elementals", 4000, 25000, MAGIC_USERS },
    {  85 - 84, "brazier of sleep smoke", 0, 1000, MAGIC_USERS },
    {  92 - 85, "brooch of shielding", 1000, 10000 },
    {  93 - 92, "broom of animated attack", 0, 3000 },
    {  98 - 93, "broom of flying", 2000, 10000 },
    { 100 - 98, "Bucknards everfull purse", 1500, 15000, NO_CLASS_RESTRICTIONS, generateBucknardsEverfullPurse }
  };
  size_t const miscMagicItemsTableCount = sizeof miscMagicItemsTable 
                                        / sizeof miscMagicItemsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct MiscMagicItem const *miscMagicItem = NULL;
  for (int i = 0; i < miscMagicItemsTableCount; ++i) {
    range += miscMagicItemsTable[i].percent;
    if (dieRoll <= range) {
      miscMagicItem = &miscMagicItemsTable[i];
      break;
    }
  }
  assert(miscMagicItem);
  
  if (miscMagicItem->generate) {
    miscMagicItem->generate(magicItem, dice);
  } else {
    magicItem->experiencePoints = miscMagicItem->experiencePoints;
    magicItem->trueValue_cp = miscMagicItem->saleValue_gp * CP_PER_GP;
    em_asprintf(&magicItem->trueDescription, "%s", miscMagicItem->name);
  }
}


static void generateMiscMagicItemTable2(struct MagicItem *magicItem, 
                                        struct Dice *dice)
{
  static struct MiscMagicItem const miscMagicItemsTable[] = {
    {   6 -  0, "candle of invocation", 1000, 5000, CLERICS },
    {   8 -  6, "carpet of flying", 7500, 25000 },
    {  10 -  8, "censer controlling air elementals", 4000, 25000, MAGIC_USERS },
    {  11 - 10, "censer of summoning hostile air elementals", 0, 1000, MAGIC_USERS },
    {  13 - 11, "chime of opening", 3500, 20000 },
    {  14 - 13, "chime of hunger", 0, 0 },
    {  18 - 14, "cloak of displacement", 3000, 17500 },
    {  27 - 18, "cloak of elvenkind", 1000, 6000 },
    {  30 - 27, "cloak of manta ray", 2000, 12500 },
    {  32 - 30, "cloak of poisonousness", 0, 2500 },
    {  55 - 32, "cloak of protection", 1000, 10000, NO_CLASS_RESTRICTIONS, generateCloakOfProtection },
    {  60 - 55, "crystal ball", 1000, 5000, MAGIC_USERS, generateCrystalBall },
    {  61 - 60, "crystal hypnosis ball", 0, 3000, MAGIC_USERS },
    {  63 - 61, "cube of force", 3000, 20000 },
    {  65 - 63, "cube of frost resistance", 2000, 14000 },
    {  67 - 65, "cubic gate", 5000, 17500 },
    {  69 - 67, "Daern's instant fortress", 7000, 27500 },
    {  72 - 69, "decanter of endless water", 1000, 3000 },
    {  76 - 72, "deck of many things", 0, 10000 },
    {  77 - 76, "drums of deafening", 0, 500 },
    {  79 - 77, "drums of panic", 6500, 35000 },
    {  85 - 79, "dust of appearance", 1000, 4000 },
    {  91 - 85, "dust of disappearance", 2000, 8000 },
    {  92 - 91, "dust of sneezing and choking", 0, 1000 },
    {  93 - 92, "efreeti bottle", 9000, 45000 },
    {  94 - 93, "eversmoking bottle", 500, 2500 },
    {  95 - 94, "eyes of charming", 4000, 24000, MAGIC_USERS },
    {  97 - 95, "eyes of the eagle", 3500, 18000 },
    {  99 - 97, "eyes of minute seeing", 2000, 12500 },
    { 100 - 99, "eyes of petrification", 0, 0, NO_CLASS_RESTRICTIONS, generateEyesOfPetrification }
  };
  size_t const miscMagicItemsTableCount = sizeof miscMagicItemsTable 
                                        / sizeof miscMagicItemsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct MiscMagicItem const *miscMagicItem = NULL;
  for (int i = 0; i < miscMagicItemsTableCount; ++i) {
    range += miscMagicItemsTable[i].percent;
    if (dieRoll <= range) {
      miscMagicItem = &miscMagicItemsTable[i];
      break;
    }
  }
  assert(miscMagicItem);
  
  if (miscMagicItem->generate) {
    miscMagicItem->generate(magicItem, dice);
  } else {
    magicItem->experiencePoints = miscMagicItem->experiencePoints;
    magicItem->trueValue_cp = miscMagicItem->saleValue_gp * CP_PER_GP;
    em_asprintf(&magicItem->trueDescription, "%s", miscMagicItem->name);
  }  
}


static void generateMiscMagicItemTable3(struct MagicItem *magicItem, 
                                        struct Dice *dice)
{
  static struct MiscMagicItem const miscMagicItemsTable[] = {
    {  15 -  0, "figurine of wondrous power", 100, 1000, NO_CLASS_RESTRICTIONS, generateFigurineOfWondrousPower },
    {  16 - 15, "flask of curses", 0, 1000 },
    {  18 - 16, "gauntlets of dexterity", 1000, 10000 },
    {  20 - 18, "gauntlets of fumbling", 0, 1000 },
    {  22 - 20, "gauntlets of ogre power", 1000, 15000, CLERICS | FIGHTERS | THIEVES },
    {  25 - 22, "gauntlets of swimming and climbing", 1000, 10000, CLERICS | FIGHTERS | THIEVES },
    {  26 - 25, "gem of brightness", 2000, 17500 },
    {  27 - 26, "gem of seeing", 2000, 25000 },
    {  28 - 27, "girdle of femininity/masculinity", 0, 1000, CLERICS | FIGHTERS | THIEVES },
    {  29 - 28, "girdle of giant strength", 200, 2500, CLERICS | FIGHTERS | THIEVES, generateGirdleOfGiantStrength },
    {  30 - 29, "helm of brilliance", 2500, 60000 },
    {  35 - 30, "helm of comprehending languages & reading magic", 1000, 12500 },
    {  37 - 35, "helm of opposite alignment", 0, 1000 },
    {  39 - 37, "helm of telepathy", 3000, 35000 },
    {  40 - 39, "helm of teleportation", 2500, 30000 },
    {  45 - 40, "helm of underwater action", 1000, 10000 },
    {  46 - 45, "horn of blasting", 5000, 55000 },
    {  48 - 46, "horn of bubbles", 0, 0 },
    {  49 - 48, "horn of collapsing", 1500, 25000 },
    {  53 - 49, "horn of the tritons", 2000, 17500, CLERICS | FIGHTERS },
    {  60 - 53, "horn of Valhalla", 1000, 15000, NO_CLASS_RESTRICTIONS, generateHornOfValhalla },
    {  63 - 60, "horseshoes of speed", 2000, 10000 },
    {  65 - 63, "horseshoes of a zephyr", 1500, 7500 },
    {  70 - 65, "incense of meditation", 500, 7500, CLERICS },
    {  71 - 70, "incense of obsession", 0, 500, CLERICS },
    {  72 - 71, "ioun stones", 300, 5000, NO_CLASS_RESTRICTIONS, generateIounStones },
    {  78 - 72, "instrument of the bards", 1000, 5000, NO_CLASS_RESTRICTIONS, generateInstrumentOfTheBards },
    {  80 - 78, "iron flask", 0, 0 },
    {  85 - 80, "javelin of lightning", 250, 3000, FIGHTERS },
    {  90 - 85, "javelin of piercing", 250, 3000, FIGHTERS },
    {  91 - 90, "jewel of attacks", 0, 1000 },
    {  92 - 91, "jewel of flawlessness", 0, 1000, NO_CLASS_RESTRICTIONS, generateJewelOfFlawlessness },
    { 100 - 92, "Keoghtom's ointment", 500, 10000 }
  };
  size_t const miscMagicItemsTableCount = sizeof miscMagicItemsTable 
                                        / sizeof miscMagicItemsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct MiscMagicItem const *miscMagicItem = NULL;
  for (int i = 0; i < miscMagicItemsTableCount; ++i) {
    range += miscMagicItemsTable[i].percent;
    if (dieRoll <= range) {
      miscMagicItem = &miscMagicItemsTable[i];
      break;
    }
  }
  assert(miscMagicItem);
  
  if (miscMagicItem->generate) {
    miscMagicItem->generate(magicItem, dice);
  } else {
    magicItem->experiencePoints = miscMagicItem->experiencePoints;
    magicItem->trueValue_cp = miscMagicItem->saleValue_gp * CP_PER_GP;
    em_asprintf(&magicItem->trueDescription, "%s", miscMagicItem->name);
  }
}


static void generateMiscMagicItemTable4(struct MagicItem *magicItem, 
                                        struct Dice *dice)
{
  static struct MiscMagicItem const miscMagicItemsTable[] = {
    {   1 -  0, "libram of gainful conjuration", 8000, 40000, MAGIC_USERS },
    {   2 -  1, "libram of ineffable damnation", 8000, 40000, MAGIC_USERS },
    {   3 -  2, "libram of silver magic", 8000, 40000, MAGIC_USERS },
    {   4 -  3, "lyre of building", 5000, 30000 },
    {   5 -  4, "manual of bodily health", 5000, 50000 },
    {   6 -  5, "manual of gainful exercise", 5000, 50000 },
    {   7 -  6, "manual of golems", 3000, 30000, CLERICS | MAGIC_USERS },
    {   8 -  7, "manual of puissant skill at arms", 8000, 40000, FIGHTERS },
    {   9 -  8, "manual of quickness of action", 5000, 50000 },
    {  10 -  9, "manual of stealthy pilfering", 8000, 40000, THIEVES },
    {  11 - 10, "mattock of the titans", 3500, 7000, FIGHTERS },
    {  12 - 11, "maul of the titans", 4000, 12000 },
    {  15 - 12, "medallion of ESP", 1000, 10000, NO_CLASS_RESTRICTIONS, generateMedallionOfESP },
    {  17 - 15, "medallion of thought projection", 0, 1000 },
    {  18 - 17, "mirror of life trapping", 2500, 25000, MAGIC_USERS },
    {  19 - 18, "mirror of mental prowess", 5000, 50000 },
    {  20 - 19, "mirror of opposition", 0, 2000 },
    {  23 - 20, "necklace of adaptation", 1000, 10000 },
    {  27 - 23, "necklace of missiles", 50, 200, NO_CLASS_RESTRICTIONS, generateNecklaceOfMissiles },
    {  33 - 27, "necklace of prayer beads", 500, 3000, CLERICS, generateNecklaceOfPrayerBeads },
    {  35 - 33, "necklace of strangulation", 0, 1000 },
    {  38 - 35, "net of entrapment", 1000, 7500, CLERICS | FIGHTERS | THIEVES },
    {  42 - 38, "net of snaring", 1000, 6000, CLERICS | FIGHTERS | THIEVES },
    {  44 - 42, "Nolzurs' marvelous pigments", 500, 3000, NO_CLASS_RESTRICTIONS, generateNolzursMarvelousPigments },
    {  46 - 44, "pearl of power", 200, 2000, MAGIC_USERS, generatePearlOfPower },
    {  48 - 46, "pearl of wisdom", 500, 5000, CLERICS },
    {  50 - 48, "periapt of foul rotting", 0, 1000 },
    {  53 - 50, "periapt of health", 1000, 10000 },
    {  60 - 53, "periapt of proof against poison", 1500, 12500 },
    {  64 - 60, "periapt of wound closure", 1000, 10000 },
    {  70 - 64, "phylactery of faithfulness", 1000, 7500, CLERICS },
    {  74 - 70, "phylactery of long years", 3000, 25000, CLERICS },
    {  76 - 74, "phylactery of monstrous attention", 0, 2000, CLERICS },
    {  84 - 76, "pipes of the sewers", 1750, 8500 },
    {  85 - 84, "portable hole", 5000, 50000 },
    { 100 - 85, "Quaal's feather token", 500, 2000, NO_CLASS_RESTRICTIONS, generateQuaalsFeatherToken },
  };
  size_t const miscMagicItemsTableCount = sizeof miscMagicItemsTable 
                                        / sizeof miscMagicItemsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct MiscMagicItem const *miscMagicItem = NULL;
  for (int i = 0; i < miscMagicItemsTableCount; ++i) {
    range += miscMagicItemsTable[i].percent;
    if (dieRoll <= range) {
      miscMagicItem = &miscMagicItemsTable[i];
      break;
    }
  }
  assert(miscMagicItem);
  
  if (miscMagicItem->generate) {
    miscMagicItem->generate(magicItem, dice);
  } else {
    magicItem->experiencePoints = miscMagicItem->experiencePoints;
    magicItem->trueValue_cp = miscMagicItem->saleValue_gp * CP_PER_GP;
    em_asprintf(&magicItem->trueDescription, "%s", miscMagicItem->name);
  }
}


static void generateMiscMagicItemTable5(struct MagicItem *magicItem, 
                                        struct Dice *dice)
{
  static struct MiscMagicItem const miscMagicItemsTable[] = {
    {   1 -  0, "robe of the archmagi", 6000, 65000, MAGIC_USERS },
    {   8 -  1, "robe of blending", 3500, 35000 },
    {   9 -  8, "robe of eyes", 4500, 50000, MAGIC_USERS },
    {  10 -  9, "robe of powerlessness", 0, 1000 },
    {  11 - 10, "robe of scintillating colors", 2750, 25000, CLERICS | MAGIC_USERS },
    {  19 - 11, "robe of useful items", 1500, 15000, MAGIC_USERS },
    {  25 - 19, "rope of climbing", 1000, 10000 },
    {  27 - 25, "rope of construction", 0, 1000 },
    {  31 - 27, "rope of entanglement", 1250, 12000 },
    {  32 - 31, "rug of smothering", 0, 1500 },
    {  33 - 32, "rug of welcome", 6500, 45000, MAGIC_USERS },
    {  34 - 33, "saw of mighty cutting", 1750, 12500, FIGHTERS },
    {  35 - 34, "scarab of death", 0, 2500 },
    {  38 - 35, "scarab of enraging enemies", 1000, 8000 },
    {  40 - 38, "scarab of insanity", 1500, 11000 },
    {  46 - 40, "scarab of protection", 2500, 25000 },
    {  47 - 46, "spade of colossal excavation", 1000, 6500, FIGHTERS },
    {  48 - 47, "sphere of annihilation", 3750, 30000, MAGIC_USERS },
    {  50 - 48, "stone of controlling earth elementals", 1500, 12500 },
    {  52 - 50, "stone of good luck (luckstone)", 3000, 25000 },
    {  54 - 52, "stone of weight (loadstone)", 0, 1000 },
    {  57 - 54, "talisman of pure good", 3500, 27500, CLERICS },
    {  58 - 57, "talisman of the sphere", 100, 10000, MAGIC_USERS },
    {  60 - 58, "talisman of ultimate evil", 3500, 32500, CLERICS },
    {  66 - 60, "talisman of Zagy", 1000, 10000 },
    {  67 - 66, "tome of clear thought", 8000, 48000 },
    {  68 - 67, "tome of leadership and influence", 7500, 40000 },
    {  69 - 68, "tome of understanding", 8000, 43500 },
    {  76 - 69, "trident of fish command", 500, 4000, CLERICS | FIGHTERS | THIEVES },
    {  78 - 76, "trident of submission", 1250, 12500, FIGHTERS },
    {  83 - 78, "trident of warning", 1000, 10000, CLERICS | FIGHTERS | THIEVES },
    {  85 - 83, "trident of yearning", 0, 1000 },
    {  87 - 85, "vacuous grimoire", 0, 1000 },
    {  90 - 87, "well of many worlds", 6000, 12000 },
    { 100 - 90, "wings of flying", 750, 7500 }
  };
  size_t const miscMagicItemsTableCount = sizeof miscMagicItemsTable 
                                        / sizeof miscMagicItemsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct MiscMagicItem const *miscMagicItem = NULL;
  for (int i = 0; i < miscMagicItemsTableCount; ++i) {
    range += miscMagicItemsTable[i].percent;
    if (dieRoll <= range) {
      miscMagicItem = &miscMagicItemsTable[i];
      break;
    }
  }
  assert(miscMagicItem);
  
  if (miscMagicItem->generate) {
    miscMagicItem->generate(magicItem, dice);
  } else {
    magicItem->experiencePoints = miscMagicItem->experiencePoints;
    magicItem->trueValue_cp = miscMagicItem->saleValue_gp * CP_PER_GP;
    em_asprintf(&magicItem->trueDescription, "%s", miscMagicItem->name);
  }
}


static void generateMiscWeapon(struct MagicItem *magicItem, struct Dice *dice)
{
  
  static struct MiscWeapon {
    int percent;
    char const *name;
    char const *quantity;
    int experiencePoints;
    int saleValue_gp;
  } const miscWeaponsTable[] = {
    {   8 -  0, "arrow +1", "2d12", 20, 120 },
    {  12 -  8, "arrow +2", "2d8", 50, 300 },
    {  14 - 12, "arrow +3", "2d6", 75, 450 },
    {  15 - 14, "arrow of slaying", "1", 250, 2500 },
    {  20 - 15, "axe +1", "1", 300, 1750 },
    {  22 - 20, "axe +2", "1", 600, 3750 },
    {  23 - 22, "axe +2, throwing", "1", 750, 4500 },
    {  24 - 23, "axe +3", "1", 1000, 7000 },
    {  27 - 24, "battle axe +1", "1", 400, 2500 },
    {  32 - 27, "bolt +2", "2d10", 50, 300 },
    {  35 - 32, "bow +1", "1", 500, 3500 },
    {  36 - 35, "crossbow of accuracy +3", "1", 2000, 12000 },
    {  37 - 36, "crossbow of distance", "1", 1500, 7500 },
    {  38 - 37, "crossbow of speed", "1", 1500, 7500 },
    {  46 - 38, "dagger +1, +2 vs creatures smaller than man-sized", "1", 100, 750 },
    {  50 - 46, "dagger +2, +3 vs creatures larger than man-sized", "1", 250, 2000 },
    {  51 - 50, "dagger of venom", "1", 350, 3000 },
    {  56 - 51, "flail +1", "1", 450, 4000 },
    {  60 - 56, "hammer +1", "1", 300, 2500 },
    {  62 - 60, "hammer +2", "1", 650, 6000 },
    {  63 - 62, "hammer +3, dwarven thrower", "1", 1500, 15000 },
    {  64 - 63, "hammer of thunderbolts", "1", 2500, 25000 },
    {  67 - 64, "javelin +2", "1", 750, 5000 },
    {  72 - 67, "mace +1", "1", 350, 3000 },
    {  75 - 72, "mace +2", "1", 700, 4500 },
    {  76 - 75, "mace of disruption", "1", 1750, 17500 },
    {  77 - 76, "mace +4", "1", 1500, 15000 },
    {  80 - 77, "military pick +1", "1", 350, 2500 },
    {  83 - 80, "morning star +1", "1", 400, 3000 },
    {  88 - 83, "scimitar +2", "1", 750, 6000 },
    {  89 - 88, "sling of seeking +2", "1", 700, 7000 },
    {  94 - 89, "spear +1", "1", 500, 3000 },
    {  96 - 94, "spear +2", "1", 1000, 6500 },
    {  97 - 96, "spear +3", "1", 1750, 15000 },
    {  99 - 97, "spear, cursed backbiter", "1", 0, 1000 },
    { 100 - 99, "trident (military fork) +3", "1", 1500, 12500 }
  };
  size_t const miscWeaponsTableCount = sizeof miscWeaponsTable 
                                     / sizeof miscWeaponsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct MiscWeapon const *miscWeapon = NULL;
  for (int i = 0; i < miscWeaponsTableCount; ++i) {
    range += miscWeaponsTable[i].percent;
    if (dieRoll <= range) {
      miscWeapon = &miscWeaponsTable[i];
      break;
    }
  }
  assert(miscWeapon);
  
  int quantity = roll(dice, miscWeapon->quantity);
  magicItem->experiencePoints = miscWeapon->experiencePoints * quantity;
  magicItem->trueValue_cp = miscWeapon->saleValue_gp * CP_PER_GP * quantity;
  
  if (1 == quantity) {
    em_asprintf(&magicItem->trueDescription, "%s", miscWeapon->name);
  } else {
    em_asprintf(&magicItem->trueDescription, "(%i) %s",
                quantity, miscWeapon->name);
  }
}


static void generateNecklaceOfMissiles(struct MagicItem *magicItem, 
                                       struct Dice *dice)
{
  char const *type;
  int totalHitDice;
  
  int dieRoll = roll(dice, "1d20");
  if (dieRoll <= 4) {
    type = "(1) 5 hd, (2) 3 hd";
    totalHitDice = (1 * 5) + (2 * 3);
  } else if (dieRoll <= 8) {
    type = "(1) 6 hd, (2) 4 hd, (2) 2 hd";
    totalHitDice = (1 * 6) + (2 * 4) + (2 * 2);
  } else if (dieRoll <= 12) {
    type = "(1) 7 hd, (2) 5 hd, (4) 3 hd";
    totalHitDice = (1 * 7) + (2 * 5) + (4 * 3);
  } else if (dieRoll <= 16) {
    type = "(1) 8 hd, (2) 6 hd, (2) 4 hd, (4) 2 hd";
    totalHitDice = (1 * 8) + (2 * 6) + (2 * 4) + (4 * 2);
  } else if (dieRoll <= 18) {
    type = "(1) 9 hd, (2) 7 hd, (2) 5 hd, (2) 3 hd";
    totalHitDice = (1 * 9) + (2 * 7) + (2 * 5) + (2 * 3);
  } else if (dieRoll <= 19) {
    type = "(1) 10 hd, (2) 8 hd, (2) 6 hd, (4) 4 hd";
    totalHitDice = (1 * 10) + (2 * 8) + (2 * 6) + (4 * 4);
  } else {
    type = "(1) 11 hd, (2) 9 hd, (2) 7 hd, (2) 5 hd, (2) 3 hd";
    totalHitDice = (1 * 11) + (2 * 9) + (2 * 7) + (2 * 5) + (2 * 3);
  }
  
  magicItem->experiencePoints = totalHitDice * 50;
  magicItem->trueValue_cp = totalHitDice * 200 * CP_PER_GP;
  em_asprintf(&magicItem->trueDescription,
              "necklace of missiles (with %s missiles)", type);
}


static void generateNecklaceOfPrayerBeads(struct MagicItem *magicItem, 
                                          struct Dice *dice)
{
# define TYPE_COUNT 6
  int specialBeadCounts[TYPE_COUNT] = {};
  int specialBeadTotal = roll(dice, "1d4+2");
  for (int i = 0; i < specialBeadTotal; ++i) {
    int dieRoll = roll(dice, "1d20");
    if (dieRoll <= 5) {
      ++specialBeadCounts[0];
    } else if (dieRoll <= 10) {
      ++specialBeadCounts[1];
    } else if (dieRoll <= 15) {
      ++specialBeadCounts[2];
    } else if (dieRoll <= 17) {
      ++specialBeadCounts[3];
    } else if (dieRoll <= 18) {
      ++specialBeadCounts[4];
    } else {
      ++specialBeadCounts[5];
    }
  }
  
  char const *types[] = {
    "atonement", "blessing", "curing", "karma", "summons", "wind walking"
  };
  size_t bufferSize = 0;
  char const separator[] = ", ";
  size_t separatorLength = strlen(separator);
  for (int i = 0; i < TYPE_COUNT; ++i) {
    bufferSize += strlen(types[i]) + separatorLength;
  }
  bufferSize += sizeof '\0';
  
  char *buffer = em_calloc(bufferSize, sizeof(char));
  char *bufferEnd = buffer;
  bool first = true;
  for (int i = 0; i < TYPE_COUNT; ++i) {
    if ( ! specialBeadCounts[i]) continue;
    
    if (first) {
      first = false;
    } else {
      strcat(bufferEnd, separator);
      bufferEnd += separatorLength;
    }
    if (specialBeadCounts[i] > 1) {
      int charsPrinted = sprintf(bufferEnd, "(%i) ", specialBeadCounts[i]);
      if (charsPrinted > 0) {
        bufferEnd += charsPrinted;
      }
    }
    strcat(bufferEnd, types[i]);
    bufferEnd += strlen(types[i]);
  }
  assert(bufferEnd < buffer + bufferSize);
  
  magicItem->experiencePoints = specialBeadTotal * 500;
  magicItem->trueValue_cp = specialBeadTotal * 3000 * CP_PER_GP;
  em_asprintf(&magicItem->trueDescription,
              "necklace of prayer beads with %s", buffer);
}


static void generateNolzursMarvelousPigments(struct MagicItem *magicItem, 
                                             struct Dice *dice)
{
  int quantity = roll(dice, "1d4");
  magicItem->experiencePoints = quantity * 500;
  magicItem->trueValue_cp = quantity * 3000 * CP_PER_GP;
  magicItem->trueDescription = STRDUP_OR_DIE("Nolzurs' marvelous pigments");
}


static void generateOrbOfDragonkind(struct MagicItem *magicItem, 
                                    struct Dice *dice)
{
  int dieRoll = roll(dice, "1d8");
  char const *type = NULL;
  switch (dieRoll) {
    case 1: type = "hatchling"; break;
    case 2: type = "wyrmkin"; break;
    case 3: type = "dragonette"; break;
    case 4: type = "dragon"; break;
    case 5: type = "great serpent"; break;
    case 6: type = "fire drake"; break;
    case 7: type = "elder wyrm"; break;
    case 8: type = "eternal grand dragon"; break;
    default: UNEXPECTED("1d8 die roll is %i", dieRoll); break;
  }
  magicItem->experiencePoints = 0;
  magicItem->trueValue_cp = dieRoll * 10000 * CP_PER_GP;
  em_asprintf(&magicItem->trueDescription,
              "orb of dragonkind: orb of the %s", type);
}


static void generatePearlOfPower(struct MagicItem *magicItem, 
                                 struct Dice *dice)
{
  int spellQuantity = 1;
  int spellLevel;
  
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 25) {
    spellLevel = 1;
  } else if (dieRoll <= 45) {
    spellLevel = 2;
  } else if (dieRoll <= 60) {
    spellLevel = 3;
  } else if (dieRoll <= 75) {
    spellLevel = 4;
  } else if (dieRoll <= 85) {
    spellLevel = 5;
  } else if (dieRoll <= 92) {
    spellLevel = 6;
  } else if (dieRoll <= 96) {
    spellLevel = 7;
  } else if (dieRoll <= 98) {
    spellLevel = 8;
  } else if (dieRoll <= 99) {
    spellLevel = 9;
  } else {
    spellQuantity = 2;
    spellLevel = roll(dice, "1d6");
  }
  
  char const *effect;
  if (1 == roll(dice, "1d20")) {
    effect = "(cursed) forget";
  } else {
    effect = "recall";
  }
  
  magicItem->experiencePoints = spellLevel * spellQuantity * 200;
  magicItem->trueValue_cp = spellLevel * spellQuantity * 2000 * CP_PER_GP;
  
  char const *plural = (spellQuantity == 1) ? "" : "s";
  em_asprintf(&magicItem->trueDescription,
              "pearl of power: %s %i level %i spell%s",
              effect, spellQuantity, spellLevel, plural);
}


static void generatePotion(struct MagicItem *magicItem, struct Dice *dice)
{
  static struct Potion {
    int percent;
    char const *name;
    int experiencePoints;
    int saleValue_gp;
    ClassRestrictions classRestrictions;
    GenerateFunction generate;
  } const potionsTable[] = {
    {   3 -  0, "animal control", 250, 400 },
    {   6 -  3, "clairaudience", 250, 400 },
    {   9 -  6, "clairvoyance", 300, 500 },
    {  12 -  9, "climbing", 300, 500 },
    {  15 - 12, "delusion", 0, 150 },
    {  18 - 15, "diminution", 300, 500 },
    {  20 - 18, "dragon control", 500, 5000, NO_CLASS_RESTRICTIONS, generatePotionOfDragonControl },
    {  23 - 20, "ESP", 500, 850 },
    {  26 - 23, "extra-healing", 400, 800 },
    {  29 - 26, "fire resistance", 250, 400 },
    {  32 - 29, "flying", 500, 750 },
    {  34 - 32, "gaseous form", 300, 400 },
    {  36 - 34, "giant control", 400, 1000, NO_CLASS_RESTRICTIONS, generatePotionOfGiantControl },
    {  39 - 36, "giant strength", 500, 900, FIGHTERS, generatePotionOfGiantStrength },
    {  41 - 39, "growth", 250, 300 },
    {  47 - 41, "healing", 200, 400 },
    {  49 - 47, "heroism", 300, 500, FIGHTERS },
    {  51 - 49, "human control", 500, 900 },
    {  54 - 51, "invisibility", 250, 500 },
    {  57 - 54, "invulnerability", 350, 500, FIGHTERS },
    {  60 - 57, "levitation", 250, 400 },
    {  63 - 60, "longevity", 500, 1000 },
    {  66 - 63, "oil of etherealness", 600, 1500 },
    {  69 - 66, "oil of slipperiness", 400, 750 },
    {  72 - 69, "philter of love", 200, 300 },
    {  75 - 72, "philter of persuasiveness", 400, 850 },
    {  78 - 75, "plant control", 250, 300 },
    {  81 - 78, "polymorph (self)", 200, 350 },
    {  84 - 81, "poison", 0, 0 },
    {  87 - 84, "speed", 200, 450 },
    {  90 - 87, "super-heroism", 450, 750, FIGHTERS },
    {  93 - 90, "sweet water", 200, 250 },
    {  96 - 93, "treasure finding", 600, 2000 },
    {  97 - 96, "undead control", 700, 2500 },
    { 100 - 97, "water breathing", 400, 900 }
  };
  size_t const potionsTableCount = sizeof potionsTable / sizeof potionsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct Potion const *potion = NULL;
  for (int i = 0; i < potionsTableCount; ++i) {
    range += potionsTable[i].percent;
    if (dieRoll <= range) {
      potion = &potionsTable[i];
      break;
    }
  }
  assert(potion);
  
  if (potion->generate) {
    potion->generate(magicItem, dice);
  } else {
    magicItem->experiencePoints = potion->experiencePoints;
    magicItem->trueValue_cp = potion->saleValue_gp * CP_PER_GP;
    em_asprintf(&magicItem->trueDescription, "%s potion", potion->name);
  }
}


static void generatePotionOfDragonControl(struct MagicItem *magicItem, 
                                          struct Dice *dice)
{
  char const *type;
  
  int dieRoll = roll(dice, "1d20");
  if (dieRoll <= 2) {
    type = "white";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 4) {
    type = "black";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 7) {
    type = "green";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 9) {
    type = "blue";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 10) {
    type = "red";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 12) {
    type = "brass";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 14) {
    type = "copper";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 15) {
    type = "bronze";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 16) {
    type = "silver";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 17) {
    type = "gold";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 5000 * CP_PER_GP;
  } else if (dieRoll <= 19) {
    type = "evil";
    magicItem->experiencePoints = 1000;
    magicItem->trueValue_cp = 9000 * CP_PER_GP;
  } else {
    type = "good";
    magicItem->experiencePoints = 1000;
    magicItem->trueValue_cp = 9000 * CP_PER_GP;
  }
  
  em_asprintf(&magicItem->trueDescription, "potion of %s dragon control", type);
}


static void generatePotionOfGiantControl(struct MagicItem *magicItem, 
                                         struct Dice *dice)
{
  char const *type;
  int level;
  
  int dieRoll = roll(dice, "1d20");
  if (dieRoll <= 5) {
    type = "hill";
    level = 1;
  } else if (dieRoll <= 9) {
    type = "stone";
    level = 2;
  } else if (dieRoll <= 13) {
    type = "frost";
    level = 3;
  } else if (dieRoll <= 17) {
    type = "fire";
    level = 4;
  } else if (dieRoll <= 19) {
    type = "cloud";
    level = 5;
  } else {
    type = "storm";
    level = 6;
  }
  
  magicItem->experiencePoints = level * 100 + 300;
  magicItem->trueValue_cp = level * 1000 * CP_PER_GP;
  em_asprintf(&magicItem->trueDescription, "potion of %s giant control", type);
}


static void generatePotionOfGiantStrength(struct MagicItem *magicItem, 
                                          struct Dice *dice)
{
  char const *type;
  int level;
  
  int dieRoll = roll(dice, "1d20");
  if (dieRoll <= 6) {
    type ="hill";
    level = 1;
  } else if (dieRoll <= 10) {
    type = "stone";
    level = 2;
  } else if (dieRoll <= 14) {
    type = "frost";
    level = 3;
  } else if (dieRoll <= 17) {
    type = "fire";
    level = 4;
  } else if (dieRoll <= 19) {
    type = "cloud";
    level = 5;
  } else {
    type = "storm";
    level = 6;
  }
  
  magicItem->experiencePoints = level * 50 + 450;
  magicItem->trueValue_cp = (level * 100 + 800) * CP_PER_GP;
  em_asprintf(&magicItem->trueDescription, "potion of %s giant strength", type);
}


static void generateQuaalsFeatherToken(struct MagicItem *magicItem, 
                                       struct Dice *dice)
{
  char const *type;
  
  int dieRoll = roll(dice, "1d20");
  if (dieRoll <= 4) {
    type = "anchor";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 2000 * CP_PER_GP;
  } else if (dieRoll <= 7) {
    type = "bird";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 2000 * CP_PER_GP;
  } else if (dieRoll <= 10) {
    type = "fan";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 2000 * CP_PER_GP;
  } else if (dieRoll <= 13) {
    type = "swan boat";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 2000 * CP_PER_GP;
  } else if (dieRoll <= 18) {
    type = "tree";
    magicItem->experiencePoints = 500;
    magicItem->trueValue_cp = 2000 * CP_PER_GP;
  } else {
    type = "whip";
    magicItem->experiencePoints = 1000;
    magicItem->trueValue_cp = 7000 * CP_PER_GP;
  }
  
  em_asprintf(&magicItem->trueDescription, "Quaal's feather token: %s", type);
}


static void generateRing(struct MagicItem *magicItem, struct Dice *dice)
{
  static struct Ring {
    int percent;
    char const *name;
    bool limitedCharges;
    int experiencePoints;
    int saleValue_gp;
    ClassRestrictions classRestrictions;
    GenerateFunction generate;
  } const ringsTable[] = {
    {   6 -  0, "contrariness", false, 0, 1000 },
    {  12 -  6, "delusion", false, 0, 2000 },
    {  14 - 12, "djinni summoning", true, 3000, 20000 },
    {  15 - 14, "elemental control", false, 5000, 25000 },
    {  21 - 15, "feather falling", false, 1000, 5000 },
    {  27 - 21, "fire resistance", false, 1000, 5000 },
    {  30 - 27, "free action", false, 1000, 5000 },
    {  33 - 30, "human influence", true, 2000, 10000 },
    {  40 - 33, "invisibility", false, 1500, 7500 },
    {  43 - 40, "mammal control", true, 1000, 5000 },
    {  44 - 43, "multiple wishes", true, 5000, 25000 },
    {  60 - 44, "protection", false, 2000, 10000, NO_CLASS_RESTRICTIONS, generateRingOfProtection },
    {  61 - 60, "regeneration", false, 5000, 40000 },
    {  63 - 61, "shooting stars", false, 3000, 15000 },
    {  65 - 63, "spell storing", false, 2500, 22500 },
    {  69 - 65, "spell turning", false, 2000, 17500 },
    {  75 - 69, "swimming", false, 1000, 5000 },
    {  77 - 75, "telekinesis", true, 2000, 10000 },
    {  79 - 77, "three wishes", true, 3000, 15000 },
    {  85 - 79, "warmth", false, 1000, 5000 },
    {  90 - 85, "water walking", false, 1000, 5000 },
    {  98 - 90, "weakness", false, 0, 1000 },
    {  99 - 98, "wizardry", true, 4000, 50000, MAGIC_USERS },
    { 100 - 99, "x-ray vision", false, 4000, 35000 }
  };
  size_t const ringsTableCount = sizeof ringsTable / sizeof ringsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct Ring const *ring = NULL;
  for (int i = 0; i < ringsTableCount; ++i) {
    range += ringsTable[i].percent;
    if (dieRoll <= range) {
      ring = &ringsTable[i];
      break;
    }
  }
  assert(ring);
    
  if (ring->generate) {
    ring->generate(magicItem, dice);
  } else {
    magicItem->experiencePoints = ring->experiencePoints;
    magicItem->trueValue_cp = ring->saleValue_gp * CP_PER_GP;
    em_asprintf(&magicItem->trueDescription, "ring of %s", ring->name);
  }
}


static void generateRingOfProtection(struct MagicItem *magicItem, 
                                     struct Dice *dice)
{
  char const *type;
  
  int dieRoll = roll(dice, "1d100");
  if (dieRoll <= 70) {
    type = "+1";
    magicItem->experiencePoints = 2000;
  } else if (dieRoll <= 82) {
    type = "+2";
    magicItem->experiencePoints = 2500;
  } else if (dieRoll <= 83) {
    type = "+2, 5 ft radius";
    magicItem->experiencePoints = 2500;
  } else if (dieRoll <= 90) {
    type = "+3";
    magicItem->experiencePoints = 3000;
  } else if (dieRoll <= 91) {
    type = "+3, 5 ft radius";
    magicItem->experiencePoints = 3500;
  } else if (dieRoll <= 97) {
    type = "+4 on AC, +2 saving throws";
    magicItem->experiencePoints = 3500;
  } else {
    type = "+6 on AC, +1 saving throws";
    magicItem->experiencePoints = 4000;
  }
  
  magicItem->trueValue_cp = magicItem->experiencePoints * 5 * CP_PER_GP;
  em_asprintf(&magicItem->trueDescription, "ring of protection %s", type);
}


static void generateRodStaffOrWand(struct MagicItem *magicItem, 
                                   struct Dice *dice)
{
  static struct RodStaffOrWand {
    int percent;
    char const *name;
    int experiencePoints;
    int saleValue_gp;
    ClassRestrictions classRestrictions;
  } const rodsStavesAndWandsTable[] = {
    {   3 -  0, "rod of absorption", 7500, 40000, CLERICS | MAGIC_USERS },
    {   4 -  3, "rod of beguiling", 5000, 30000, CLERICS | MAGIC_USERS | THIEVES },
    {  14 -  4, "rod of cancellation", 10000, 15000 },
    {  16 - 14, "rod of lordly might", 6000, 20000, FIGHTERS },
    {  17 - 16, "rod of resurrection", 10000, 35000, CLERICS },
    {  18 - 17, "rod of rulership", 8000, 35000 },
    {  19 - 18, "rod of smiting", 4000, 15000, CLERICS | FIGHTERS },
    {  20 - 19, "staff of command", 5000, 25000, CLERICS | MAGIC_USERS },
    {  22 - 20, "staff of curing", 6000, 25000, CLERICS },
    {  23 - 22, "staff of the magi", 15000, 75000, MAGIC_USERS },
    {  24 - 23, "staff of power", 12000, 60000, MAGIC_USERS },
    {  27 - 24, "staff of the serpent", 7000, 35000, CLERICS },
    {  31 - 27, "staff of striking", 6000, 15000, CLERICS | MAGIC_USERS },
    {  33 - 31, "staff of withering", 8000, 35000, CLERICS },
    {  34 - 33, "wand of conjuration", 7000, 35000, MAGIC_USERS },
    {  38 - 34, "wand of enemy detection", 2000, 10000 },
    {  41 - 38, "wand of fear", 3000, 15000, CLERICS | MAGIC_USERS },
    {  44 - 41, "wand of fire", 4500, 25000, MAGIC_USERS },
    {  47 - 44, "wand of frost", 6000, 50000, MAGIC_USERS },
    {  52 - 47, "wand of illumination", 2000, 10000 },
    {  56 - 52, "wand of illusion", 3000, 20000, MAGIC_USERS },
    {  59 - 56, "wand of lightning", 4000, 30000, MAGIC_USERS },
    {  68 - 59, "wand of magic detection", 2500, 25000 },
    {  73 - 68, "wand of metal & mineral detection", 1500, 7500 },
    {  78 - 73, "wand of magic missiles", 4000, 35000 },
    {  86 - 78, "wand of negation", 3500, 15000 },
    {  89 - 86, "wand of paralyzation", 3500, 25000, MAGIC_USERS },
    {  92 - 89, "wand of polymorphing", 3500, 25000, MAGIC_USERS },
    {  94 - 92, "wand of secret door & trap location", 5000, 40000 },
    { 100 - 94, "wand of wonder", 6000, 10000 }
  };
  size_t const rodsStavesAndWandsTableCount = sizeof rodsStavesAndWandsTable 
                                            / sizeof rodsStavesAndWandsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct RodStaffOrWand const *rodStaffOrWand = NULL;
  for (int i = 0; i < rodsStavesAndWandsTableCount; ++i) {
    range += rodsStavesAndWandsTable[i].percent;
    if (dieRoll <= range) {
      rodStaffOrWand = &rodsStavesAndWandsTable[i];
      break;
    }
  }
  
  magicItem->experiencePoints = rodStaffOrWand->experiencePoints;
  magicItem->trueValue_cp = rodStaffOrWand->saleValue_gp * CP_PER_GP;
  
  em_asprintf(&magicItem->trueDescription, "%s", rodStaffOrWand->name);
}


static void generateScroll(struct MagicItem *magicItem, struct Dice *dice)
{
  
  static struct Scroll {
    int percent;
    char const *name;
    bool isSpellScroll;
    int spellCount;
    char const *magicUserSpellLevelRange;
    char const *clericalSpellLevelRange;
    int experiencePoints;
    int saleValue_gp;
  } const scrollsTable[] = {
    {  10 - 00, "(spell scroll)", true, 1, "1d4", "1d4", 100, 300 },
    {  16 - 10, "(spell scroll)", true, 1, "1d6", "1d6", 100, 300 },
    {  19 - 16, "(spell scroll)", true, 1, "1d8+1", "1d6+1", 100, 300 },
    {  24 - 19, "(spell scroll)", true, 2, "1d4", "1d4", 100, 300 },
    {  27 - 24, "(spell scroll)", true, 2, "1d8", "1d6", 100, 300 },
    {  32 - 27, "(spell scroll)", true, 3, "1d4", "1d4", 100, 300 },
    {  35 - 32, "(spell scroll)", true, 3, "1d8+1", "1d6+1", 100, 300 },
    {  39 - 35, "(spell scroll)", true, 4, "1d6", "1d6", 100, 300 },
    {  42 - 39, "(spell scroll)", true, 4, "1d8", "1d6", 100, 300 },
    {  46 - 42, "(spell scroll)", true, 5, "1d6", "1d6", 100, 300 },
    {  49 - 46, "(spell scroll)", true, 5, "1d8", "1d6", 100, 300 },
    {  52 - 49, "(spell scroll)", true, 6, "1d6", "1d6", 100, 300 },
    {  54 - 52, "(spell scroll)", true, 6, "1d6+2", "1d4+2", 100, 300 },
    {  57 - 54, "(spell scroll)", true, 7, "1d8", "1d6", 100, 300 },
    {  59 - 57, "(spell scroll)", true, 7, "1d8+1", "1d6+1", 100, 300 },
    {  60 - 59, "(spell scroll)", true, 7, "1d6+3", "1d4+3", 100, 300 },
    {  62 - 60, "protection from demons", false, 1, "0", "0", 2500, 2500 * 5 },
    {  64 - 62, "protection from devils", false, 1, "0", "0", 2500, 2500 * 5 },
    {  70 - 64, "protection from elementals", false, 1, "0", "0", 1500, 1500 * 5 },
    {  76 - 70, "protection from lycanthropes", false, 1, "0", "0", 1000, 1000 * 5 },
    {  82 - 76, "protection from magic", false, 1, "0", "0", 1500, 1500 * 5 },
    {  87 - 82, "protection from petrification", false, 1, "0", "0", 2000, 2000 * 5 },
    {  92 - 87, "protection from possession", false, 1, "0", "0", 2000, 2000 * 5 },
    {  97 - 92, "protection from undead", false, 1, "0", "0", 1500, 1500 * 5 },
    { 100 - 97, "cursed", false, 1, "0", "0" }
  };
  size_t const scrollsTableCount = sizeof scrollsTable / sizeof scrollsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct Scroll const *scroll = NULL;
  for (int i = 0; i < scrollsTableCount; ++i) {
    range += scrollsTable[i].percent;
    if (dieRoll <= range) {
      scroll = &scrollsTable[i];
      break;
    }
  }
  assert(scroll);
  
  if (scroll->isSpellScroll) {
    magicItem->trueDetails = em_calloc(scroll->spellCount + 1, sizeof(char *));
    
    enum SpellType spellType;
    char const *spellTypeName;
    char const *spellLevelRange = scroll->clericalSpellLevelRange;
    dieRoll = roll(dice, "1d100");
    if (dieRoll <= 70) {
      if (roll(dice, "1d100") <= 10) {
        spellType = IllusionistSpellType;
        spellTypeName = "illusionist";
      } else {
        spellType = MagicUserSpellType;
        spellTypeName = "magic-user";
        spellLevelRange = scroll->magicUserSpellLevelRange;
      }
    } else {
      if (roll(dice, "1d100") <= 25) {
        spellType = DrudicalSpellType;
        spellTypeName = "druidical";
      } else { 
        spellType = ClericalSpellType;
        spellTypeName = "clerical";
      }
    }
    
    int spellLevels = 0;
    for (int i = 0; i < scroll->spellCount; ++i) {
      int spellLevel = roll(dice, spellLevelRange);
      char const *spellName = determineSpell(dice, spellType, spellLevel);
      em_asprintf(&magicItem->trueDetails[i], "level %i: %s",
                  spellLevel, spellName);
      spellLevels += spellLevel;
    }
    
    magicItem->experiencePoints = scroll->experiencePoints * spellLevels;
    magicItem->trueValue_cp = scroll->saleValue_gp * spellLevels * CP_PER_GP;
    
    char const *plural = scroll->spellCount == 1 ? "" : "s";
    em_asprintf(&magicItem->trueDescription, "%s scroll (%i spell%s)",
                spellTypeName, scroll->spellCount, plural);
  } else {
    magicItem->experiencePoints = scroll->experiencePoints;
    magicItem->trueValue_cp = scroll->saleValue_gp * CP_PER_GP;
    em_asprintf(&magicItem->trueDescription, "%s scroll", scroll->name);
  }
}


static void generateSword(struct MagicItem *magicItem, struct Dice *dice)
{
  static struct Sword {
    int percent;
    char const *nameFormat;
    char const *details[4];
    int baseEgo;
    int experiencePoints;
    int saleValue_gp;
  } const swordsTable[] = {
    {  25 -  0, "%s +1", {}, 1, 400, 2000 },
    {  30 - 25, "%s +1", 
                { "+2 vs magic-using & enchanted creatures" }, 3, 600, 3000 },
    {  35 - 30, "%s +1", 
                { "+3 vs lycanthropes & shape changers" }, 4, 700, 3500 },
    {  40 - 35, "%s +1", 
                { "+3 vs regenerating creatures" }, 4, 800, 4000 },
    {  45 - 40, "%s +1", 
                { "+4 vs reptiles" }, 5, 800, 4000 },
    {  49 - 45, "%s +1, flame tongue", 
                { "+2 vs regenerating creatures", 
                  "+3 vs cold-using, inflammable or avian creatures", 
                  "+4 vs undead" }, 5, 900, 4500 },
    {  50 - 49, "%s +1, luck blade", {}, 2, 1000, 5000 },
    {  58 - 50, "%s +2", {}, 2, 800, 4000 },
    {  62 - 58, "%s +2, giant slayer", {}, 4, 900, 4500 },
    {  66 - 62, "%s +2, dragon slayer", {}, 4, 900, 4500 },
    {  67 - 66, "%s +2, nine lives stealer", {}, 4, 1600, 8000 },
    {  71 - 67, "%s +3", {}, 3, 1400, 7000 },
    {  74 - 71, "%s +3, frost brand", 
                { "+6 vs fire using/dwelling creatures" }, 9, 1600, 8000 },
    {  76 - 74, "%s +4", {}, 4, 2000, 10000 },
    {  77 - 76, "%s +4, defender", {}, 8, 3000, 15000 },
    {  78 - 77, "%s +5", {}, 5, 3000, 15000 },
    {  79 - 78, "%s +5, defender", {}, 10, 3600, 18000 },
    {  80 - 79, "%s +5, holy avenger", {}, 10, 4000, 20000 },
    {  81 - 80, "%s of dancing", {}, 8, 4400, 22000 },
    {  82 - 81, "%s of wounding", {}, 2, 4400, 22000 },
    {  83 - 82, "%s of life stealing", {}, 4, 5000, 25000 },
    {  84 - 83, "%s of sharpness", {}, 6, 7000, 35000 },
    {  85 - 84, "%s, vorpal weapon", {}, 6, 10000, 50000 },
    {  90 - 85, "%s +1, cursed", {}, 1, 400, 0 },
    {  95 - 90, "%s -2, cursed", {}, 2, 600, 0 },
    { 100 - 95, "%s, cursed berserking", {}, 4, 900, 0 }
  };
  size_t const swordsTableCount = sizeof swordsTable / sizeof swordsTable[0];
  
  int dieRoll = roll(dice, "1d100");
  int range = 0;
  struct Sword const *sword = NULL;
  for (int i = 0; i < swordsTableCount; ++i) {
    range += swordsTable[i].percent;
    if (dieRoll <= range) {
      sword = &swordsTable[i];
      break;
    }
  }
  assert(sword);
  
  magicItem->experiencePoints = sword->experiencePoints;
  magicItem->trueValue_cp = sword->saleValue_gp * CP_PER_GP;
  
  char const *swordType;
  dieRoll = roll(dice, "1d100");
  if (dieRoll <= 70) {
    swordType = "longsword";
  } else if (dieRoll <= 90) {
    swordType = "broadsword";
  } else if (dieRoll <= 95) {
    swordType = "short sword";
  } else if (dieRoll <= 99) {
    swordType = "bastard sword";
  } else {
    swordType = "two-handed sword";
  }
  em_asprintf(&magicItem->trueDescription, sword->nameFormat, swordType);
  
  int detailCount = 0;
  while (sword->details[detailCount]) {
    ++detailCount;
  }
  if (detailCount) {
    magicItem->trueDetails = em_calloc(detailCount + 1, sizeof(char *));
    for (int i = 0; i < detailCount; ++i) {
      magicItem->trueDetails[i] = STRDUP_OR_DIE(sword->details[i]);
    }
  }
  
  int ego = sword->baseEgo;
  int intelligence = 0;
  int primaryAbilityCount = 0;
  int extraordinaryPowerCount = 0;
  char const *communication = NULL;
  
  dieRoll = roll(dice, "1d100");
  if (dieRoll <= 75) {
    /* not an unusual magic sword */
    return;
  } else if (dieRoll <= 83) {
    intelligence = 12;
    primaryAbilityCount = 1;
    communication = "semi-empathy";
  } else if (dieRoll <= 89) {
    intelligence = 13;
    primaryAbilityCount = 2;
    communication = "empathy";
  } else if (dieRoll <= 94) {
    intelligence = 14;
    primaryAbilityCount = 2;
    communication = "speech";
  } else if (dieRoll <= 97) {
    intelligence = 15;
    primaryAbilityCount = 3;
    communication = "speech";
  } else if (dieRoll <= 99) {
    intelligence = 16;
    primaryAbilityCount = 3;
    communication = "speech";
    ego += 1; /* read languages */
  } else {
    intelligence = 17;
    primaryAbilityCount = 3;
    extraordinaryPowerCount = 1;
    communication = "speech and telepathy";
    ego += 1; /* read languages */
    ego += 2; /* read magic */
    ego += 2; /* telepathy */
  }
  assert(intelligence);
  
  char *trueDescription = magicItem->trueDescription;
  em_asprintf(&magicItem->trueDescription, "%s (unusual)", trueDescription);
  free(trueDescription);
  
  int detailCapacity = detailCount +  
                     + 2 /* intelligence, alignment */
                     + 6 /* up to 6 primary abilities */
                     + 3 /* 2 extraordinary powers or 1 power + special purpose & power */
                     + 2 /* languages list, ego points */
                     + 1 /* NULL */;
  if ( ! magicItem->trueDetails) {
    magicItem->trueDetails = em_calloc(detailCapacity, sizeof(char *));
  } else {
    magicItem->trueDetails = REALLOC_OR_DIE(magicItem->trueDetails, 
                                            detailCapacity * sizeof(char *));
    char **startOfUnused = magicItem->trueDetails + detailCount;
    size_t sizeOfUnused = (detailCapacity - detailCount) * sizeof(char *);
    memset(startOfUnused, 0, sizeOfUnused);
  }
  
  em_asprintf(&magicItem->trueDetails[detailCount], "intelligence %i (%s)",
              intelligence, communication);
  ++detailCount;
  assert(detailCount < detailCapacity);
  
  char const *alignment;
  dieRoll = roll(dice, "1d100");
  if (dieRoll <= 5) {
    alignment = "chaotic good";
  } else if (dieRoll <= 15) {
    alignment = "chaotic neutral";
  } else if (dieRoll <= 20) {
    alignment = "chaotic evil";
  } else if (dieRoll <= 25) {
    alignment = "neutral evil";
  } else if (dieRoll <= 30) {
    alignment = "lawful evil";
  } else if (dieRoll <= 55) {
    alignment = "lawful good";
  } else if (dieRoll <= 60) {
    alignment = "lawful neutral";
  } else if (dieRoll <= 80) {
    alignment = "neutral";
  } else {
    alignment = "neutral good";
  }
  em_asprintf(&magicItem->trueDetails[detailCount], "%s alignment", alignment);
  ++detailCount;
  assert(detailCount < detailCapacity);
  
  struct PrimaryAbilities {
    int count;
    double detectElevators;
    double detectSlopingPassages;
    double detectTraps;
    double detectEvilGood;
    double detectPreciousMetals;
    double detectGems;
    double detectMagic;
    double detectSecretDoors;
    double detectInvisible;
    double locateObject;
  } primaryAbilities = {};
  for (int i = 0; i < primaryAbilityCount; ++i) {
    int dieRollMax = 100;
    int rolls = 1;
    do {
      dieRoll = rollDice(dice, 1, dieRollMax);
      --rolls;
      if (dieRoll <= 11) {
        ++primaryAbilities.count;
        primaryAbilities.detectElevators += 1;
      } else if (dieRoll <= 22) {
        ++primaryAbilities.count;
        primaryAbilities.detectSlopingPassages += 1;
      } else if (dieRoll <= 33) {
        ++primaryAbilities.count;
        primaryAbilities.detectTraps += 1;
      } else if (dieRoll <= 44) {
        ++primaryAbilities.count;
        primaryAbilities.detectEvilGood += 1;
      } else if (dieRoll <= 55) {
        ++primaryAbilities.count;
        primaryAbilities.detectPreciousMetals += 2;
      } else if (dieRoll <= 66) {
        ++primaryAbilities.count;
        primaryAbilities.detectGems += 0.5;
      } else if (dieRoll <= 77) {
        ++primaryAbilities.count;
        primaryAbilities.detectMagic += 1;
      } else if (dieRoll <= 82) {
        ++primaryAbilities.count;
        primaryAbilities.detectSecretDoors += 0.5;
      } else if (dieRoll <= 87) {
        ++primaryAbilities.count;
        primaryAbilities.detectInvisible += 1;
      } else if (dieRoll <= 92) {
        ++primaryAbilities.count;
        primaryAbilities.locateObject += 12;
      } else if (dieRoll <= 98) {
        rolls = 2;
        dieRollMax = 92;
      } else {
        ++extraordinaryPowerCount;
      }
    } while (rolls > 0);
  }
  ego += primaryAbilities.count;
  
  if (primaryAbilities.detectElevators) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "detect \"elevator\"/shifting rooms/walls in a %.0f\" radius",
                primaryAbilities.detectElevators);
    ++detailCount;
  }
  if (primaryAbilities.detectSlopingPassages) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "detect sloping passages in a %.0f\" radius",
                primaryAbilities.detectSlopingPassages);
    ++detailCount;
  }
  if (primaryAbilities.detectTraps) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "detect traps of large size in a %.0f\" radius",
                primaryAbilities.detectTraps);
    ++detailCount;
  }
  if (primaryAbilities.detectEvilGood) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "detect evil/good in a %.0f\" radius",
                primaryAbilities.detectEvilGood);
    ++detailCount;
  }
  if (primaryAbilities.detectPreciousMetals) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "detect precious metals, kind and amount in a %.0f\" radius",
                primaryAbilities.detectPreciousMetals);
    ++detailCount;
  }
  if (primaryAbilities.detectGems) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "detect gems, kind and number in a %.1f\" radius",
                primaryAbilities.detectGems);
    ++detailCount;
  }
  if (primaryAbilities.detectMagic) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "detect magic in a %.0f\" radius",
                primaryAbilities.detectMagic);
    ++detailCount;
  }
  if (primaryAbilities.detectSecretDoors) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "detect secret doors in a %.1f\" radius",
                primaryAbilities.detectSecretDoors);
    ++detailCount;
  }
  if (primaryAbilities.detectInvisible) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "detect invisible objects in a %.0f\" radius",
                primaryAbilities.detectInvisible);
    ++detailCount;
  }
  if (primaryAbilities.locateObject) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "locate object in a %.0f\" radius",
                primaryAbilities.locateObject);
    ++detailCount;
  }
  assert(detailCount < detailCapacity);
  
  struct ExtraordinaryPowers {
    int count;
    int charmPerson;
    int clairaudience;
    int clairvoyance;
    int determineDirections;
    int esp;
    int flying;
    int heal;
    int illusion;
    int levitation;
    int strength;
    int telekinesis;
    int telepathy;
    int teleportation;
    int xrayVision;
  } extraordinaryPowers = {};
  bool hasSpecialPurpose = false;
  for (int i = 0; i < extraordinaryPowerCount; ++i) {
    int dieRollMax = 100;
    int rolls = 1;
    bool rollTwicePossible = true;
    bool chooseOnePossible = true;
    bool specialPurposePossible = true;
    do {
      dieRoll = rollDice(dice, 1, dieRollMax);
      --rolls;
      if (dieRoll <= 7) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.charmPerson += 3;
      } else if (dieRoll <= 15) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.clairaudience += 3;
      } else if (dieRoll <= 22) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.clairvoyance += 3;
      } else if (dieRoll <= 28) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.determineDirections += 2;
      } else if (dieRoll <= 34) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.esp += 3;
      } else if (dieRoll <= 41) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.flying += 1;
      } else if (dieRoll <= 47) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.heal += 1;
      } else if (dieRoll <= 54) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.illusion += 2;
      } else if (dieRoll <= 61) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.levitation += 3;
      } else if (dieRoll <= 67) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.strength += 1;
      } else if (dieRoll <= 75) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.telekinesis += 2;
      } else if (dieRoll <= 81) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.telepathy += 2;
      } else if (dieRoll <= 88) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.teleportation += 1;
      } else if (dieRoll <= 94) {
        ++extraordinaryPowers.count;
        extraordinaryPowers.xrayVision += 2;
      } else {
        int range = 94;
        
        int const rollTwiceRange = 3;
        if (rollTwicePossible) {
          range += rollTwiceRange;
        }
        if (dieRoll <= range) {
          rollTwicePossible = false;
          rolls = 2;
          dieRollMax -= rollTwiceRange;
        }
        
        int const chooseOneRange = 2;
        if (chooseOnePossible) {
          range += chooseOneRange;
        }
        if (dieRoll <= range) {
          chooseOnePossible = false;
          rolls = 1;
          dieRollMax -= chooseOneRange;
        }
        
        int const specialPurposeRange = 1;
        if (specialPurposePossible) {
          range += specialPurposeRange;
        }
        if (dieRoll <= range) {
          specialPurposePossible = false;
          hasSpecialPurpose = true;
          rolls = 1;
          dieRollMax -= specialPurposeRange;
        }

      }
    } while (rolls > 0);
  }
  ego += extraordinaryPowers.count * 2;
  
  if (extraordinaryPowers.charmPerson) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "charm person on contact - %i times/day",
                extraordinaryPowers.charmPerson);
    ++detailCount;
  }
  if (extraordinaryPowers.clairaudience) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "clairaudience, 3\" range - %i times/day, 1 round per use",
                extraordinaryPowers.clairaudience);
    ++detailCount;
  }
  if (extraordinaryPowers.clairvoyance) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "clairvoyance, 3\" range - %i times/day, 1 round per use",
                extraordinaryPowers.clairvoyance);
    ++detailCount;
  }
  if (extraordinaryPowers.determineDirections) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "determine directions and depth - %i times/day",
                extraordinaryPowers.determineDirections);
    ++detailCount;
  }
  if (extraordinaryPowers.esp) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "ESP, 3\" range - %i times/day, 1 round per use",
                extraordinaryPowers.esp);
    ++detailCount;
  }
  if (extraordinaryPowers.flying) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "flying, 12\"/turn - %i hours/day",
                extraordinaryPowers.flying);
    ++detailCount;
  }
  if (extraordinaryPowers.heal) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "heal - %i times/day",
                extraordinaryPowers.heal);
    ++detailCount;
  }
  if (extraordinaryPowers.illusion) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "illusion, 12\" range - %i times/day, as the wand",
                extraordinaryPowers.illusion);
    ++detailCount;
  }
  if (extraordinaryPowers.levitation) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "levitation, 1 turn duration - %i times/day, at 6th level of magic use ability",
                extraordinaryPowers.levitation);
    ++detailCount;
  }
  if (extraordinaryPowers.strength) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "strength - %i times/day (upon wielder only)",
                extraordinaryPowers.strength);
    ++detailCount;
  }
  if (extraordinaryPowers.telekinesis) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "telekinesis, 2,500 gp wt maximum - %i times/day, 1 round each use",
                extraordinaryPowers.telekinesis);
    ++detailCount;
  }
  if (extraordinaryPowers.telepathy) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "telepathy, 6\" range - %i times/day",
                extraordinaryPowers.telepathy);
    ++detailCount;
  }
  if (extraordinaryPowers.teleportation) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "teleportation - %i times/day, 6,000 gp wt maximum, 2 segments to activate",
                extraordinaryPowers.teleportation);
    ++detailCount;
  }
  if (extraordinaryPowers.xrayVision) {
    em_asprintf(&magicItem->trueDetails[detailCount],
                "X-ray vision, 4\" range - %i times/day, 1 turn per use",
                extraordinaryPowers.xrayVision);
    ++detailCount;
  }
  assert(detailCount < detailCapacity);
  
  if (hasSpecialPurpose) {
    ego += 5;
    char const *specialPurpose;
    dieRoll = roll(dice, "1d100");
    if (dieRoll <= 10) {
      if (0 == strcmp(alignment, "neutral")) {
        specialPurpose = "preserve the balance by defeating/slaying powerful beings of extreme alignment (LG, LE, CG, CE)";
      } else {
        specialPurpose = "defeat/slay diametrically opposed alignment";
      }
    } else if (dieRoll <= 20) {
      specialPurpose = "kill clerics";
    } else if (dieRoll <= 30) {
      specialPurpose = "kill fighters";
    } else if (dieRoll <= 40) {
      specialPurpose = "kill magic-users";
    } else if (dieRoll <= 50) {
      specialPurpose = "kill thieves";
    } else if (dieRoll <= 55) {
      specialPurpose = "kill bards/monks";
    } else if (dieRoll <= 65) {
      specialPurpose = "overthrow law and/or chaos";
    } else if (dieRoll <= 75) {
      specialPurpose = "slay good and/or evil";
    } else {
      specialPurpose = "slay non-human monsters";
    }
    em_asprintf(&magicItem->trueDetails[detailCount],
                "special purpose: %s", specialPurpose);
    ++detailCount;
    assert(detailCount < detailCapacity);
    
    char const *specialPurposePower;
    char const *format = "special purpose power: %s on a hit unless save vs magic";
    dieRoll = roll(dice, "1d100");
    if (dieRoll <= 10) {
      specialPurposePower = "blindness for 2-12 rounds";
    } else if (dieRoll <= 20) {
      specialPurposePower = "confusion for 2-12 rounds";
    } else if (dieRoll <= 25) {
      specialPurposePower = "disintegrate";
    } else if (dieRoll <= 55) {
      specialPurposePower = "fear for 1-4 rounds";
    } else if (dieRoll <= 65) {
      specialPurposePower = "insanity for 1-4 rounds";
    } else if (dieRoll <= 80) {
      specialPurposePower = "paralysis for 1-4 rounds";
    } else {
      specialPurposePower = "+2 on all saving throws, -1 on each die of damage sustained";
      format = "special purpose power: %s";
    }
    em_asprintf(&magicItem->trueDetails[detailCount],
                format, specialPurposePower);
    ++detailCount;
    assert(detailCount < detailCapacity);
  }
  
  // languages
  if (intelligence >= 14) {
    int dieRollMax = 100;
    int rolls = 1;
    int languageCount = 0;
    int minLanguageCount = 0;
    do {
      dieRoll = rollDice(dice, 1, dieRollMax);
      --rolls;
      if (dieRoll <= 40) {
        languageCount += 1;
      } else if (dieRoll <= 70) {
        languageCount += 2;
      } else if (dieRoll <= 85) {
        languageCount += 3;
      } else if (dieRoll <= 95) {
        languageCount += 4;
      } else if (dieRoll <= 99) {
        languageCount += 5;
      } else {
        languageCount = 0;
        minLanguageCount = 6;
        dieRollMax = 99;
        rolls = 2;
      }
    } while (rolls > 0);
    if (languageCount < minLanguageCount) {
      languageCount = minLanguageCount;
    }
    ego += (languageCount + 1 /* alignment language */ + 1) / 2;
    
    char const *languages[10];
    char const *prefix = "speaks ";
    size_t languagesSize = strlen(prefix);
    for (int i = 0; i < languageCount; ++i) {
      languages[i] = determineLanguage(dice, languages, i);
      languagesSize += strlen(languages[i]);
    }
    char const *separator = ", ";
    languagesSize += (strlen(separator) * languageCount);
    languagesSize += strlen(alignment);
    languagesSize += sizeof('\0');
    char *languagesDetail = em_calloc(sizeof(char), languagesSize);
    strcat(languagesDetail, prefix);
    for (int i = 0; i < languageCount; ++i) {
      if (i) {
        strcat(languagesDetail, separator);
      }
      strcat(languagesDetail, languages[i]);
    }
    strcat(languagesDetail, separator);
    strcat(languagesDetail, alignment);
    magicItem->trueDetails[detailCount] = languagesDetail;
    ++detailCount;
    assert(detailCount < detailCapacity);
  }
  
  em_asprintf(&magicItem->trueDetails[detailCount],
              "personality strength %i (ego %i)",
              (intelligence + ego), ego);
  ++detailCount;
  assert(detailCount < detailCapacity);
}


static void generateTeethOfDahlverNar(struct MagicItem *magicItem, 
                                      struct Dice *dice)
{
  int toothNumber = roll(dice, "1d32");
  
  magicItem->experiencePoints = 0;
  magicItem->trueValue_cp = 5000 * CP_PER_GP;
  em_asprintf(&magicItem->trueDescription,
              "tooth of Dahlver-Nar (#%i)", toothNumber);
}


void initializeMagicItem(struct MagicItem *magicItem)
{
  memset(magicItem, 0, sizeof(struct MagicItem));
}

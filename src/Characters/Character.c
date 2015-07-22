#include "Character.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "alloc_or_die.h"
#include "dice.h"


static size_t const characteristicCount = 6;
static size_t const characteristicSize = sizeof(int);
static size_t const method2CharacteristicCount = characteristicCount * 2;
static int const method3RollCount = 6;
static size_t const method4CharacterCount = 12;
static size_t const method4CharacteristicCount = characteristicCount 
                                               * method4CharacterCount;


static int compareCharacteristics(void const *characteristic1, 
                                  void const *characteristic2);
static int compareCharacteristicSets(void const *characteristicSet1, 
                                     void const *characteristicSet2);
static bool contains(char const *set[], size_t setCount, char const *s);


char const *characteristicGenerationMethodDescription(enum CharacteristicGenerationMethod method)
{
  switch (method) {
    case SimpleCharacteristicGenerationMethod:
      return "roll 3d6 once per characteristic";
    case CharacteristicGenerationMethod1:
      return "roll 4d6 six times and drop the lowest die from each roll; player assigns rolls to characteristics";
    case CharacteristicGenerationMethod2:
      return "roll 3d6 twelve times and drop the lowest six rolls; player assigns rolls to characteristics";
    case CharacteristicGenerationMethod3:
      return "roll 3d6 six times per characteristic and keep the highest roll";
    case CharacteristicGenerationMethod4:
      return "generate 12 characters using the simple method; player chooses one of the twelve";
    case GeneralNPCCharacteristicGenerationMethod:
      return "roll 3d6 once per characteristic; change any die of 1 to 3 or 6 to 4";
    case SpecialNPCCharacteristicGenerationMethod:
      return "roll 3d6 once per normal characteristic; for profession characteristic(s), add 1 to each die less than 6";
    default: return "Unknown character generation method";
  }
}


static int compareCharacteristics(void const *characteristic1, 
                                  void const *characteristic2)
{
  return *((int const *) characteristic2) - *((int const *) characteristic1);
}


static int compareCharacteristicSets(void const *characteristicSet1, 
                                     void const *characteristicSet2)
{
  int const *set1 = (int const *) characteristicSet1;
  int const *set2 = (int const *) characteristicSet2;
  
  int total1 = 0;
  int total2 = 0;
  for (int i = 0; i < characteristicCount; ++i) {
    total1 += set1[i];
    total2 += set2[i];
  }
  return total2 - total1;
}


static bool contains(char const *set[], size_t setCount, char const *s)
{
  if ( ! set) {
    return false;
  }
  
  for (int i = 0; i < setCount; ++i) {
    if (0 == strcasecmp(set[i], s)) {
      return true;
    }
  }
  return false;
}


char const *determineLanguage(struct rnd *rnd,
                              char const *exclude[], 
                              size_t excludeCount)
{
  static struct {
    int percent;
    char const *language;
  } const languageTable[] = {
    {  1 -  0, "Brownie" },
    {  3 -  1, "Bugbear" },
    {  4 -  3, "Centaur" },
    {  5 -  4, "Black Dragon" },
    {  6 -  5, "Blue Dragon" },
    {  7 -  6, "Brass Dragon" },
    {  8 -  7, "Bronze Dragon" },
    {  9 -  8, "Copper Dragon" },
    { 10 -  9, "Gold Dragon" },
    { 11 - 10, "Green Dragon" },
    { 12 - 11, "Red Dragon" },
    { 13 - 12, "Silver Dragon" },
    { 14 - 13, "White Dragon" },
    { 15 - 14, "Dryad" },
    { 20 - 15, "Dwarvish" },
    { 25 - 20, "Elvish" },
    { 26 - 25, "Ettin" },
    { 27 - 26, "Gargoyle" },
    { 28 - 27, "Cloud Giant" },
    { 29 - 28, "Fire Giant" },
    { 30 - 29, "Frost Giant" },
    { 33 - 30, "Hill Giant" },
    { 34 - 33, "Stone Giant" },
    { 35 - 34, "Storm Giant" },
    { 39 - 35, "Goblin" },
    { 40 - 39, "Gnoll" },
    { 44 - 40, "Gnome" },
    { 49 - 44, "Halfling" },
    { 51 - 49, "Hobgoblin" },
    { 54 - 51, "Kobold" },
    { 55 - 54, "Lammasu" },
    { 58 - 55, "Lizard Man" },
    { 59 - 58, "Manticore" },
    { 60 - 59, "Medusian" },
    { 61 - 60, "Minotaur" },
    { 62 - 61, "Guardian Naga" },
    { 63 - 62, "Spirit Naga" },
    { 64 - 63, "Water Naga" },
    { 65 - 64, "Nixie" },
    { 66 - 65, "Nymph" },
    { 70 - 66, "Ogrish" },
    { 71 - 70, "Ogre Magian" },
    { 76 - 71, "Orcish" },
    { 77 - 76, "Pixie" },
    { 78 - 77, "Salamander" },
    { 79 - 78, "Satyr" },
    { 80 - 79, "Shedu" },
    { 81 - 80, "Sprite" },
    { 82 - 81, "Sylph" },
    { 83 - 82, "Titan" },
    { 84 - 83, "Troll" },
    { 85 - 84, "Xorn" }
  };
  size_t const languageTableCount = sizeof languageTable 
                                  / sizeof languageTable[0];
  
  int total = 0;
  for (int i = 0; i < languageTableCount; ++i) {
    if ( ! contains(exclude, excludeCount, languageTable[i].language)) {
      total += languageTable[i].percent;
    }
  }
  
  int score = dice_roll(dice_make(1, total), rnd, NULL);
  int range = 0;
  for (int i = 0; i < languageTableCount; ++i) {
    if ( ! contains(exclude, excludeCount, languageTable[i].language)) {
      range += languageTable[i].percent;
      if (score <= range) {
        return languageTable[i].language;
      }
    }
  }
  assert(false);
  return NULL;
}


char const *determineSpell(struct rnd *rnd,
                           enum SpellType spellType, 
                           int spellLevel)
{
  assert(spellLevel >= 1);
  assert(spellLevel <= (spellType == MagicUserSpellType ? 9 : 7));
  
  struct {
    enum SpellType type;
    int level;
    char const *name;
  } const spellsTable[] = {
    { ClericalSpellType, 1, "bless" },
    { ClericalSpellType, 1, "command" },
    { ClericalSpellType, 1, "create water" },
    { ClericalSpellType, 1, "cure light wounds" },
    { ClericalSpellType, 1, "detect evil" },
    { ClericalSpellType, 1, "detect magic" },
    { ClericalSpellType, 1, "light" },
    { ClericalSpellType, 1, "protection from evil" },
    { ClericalSpellType, 1, "purify food & drink" },
    { ClericalSpellType, 1, "remove fear" },
    { ClericalSpellType, 1, "resist cold" },
    { ClericalSpellType, 1, "sanctuary" },
    
    { ClericalSpellType, 2, "augury" },
    { ClericalSpellType, 2, "chant" },
    { ClericalSpellType, 2, "detect charm" },
    { ClericalSpellType, 2, "find traps" },
    { ClericalSpellType, 2, "hold person" },
    { ClericalSpellType, 2, "know alignment" },
    { ClericalSpellType, 2, "resist fire" },
    { ClericalSpellType, 2, "silence 15' radius" },
    { ClericalSpellType, 2, "slow poison" },
    { ClericalSpellType, 2, "snake charm" },
    { ClericalSpellType, 2, "speak with animals" },
    { ClericalSpellType, 2, "spiritual hammer" },
    
    { ClericalSpellType, 3, "animate dead" },
    { ClericalSpellType, 3, "continual light" },
    { ClericalSpellType, 3, "create food & water" },
    { ClericalSpellType, 3, "cure blindness" },
    { ClericalSpellType, 3, "cure disease" },
    { ClericalSpellType, 3, "dispel magic" },
    { ClericalSpellType, 3, "feign death" },
    { ClericalSpellType, 3, "glyph of warding" },
    { ClericalSpellType, 3, "locate object" },
    { ClericalSpellType, 3, "prayer" },
    { ClericalSpellType, 3, "remove curse" },
    { ClericalSpellType, 3, "speak with dead" },

    { ClericalSpellType, 4, "cure serious wounds" },
    { ClericalSpellType, 4, "detect lie" },
    { ClericalSpellType, 4, "divination" },
    { ClericalSpellType, 4, "exorcise" },
    { ClericalSpellType, 4, "lower water" },
    { ClericalSpellType, 4, "neutralize poison" },
    { ClericalSpellType, 4, "protection from evil 10' radius" },
    { ClericalSpellType, 4, "speak with plants" },
    { ClericalSpellType, 4, "sticks to snakes" },
    { ClericalSpellType, 4, "tongues" },

    { ClericalSpellType, 5, "atonement" },
    { ClericalSpellType, 5, "commune" },
    { ClericalSpellType, 5, "cure critical wounds" },
    { ClericalSpellType, 5, "dispel evil" },
    { ClericalSpellType, 5, "flame strike" },
    { ClericalSpellType, 5, "insect plague" },
    { ClericalSpellType, 5, "plane shift" },
    { ClericalSpellType, 5, "quest" },
    { ClericalSpellType, 5, "raise dead" },
    { ClericalSpellType, 5, "true seeing" },
    
    { ClericalSpellType, 6, "aerial servant" },
    { ClericalSpellType, 6, "animate object" },
    { ClericalSpellType, 6, "blade barrier" },
    { ClericalSpellType, 6, "conjure animals" },
    { ClericalSpellType, 6, "find the path" },
    { ClericalSpellType, 6, "heal" },
    { ClericalSpellType, 6, "part water" },
    { ClericalSpellType, 6, "speak with monsters" },
    { ClericalSpellType, 6, "stone tell" },
    { ClericalSpellType, 6, "word of recall" },

    { ClericalSpellType, 7, "astral spell" },
    { ClericalSpellType, 7, "control weather" },
    { ClericalSpellType, 7, "earthquake" },
    { ClericalSpellType, 7, "gate" },
    { ClericalSpellType, 7, "holy (unholy) word" },
    { ClericalSpellType, 7, "regenerate" },
    { ClericalSpellType, 7, "restoration" },
    { ClericalSpellType, 7, "resurrection" },
    { ClericalSpellType, 7, "symbol" },
    { ClericalSpellType, 7, "wind walk" },
    
    { DrudicalSpellType, 1, "animal friendship" },
    { DrudicalSpellType, 1, "detect magic" },
    { DrudicalSpellType, 1, "detect snares & pits" },
    { DrudicalSpellType, 1, "entangle" },
    { DrudicalSpellType, 1, "faerie fire" },
    { DrudicalSpellType, 1, "invisibility to animals" },
    { DrudicalSpellType, 1, "locate animals" },
    { DrudicalSpellType, 1, "pass without trace" },
    { DrudicalSpellType, 1, "predict weather" },
    { DrudicalSpellType, 1, "purify water" },
    { DrudicalSpellType, 1, "shillelagh" },
    { DrudicalSpellType, 1, "speak with animals" },

    { DrudicalSpellType, 2, "barkskin" },
    { DrudicalSpellType, 2, "charm person or mammal" },
    { DrudicalSpellType, 2, "create water" },
    { DrudicalSpellType, 2, "cure light wounds" },
    { DrudicalSpellType, 2, "feign death" },
    { DrudicalSpellType, 2, "fire trap" },
    { DrudicalSpellType, 2, "heat metal" },
    { DrudicalSpellType, 2, "locate plants" },
    { DrudicalSpellType, 2, "obscurement" },
    { DrudicalSpellType, 2, "produce flame" },
    { DrudicalSpellType, 2, "trip" },
    { DrudicalSpellType, 2, "warp wood" },
    
    { DrudicalSpellType, 3, "call lightning" },
    { DrudicalSpellType, 3, "cure disease" },
    { DrudicalSpellType, 3, "hold animal" },
    { DrudicalSpellType, 3, "neutralize poison" },
    { DrudicalSpellType, 3, "plant growth" },
    { DrudicalSpellType, 3, "protection from fire" },
    { DrudicalSpellType, 3, "pyrotechnics" },
    { DrudicalSpellType, 3, "snare" },
    { DrudicalSpellType, 3, "stone shape" },
    { DrudicalSpellType, 3, "summon insects" },
    { DrudicalSpellType, 3, "tree" },
    { DrudicalSpellType, 3, "water breathing" },

    { DrudicalSpellType, 4, "animal summoning I" },
    { DrudicalSpellType, 4, "call woodland beings" },
    { DrudicalSpellType, 4, "control temperature 10' radius" },
    { DrudicalSpellType, 4, "cure serious wounds" },
    { DrudicalSpellType, 4, "dispel magic" },
    { DrudicalSpellType, 4, "hallucinatory forest" },
    { DrudicalSpellType, 4, "hold plant" },
    { DrudicalSpellType, 4, "plant door" },
    { DrudicalSpellType, 4, "produce fire" },
    { DrudicalSpellType, 4, "protection from lightning" },
    { DrudicalSpellType, 4, "repel insects" },
    { DrudicalSpellType, 4, "speak with plants" },

    { DrudicalSpellType, 5, "animal growth" },
    { DrudicalSpellType, 5, "animal summoning II" },
    { DrudicalSpellType, 5, "anti-plant shell" },
    { DrudicalSpellType, 5, "commune with nature" },
    { DrudicalSpellType, 5, "control winds" },
    { DrudicalSpellType, 5, "insect plague" },
    { DrudicalSpellType, 5, "pass plant" },
    { DrudicalSpellType, 5, "sticks to snakes" },
    { DrudicalSpellType, 5, "transmute rock to mud" },
    { DrudicalSpellType, 5, "wall of fire" },

    { DrudicalSpellType, 6, "animal summoning III" },
    { DrudicalSpellType, 6, "anti-animal shell" },
    { DrudicalSpellType, 6, "conjure fire elemental" },
    { DrudicalSpellType, 6, "cure critical wounds" },
    { DrudicalSpellType, 6, "feeblemind" },
    { DrudicalSpellType, 6, "fire seeds" },
    { DrudicalSpellType, 6, "transport via plants" },
    { DrudicalSpellType, 6, "turn wood" },
    { DrudicalSpellType, 6, "wall of thorns" },
    { DrudicalSpellType, 6, "weather summoning" },

    { DrudicalSpellType, 7, "animate rock" },
    { DrudicalSpellType, 7, "chariot of Sustarre" },
    { DrudicalSpellType, 7, "confusion" },
    { DrudicalSpellType, 7, "conjure earth elemental" },
    { DrudicalSpellType, 7, "control weather" },
    { DrudicalSpellType, 7, "creeping doom" },
    { DrudicalSpellType, 7, "finger of death" },
    { DrudicalSpellType, 7, "fire storm" },
    { DrudicalSpellType, 7, "reincarnate" },
    { DrudicalSpellType, 7, "transmute metal to wood" },
    
    { MagicUserSpellType, 1, "affect normal fires" },
    { MagicUserSpellType, 1, "burning hands" },
    { MagicUserSpellType, 1, "charm person" },
    { MagicUserSpellType, 1, "comprehend languages" },
    { MagicUserSpellType, 1, "dancing lights" },
    { MagicUserSpellType, 1, "detect magic" },
    { MagicUserSpellType, 1, "enlarge" },
    { MagicUserSpellType, 1, "erase" },
    { MagicUserSpellType, 1, "feather fall" },
    { MagicUserSpellType, 1, "find familiar" },
    { MagicUserSpellType, 1, "friends" },
    { MagicUserSpellType, 1, "hold portal" },
    { MagicUserSpellType, 1, "identify" },
    { MagicUserSpellType, 1, "jump" },
    { MagicUserSpellType, 1, "light" },
    { MagicUserSpellType, 1, "magic missile" },
    { MagicUserSpellType, 1, "mending" },
    { MagicUserSpellType, 1, "message" },
    { MagicUserSpellType, 1, "Nystul's magic aura" },
    { MagicUserSpellType, 1, "protection from evil" },
    { MagicUserSpellType, 1, "push" },
    { MagicUserSpellType, 1, "read magic" },
    { MagicUserSpellType, 1, "shield" },
    { MagicUserSpellType, 1, "shocking grasp" },
    { MagicUserSpellType, 1, "sleep" },
    { MagicUserSpellType, 1, "spider climb" },
    { MagicUserSpellType, 1, "Tenser's floating disc" },
    { MagicUserSpellType, 1, "unseen servant" },
    { MagicUserSpellType, 1, "ventriloquism" },
    { MagicUserSpellType, 1, "write" },

    { MagicUserSpellType, 2, "audible glamer" },
    { MagicUserSpellType, 2, "continual light" },
    { MagicUserSpellType, 2, "darkness 15' radius" },
    { MagicUserSpellType, 2, "detect evil" },
    { MagicUserSpellType, 2, "detect invisibility" },
    { MagicUserSpellType, 2, "ESP" },
    { MagicUserSpellType, 2, "fools gold" },
    { MagicUserSpellType, 2, "forget" },
    { MagicUserSpellType, 2, "invisibility" },
    { MagicUserSpellType, 2, "knock" },
    { MagicUserSpellType, 2, "Leomund's trap" },
    { MagicUserSpellType, 2, "levitate" },
    { MagicUserSpellType, 2, "locate object" },
    { MagicUserSpellType, 2, "magic mouth" },
    { MagicUserSpellType, 2, "mirror image" },
    { MagicUserSpellType, 2, "pyrotechnics" },
    { MagicUserSpellType, 2, "ray of enfeeblement" },
    { MagicUserSpellType, 2, "rope trick" },
    { MagicUserSpellType, 2, "scare" },
    { MagicUserSpellType, 2, "shatter" },
    { MagicUserSpellType, 2, "stinking cloud" },
    { MagicUserSpellType, 2, "strength" },
    { MagicUserSpellType, 2, "web" },
    { MagicUserSpellType, 2, "wizard lock" },

    { MagicUserSpellType, 3, "blink" },
    { MagicUserSpellType, 3, "clairaudience" },
    { MagicUserSpellType, 3, "clairvoyance" },
    { MagicUserSpellType, 3, "dispel magic" },
    { MagicUserSpellType, 3, "explosive runes" },
    { MagicUserSpellType, 3, "feign death" },
    { MagicUserSpellType, 3, "fireball" },
    { MagicUserSpellType, 3, "flame arrow" },
    { MagicUserSpellType, 3, "fly" },
    { MagicUserSpellType, 3, "gust of wind" },
    { MagicUserSpellType, 3, "haste" },
    { MagicUserSpellType, 3, "hold person" },
    { MagicUserSpellType, 3, "infravision" },
    { MagicUserSpellType, 3, "invisibility 10' radius" },
    { MagicUserSpellType, 3, "Leomund's tiny hut" },
    { MagicUserSpellType, 3, "lightning bolt" },
    { MagicUserSpellType, 3, "monster summoning I" },
    { MagicUserSpellType, 3, "phantasmal force" },
    { MagicUserSpellType, 3, "protection from evil 10' radius" },
    { MagicUserSpellType, 3, "protection from normal missiles" },
    { MagicUserSpellType, 3, "slow" },
    { MagicUserSpellType, 3, "suggestion" },
    { MagicUserSpellType, 3, "tongues" },
    { MagicUserSpellType, 3, "water breathing" },

    { MagicUserSpellType, 4, "charm monster" },
    { MagicUserSpellType, 4, "confusion" },
    { MagicUserSpellType, 4, "dig" },
    { MagicUserSpellType, 4, "dimension door" },
    { MagicUserSpellType, 4, "enchanted weapon" },
    { MagicUserSpellType, 4, "extension I" },
    { MagicUserSpellType, 4, "fear" },
    { MagicUserSpellType, 4, "fire charm" },
    { MagicUserSpellType, 4, "fire shield" },
    { MagicUserSpellType, 4, "fire trap" },
    { MagicUserSpellType, 4, "fumble" },
    { MagicUserSpellType, 4, "hallucinatory terrain" },
    { MagicUserSpellType, 4, "ice storm" },
    { MagicUserSpellType, 4, "massmorph" },
    { MagicUserSpellType, 4, "minor globe of invulnerability" },
    { MagicUserSpellType, 4, "monster summoning II" },
    { MagicUserSpellType, 4, "plant growth" },
    { MagicUserSpellType, 4, "polymorph other" },
    { MagicUserSpellType, 4, "polymorph self" },
    { MagicUserSpellType, 4, "Rary's mnemonic enhancer" },
    { MagicUserSpellType, 4, "remove curse" },
    { MagicUserSpellType, 4, "wall of fire" },
    { MagicUserSpellType, 4, "wall if ice" },
    { MagicUserSpellType, 4, "wizard eye" },

    { MagicUserSpellType, 5, "airy water" },
    { MagicUserSpellType, 5, "animal growth" },
    { MagicUserSpellType, 5, "animate dead" },
    { MagicUserSpellType, 5, "Bigby's interposing hand" },
    { MagicUserSpellType, 5, "cloudkill" },
    { MagicUserSpellType, 5, "conjure elemental" },
    { MagicUserSpellType, 5, "cone of cold" },
    { MagicUserSpellType, 5, "contact other plane" },
    { MagicUserSpellType, 5, "distance distortion" },
    { MagicUserSpellType, 5, "extension II" },
    { MagicUserSpellType, 5, "feeblemind" },
    { MagicUserSpellType, 5, "hold monster" },
    { MagicUserSpellType, 5, "Leomund's secret chest" },
    { MagicUserSpellType, 5, "magic jar" },
    { MagicUserSpellType, 5, "monster summoning III" },
    { MagicUserSpellType, 5, "Mordenkainen's faithful hound" },
    { MagicUserSpellType, 5, "passwall" },
    { MagicUserSpellType, 5, "stone shape" },
    { MagicUserSpellType, 5, "telekinesis" },
    { MagicUserSpellType, 5, "teleport" },
    { MagicUserSpellType, 5, "transmute rock to mud" },
    { MagicUserSpellType, 5, "wall of force" },
    { MagicUserSpellType, 5, "wall of iron" },
    { MagicUserSpellType, 5, "wall of stone" },

    { MagicUserSpellType, 6, "anti-magic shell" },
    { MagicUserSpellType, 6, "Bigby's forceful hand" },
    { MagicUserSpellType, 6, "control weather" },
    { MagicUserSpellType, 6, "death spell" },
    { MagicUserSpellType, 6, "disintegrate" },
    { MagicUserSpellType, 6, "enchant an item" },
    { MagicUserSpellType, 6, "extension III" },
    { MagicUserSpellType, 6, "geas" },
    { MagicUserSpellType, 6, "glassee" },
    { MagicUserSpellType, 6, "globe of invulnerability" },
    { MagicUserSpellType, 6, "guards and wards" },
    { MagicUserSpellType, 6, "invisible stalker" },
    { MagicUserSpellType, 6, "legend lore" },
    { MagicUserSpellType, 6, "lower water" },
    { MagicUserSpellType, 6, "monster summoning IV" },
    { MagicUserSpellType, 6, "move earth" },
    { MagicUserSpellType, 6, "Otiluke's freezing sphere" },
    { MagicUserSpellType, 6, "part water" },
    { MagicUserSpellType, 6, "project image" },
    { MagicUserSpellType, 6, "reincarnation" },
    { MagicUserSpellType, 6, "repulsion" },
    { MagicUserSpellType, 6, "spiritwrack" },
    { MagicUserSpellType, 6, "stone to flesh" },
    { MagicUserSpellType, 6, "Tenser's transformation" },

    { MagicUserSpellType, 7, "Bigby's grasping hand" },
    { MagicUserSpellType, 7, "cacodemon" },
    { MagicUserSpellType, 7, "charm plants" },
    { MagicUserSpellType, 7, "delayed blast fireball" },
    { MagicUserSpellType, 7, "Drawmij's instant summons" },
    { MagicUserSpellType, 7, "duo-dimension" },
    { MagicUserSpellType, 7, "limited wish" },
    { MagicUserSpellType, 7, "mass invisibility" },
    { MagicUserSpellType, 7, "monster summoning V" },
    { MagicUserSpellType, 7, "Mordenkainen's sword" },
    { MagicUserSpellType, 7, "phase door" },
    { MagicUserSpellType, 7, "power word, stun" },
    { MagicUserSpellType, 7, "reverse gravity" },
    { MagicUserSpellType, 7, "simulacrum" },
    { MagicUserSpellType, 7, "statue" },
    { MagicUserSpellType, 7, "vanish" },

    { MagicUserSpellType, 8, "antipathy/sympathy" },
    { MagicUserSpellType, 8, "Bigby's clenched fist" },
    { MagicUserSpellType, 8, "clone" },
    { MagicUserSpellType, 8, "glassteel" },
    { MagicUserSpellType, 8, "incendiary cloud" },
    { MagicUserSpellType, 8, "mass charm" },
    { MagicUserSpellType, 8, "maze" },
    { MagicUserSpellType, 8, "mind blank" },
    { MagicUserSpellType, 8, "monster summoning VI" },
    { MagicUserSpellType, 8, "Otto's irresistible dance" },
    { MagicUserSpellType, 8, "permanency" },
    { MagicUserSpellType, 8, "polymorph any object" },
    { MagicUserSpellType, 8, "power word, blind" },
    { MagicUserSpellType, 8, "Serten's spell immunity" },
    { MagicUserSpellType, 8, "symbol" },
    { MagicUserSpellType, 8, "trap the soul" },

    { MagicUserSpellType, 9, "astral spell" },
    { MagicUserSpellType, 9, "Bigby's crushing hand" },
    { MagicUserSpellType, 9, "gate" },
    { MagicUserSpellType, 9, "imprisonment" },
    { MagicUserSpellType, 9, "meteor swarm" },
    { MagicUserSpellType, 9, "monster summoning VII" },
    { MagicUserSpellType, 9, "power word, kill" },
    { MagicUserSpellType, 9, "prismatic sphere" },
    { MagicUserSpellType, 9, "shape change" },
    { MagicUserSpellType, 9, "temporal stasis" },
    { MagicUserSpellType, 9, "time stop" },
    { MagicUserSpellType, 9, "wish" },

    { IllusionistSpellType, 1, "audible glamer" },
    { IllusionistSpellType, 1, "change self" },
    { IllusionistSpellType, 1, "color spray" },
    { IllusionistSpellType, 1, "dancing lights" },
    { IllusionistSpellType, 1, "darkness" },
    { IllusionistSpellType, 1, "detect illusion" },
    { IllusionistSpellType, 1, "detect invisibility" },
    { IllusionistSpellType, 1, "gaze reflection" },
    { IllusionistSpellType, 1, "hypnotism" },
    { IllusionistSpellType, 1, "light" },
    { IllusionistSpellType, 1, "phantasmal force" },
    { IllusionistSpellType, 1, "wall of fog" },

    { IllusionistSpellType, 2, "blindness" },
    { IllusionistSpellType, 2, "blur" },
    { IllusionistSpellType, 2, "deafness" },
    { IllusionistSpellType, 2, "detect magic" },
    { IllusionistSpellType, 2, "fog cloud" },
    { IllusionistSpellType, 2, "hypnotic pattern" },
    { IllusionistSpellType, 2, "improved phantasmal force" },
    { IllusionistSpellType, 2, "invisibility" },
    { IllusionistSpellType, 2, "magic mouth" },
    { IllusionistSpellType, 2, "mirror image" },
    { IllusionistSpellType, 2, "misdirection" },
    { IllusionistSpellType, 2, "ventriloquism" },

    { IllusionistSpellType, 3, "continual darkness" },
    { IllusionistSpellType, 3, "continual light" },
    { IllusionistSpellType, 3, "dispel illusion" },
    { IllusionistSpellType, 3, "fear" },
    { IllusionistSpellType, 3, "hallucinatory terrain" },
    { IllusionistSpellType, 3, "illusionary script" },
    { IllusionistSpellType, 3, "invisibility 10' radius" },
    { IllusionistSpellType, 3, "non-detection" },
    { IllusionistSpellType, 3, "paralyzation" },
    { IllusionistSpellType, 3, "rope trick" },
    { IllusionistSpellType, 3, "spectral force" },
    { IllusionistSpellType, 3, "suggestion" },

    { IllusionistSpellType, 4, "confusion" },
    { IllusionistSpellType, 4, "dispel exhaustion" },
    { IllusionistSpellType, 4, "emotion" },
    { IllusionistSpellType, 4, "improved invisibility" },
    { IllusionistSpellType, 4, "massmorph" },
    { IllusionistSpellType, 4, "minor creation" },
    { IllusionistSpellType, 4, "phantasmal killer" },
    { IllusionistSpellType, 4, "shadow monsters" },

    { IllusionistSpellType, 5, "chaos" },
    { IllusionistSpellType, 5, "demi-shadow monsters" },
    { IllusionistSpellType, 5, "major creation" },
    { IllusionistSpellType, 5, "maze" },
    { IllusionistSpellType, 5, "projected image" },
    { IllusionistSpellType, 5, "shadow door" },
    { IllusionistSpellType, 5, "shadow magic" },
    { IllusionistSpellType, 5, "summon shadow" },

    { IllusionistSpellType, 6, "conjure animals" },
    { IllusionistSpellType, 6, "demi-shadow magic" },
    { IllusionistSpellType, 6, "mass suggestion" },
    { IllusionistSpellType, 6, "permanent illusion" },
    { IllusionistSpellType, 6, "programmed illusion" },
    { IllusionistSpellType, 6, "shades" },
    { IllusionistSpellType, 6, "true sight" },
    { IllusionistSpellType, 6, "veil" },

    { IllusionistSpellType, 7, "alter reality" },
    { IllusionistSpellType, 7, "astral spell" },
    { IllusionistSpellType, 7, "prismatic spray" },
    { IllusionistSpellType, 7, "prismatic wall" },
    { IllusionistSpellType, 7, "vision" }
  };
  size_t const spellsTableCount = sizeof spellsTable / sizeof spellsTable[0];
  
  int total = 0;
  for (int i = 0; i < spellsTableCount; ++i) {
    if (   spellsTable[i].type == spellType 
        && spellsTable[i].level == spellLevel)
    {
      ++total;
    }
  }
  assert(total);
  
  int score = dice_roll(dice_make(1, total), rnd, NULL);
  int range = 0;
  for (int i = 0; i < spellsTableCount; ++i) {
    if (   spellsTable[i].type == spellType 
        && spellsTable[i].level == spellLevel)
    {
      ++range;
      if (score <= range) {
        return spellsTable[i].name;
      }
    }
  }
  assert(false);
  return "(spell)";
}


int *generateCharacteristics(struct rnd *rnd,
                             enum CharacteristicGenerationMethod method,
                             uint32_t characteristicFlags)
{
  int *characteristics;
  switch (method) {
    case CharacteristicGenerationMethod1:
      characteristics = calloc_or_die(characteristicCount, characteristicSize);
      for (size_t i = 0; i < characteristicCount; ++i) {
        characteristics[i] = dice_roll_and_drop_lowest(dice_make(4, 6), rnd);
      }
      qsort(characteristics, characteristicCount, characteristicSize, 
            compareCharacteristics);
      break;
    case CharacteristicGenerationMethod2:
      characteristics = calloc_or_die(method2CharacteristicCount,
                                      characteristicSize);
      for (size_t i = 0; i < method2CharacteristicCount; ++i) {
        characteristics[i] = roll("3d6", rnd);
      }
      qsort(characteristics, characteristicCount, characteristicSize, 
            compareCharacteristics);
      break;
    case CharacteristicGenerationMethod3:
      characteristics = calloc_or_die(characteristicCount, characteristicSize);
      for (size_t i = 0; i < characteristicCount; ++i) {
        for (int j = 0; j < method3RollCount; ++j) {
          int characteristic = roll("3d6", rnd);
          if (characteristic > characteristics[i]) {
            characteristics[i] = characteristic;
          }
        }
      }
      break;
    case CharacteristicGenerationMethod4:
      characteristics = calloc_or_die(method4CharacteristicCount,
                                      characteristicSize);
      for (size_t i = 0; i < method4CharacteristicCount; ++i) {
        characteristics[i] = roll("3d6", rnd);
      }
      qsort(characteristics, 
            method4CharacterCount, 
            characteristicSize * characteristicCount, 
            compareCharacteristicSets);
      break;
    case GeneralNPCCharacteristicGenerationMethod:
      characteristics = calloc_or_die(characteristicCount, characteristicSize);
      for (size_t i = 0; i < characteristicCount; ++i) {
        characteristics[i] = dice_roll_with_average_scoring(dice_make(3, 6), rnd);
      }
      break;
    case SpecialNPCCharacteristicGenerationMethod:
      {
        characteristics = calloc_or_die(characteristicCount,
                                        characteristicSize);
        for (int i = 0; i < characteristicCount; ++i) {
          uint32_t characteristicFlag = STRENGTH << i;
          if (characteristicFlags & characteristicFlag) {
            characteristics[i] = dice_roll_and_adjust_upwards(dice_make(3, 6), rnd);
          } else {
            characteristics[i] = roll("3d6", rnd);
          }
        }
        break;
      }
    default:
      characteristics = calloc_or_die(characteristicCount, characteristicSize);
      for (size_t i = 0; i < characteristicCount; ++i) {
        characteristics[i] = roll("3d6", rnd);
      }
      break;
  }
  return characteristics;
}

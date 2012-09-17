#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED


#include <stddef.h>
#include <stdint.h>


#define STRENGTH      ((uint32_t) 0x0001u)
#define INTELLIGENCE  ((uint32_t) 0x0002u)
#define WISDOM        ((uint32_t) 0x0004u)
#define DEXTERITY     ((uint32_t) 0x0008u)
#define CONSTITUTION  ((uint32_t) 0x0010u)
#define CHARISMA      ((uint32_t) 0x0020u)
#define NO_SPECIAL_CHARACTERISTICS 0


struct Dice;


enum CharacteristicGenerationMethod {
  SimpleCharacteristicGenerationMethod,
  CharacteristicGenerationMethod1,
  CharacteristicGenerationMethod2,
  CharacteristicGenerationMethod3,
  CharacteristicGenerationMethod4,
  GeneralNPCCharacteristicGenerationMethod,
  SpecialNPCCharacteristicGenerationMethod
};

enum SpellType {
  MagicUserSpellType,
  ClericalSpellType,
  IllusionistSpellType,
  DrudicalSpellType
};


struct Character {
  int strength;
  int intelligence;
  int wisdom;
  int dexterity;
  int constitution;
  int charisma;
};


char const *characteristicGenerationMethodDescription(enum CharacteristicGenerationMethod method);

char const *determineLanguage(struct Dice *dice,
                              char const *exclude[], 
                              size_t excludeCount);

char const *determineSpell(struct Dice *dice, 
                           enum SpellType spellType, 
                           int spellLevel);

void finalizeCharacter(struct Character *character);

void initializeCharacter(struct Character *character);

int *generateCharacteristics(struct Dice *dice, 
                             enum CharacteristicGenerationMethod method,
                             uint32_t characteristicFlags);

#endif /* #ifndef CHARACTER_H_INCLUDED */

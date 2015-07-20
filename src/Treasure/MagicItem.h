#ifndef MAGICITEM_H_INCLUDED
#define MAGICITEM_H_INCLUDED


#include <stdint.h>


/* constants for ClassRestrictions */
#define NO_CLASS_RESTRICTIONS ((uint32_t) 0x0000u)
#define CLERICS               ((uint32_t) 0x0001u)
#define FIGHTERS              ((uint32_t) 0x0002u)
#define MAGIC_USERS           ((uint32_t) 0x0004u)
#define THIEVES               ((uint32_t) 0x0008u)

/* constants for PossibleMagicItems */
#define NO_MAGIC_ITEM   ((uint32_t) 0x0000u)
#define POTION          ((uint32_t) 0x0001u)
#define SCROLL          ((uint32_t) 0x0002u)
#define RING            ((uint32_t) 0x0004u)
#define ROD_STAFF_WAND  ((uint32_t) 0x0008u)
#define MISC_MAGIC      ((uint32_t) 0x0010u)
#define ARMOR_SHIELD    ((uint32_t) 0x0020u)
#define SWORD           ((uint32_t) 0x0040u)
#define MISC_WEAPON     ((uint32_t) 0x0080u)

#define ANY_MAGIC_ITEM (POTION | SCROLL | RING | ROD_STAFF_WAND | MISC_MAGIC \
                       | ARMOR_SHIELD | SWORD | MISC_WEAPON)

#define MAGIC_WEAPON_OR_ARMOR (SWORD | MISC_WEAPON)

#define NON_WEAPON_MAGIC (POTION | SCROLL | RING | ROD_STAFF_WAND | MISC_MAGIC \
                         | ARMOR_SHIELD)


typedef uint32_t ClassRestrictions;
typedef uint32_t PossibleMagicItems;


struct rnd;


enum MagicItemType {
  UnknownMagicItemType = 0,
  PotionMagicItemType,
  ScrollMagicItemType,
  RingMagicItemType,
  RodStaffOrWandMagicItemType,
  MiscMagicItemType,
  ArmorOrShieldMagicItemType,
  SwordMagicItemType,
  MiscWeaponMagicItemType
};


struct MagicItem {
  int experiencePoints;
  char *trueDescription;
  char **trueDetails;
  int trueValue_cp;
  enum MagicItemType type;
};


void finalizeMagicItem(struct MagicItem *magicItem);

void generateMagicItem(struct MagicItem *magicItem, 
                       struct rnd *rnd,
                       PossibleMagicItems possibleMagicItems);

void initializeMagicItem(struct MagicItem *magicItem);


#endif /* #ifndef MAGICITEM_H_INCLUDED */

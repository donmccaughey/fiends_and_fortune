#ifndef FNF_TREASURE_MAGIC_ITEM_H_INCLUDED
#define FNF_TREASURE_MAGIC_ITEM_H_INCLUDED


#include <stdint.h>


/* constants for class_restrictions_t */
#define NO_CLASS_RESTRICTIONS ((uint32_t) 0x0000u)
#define CLERICS               ((uint32_t) 0x0001u)
#define FIGHTERS              ((uint32_t) 0x0002u)
#define MAGIC_USERS           ((uint32_t) 0x0004u)
#define THIEVES               ((uint32_t) 0x0008u)

/* constants for possible_magic_items_t */
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


typedef uint32_t class_restrictions_t;
typedef uint32_t possible_magic_items_t;


struct cJSON;
struct rnd;


enum magic_item_type {
    magic_item_type_unknown = 0,
    magic_item_type_potion,
    magic_item_type_scroll,
    magic_item_type_ring,
    magic_item_type_rod_staff_or_wand,
    magic_item_type_misc,
    magic_item_type_armor,
    magic_item_type_sword,
    magic_item_type_misc_weapon
};


struct magic_item {
    int experience_points;
    char *true_description;
    char **true_details;
    int true_value_in_cp;
    enum magic_item_type type;
};


struct cJSON *
magic_item_create_json_object(struct magic_item *magic_item);

void
magic_item_finalize(struct magic_item *magic_item);

void
magic_item_generate(struct magic_item *magic_item,
                    struct rnd *rnd,
                    possible_magic_items_t possible_magic_items);

void
magic_item_initialize(struct magic_item *magic_item);

void
magic_item_initialize_from_json_object(struct magic_item *magic_item,
                                       struct cJSON *json_object);


#endif

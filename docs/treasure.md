Coins
=====
conversions
-----------
    10 copper pieces = 1 silver piece
    20 silver pieces = 1 gold piece
    2 electrum pieces = 1 gold piece
    1 gold piece = 1 gold piece
    1 platinum piece = 5 gold pieces

conversions to gold pieces
--------------------------
    1 copper piece = 0.005 gold pieces
    1 silver piece = 0.05 gold pieces
    1 electrum piece = 0.5 gold pieces
    1 gold piece = 1 gold piece
    1 platinum piece = 5 gold pieces

conversions to copper pieces
----------------------------
    1 copper piece = 1 copper piece
    1 silver piece = 10 copper pieces
    1 electrum piece = 100 copper pieces
    1 gold piece = 200 copper pieces
    1 platinum piece = 1000 copper pieces


Gems
====
    value
    rank            base value
    -----           ----------
    1               1 sp = 0.05 gp = 10 cp
    2               5 sp = 0.25 gp = 50 cp
    3               10 sp = 0.5 gp = 100 cp
    4               1 gp                    
    5               5                       
    6               10
    7               50
    8               100
    9               500
    10              1000
    11              5000
    12              10000
    13              25000
    14              50000
    15              100000
    16              250000
    17              500000
    18              1000000


    value
    rank            description
    -----           -----------
    6               ornamental stone
    7               semi-precious stone
    8               fancy stone
    9               precious stone
    10              gem stone
    11              jewel stone


    value
    rank mod        size
    -----           ----
    -2              very small
    -1              small
    0               average
    +1              large
    +2              very large
    +3              huge


Maps Or Magic
=============
    A: 30% 3 any
    B: 10% 1 sword, armor or misc weapon
    C: 10% 2 any
    D: 15% 2 any, 1 potion
    E: 25% 3 any, 1 scroll
    F: 30% 3 non-weapon, 1 potion, 1 scroll
    G: 35% 4 any, 1 scroll
    H: 15% 4 any, 1 potion, 1 scroll
    I: 15% 1 any
    
    S: 40% 2-8 potions
    T: 50% 1-4 scrolls
    U: 70% 1 ring, 1 rod, 1 misc magic, 1 armor, 1 sword, 1 misc weapon
    V: 85% 2 ring, 2 rod, 2 misc magic, 2 armor, 2 sword, 2 misc weapon
    W: 55% 1 map
    X: 60% 1 misc magic, 1 potion
    
    Z: 50% 3 any magic


Combined Hoard Treasures
========================
    01-20
      generateMonetaryTreasure_1to2_CopperAndSilver()
      --
      generateMagicTreasure_1to5_ItemAndPotions()
    
    21-40
      generateMonetaryTreasure_6to10_Gold()
      --
      generateMagicTreasure_1to5_ItemAndPotions()
    41-55
      generateMonetaryTreasure_3to5_Electrum()
      generateMonetaryTreasure_6to10_Gold()
      --
      generateMagicTreasure_1to5_ItemAndPotions()
      generate_magic_treasure_15_to_18_potions_and_scrolls()
    56-65
      generateMonetaryTreasure_1to2_CopperAndSilver()
      generateMonetaryTreasure_3to5_Electrum()
      generateMonetaryTreasure_6to10_Gold()
      --
      generate_magic_treasure_9_to_12_sword_armor_and_weapon()
      generate_magic_treasure_13_to_14_three_items()
    66-75
      generate_monetary_treasure_6_to_10_gold()
      generateMonetaryTreasure_11to12_Platinum()
      --
      generate_magic_treasure_6_to_8_two_items()
      generateMagicTreasure_15to18_PotionsAndScrolls()
    76-80
      generateMonetaryTreasure_3to5_Electrum()
      generateMonetaryTreasure_6to10_Gold()
      generateMonetaryTreasure_11to12_Platinum()
      generate_monetary_treasure_16_to_17_jewelry()
      --
      generateMagicTreasure_1to5_ItemAndPotions()
      generateMagicTreasure_9to12_SwordArmorAndWeapon()
    81-85
      generateMonetaryTreasure_1to2_CopperAndSilver()
      generateMonetaryTreasure_3to5_Electrum()
      generateMonetaryTreasure_6to10_Gold()
      generateMonetaryTreasure_11to12_Platinum()
      generate_monetary_treasure_13_to_15_gems()
      generateMonetaryTreasure_16to17_Jewelry()
      --
      map to generate_magic_treasure_1_to_5_item_and_potions()
    86-90
      generateMonetaryTreasure_1to2_CopperAndSilver()
      generate_monetary_treasure_3_to_5_electrum()
      generateMonetaryTreasure_6to10_Gold()
      generateMonetaryTreasure_11to12_Platinum()
      generateMonetaryTreasure_13to15_Gems()
      generateMonetaryTreasure_16to17_Jewelry()
      --
      map to generate_magic_treasure_19_four_items()
    91-96
      map to generate_monetary_treasure_1_to_2_copper_and_silver(),
             generateMonetaryTreasure_3to5_Electrum()
      --
      generate_magic_treasure_20_five_items()
    97-00
      map to generate_monetary_treasure_11_to_12_platinum(),
             generateMonetaryTreasure_13to15_Gems()
      --
      generateMagicTreasure_15to18_PotionsAndScrolls()
      generateMagicTreasure_20_FiveItems()


Magic Item Generation
=====================

    struct Common {
        int percent;
        int experience_points;
        int sale_value_in_gp;
        class_restrictions_t class_restrictions;
        generate_function generate;
    };
    
    
    struct XMiscMagicItem {
        int percent;
        char const *name;
        int experience_points;
        int sale_value_in_gp;
        class_restrictions_t class_restrictions;
        generate_function generate;
    };
    
    struct armor_or_shield {
        int percent;
        char const *name;
        bool is_armor;
        int experience_points;
        int sale_value_in_gp;
    }
    
    struct artifact_or_relic {
        int percent;
        char const *name;
        >>> int experience_points; (always 0)
        int sale_value_in_gp;
        generate_function generate;
    }
    
    struct misc_weapon {
        int percent;
        char const *name;
        char const *quantity;
        int experience_points;
        int sale_value_in_gp;
    }
    
    struct Potion {
        int percent;
        char const *name;
        char const *experience_points;
        char const *sale_value_in_gp;
        class_restrictions_t class_restrictions;
    } 
    
    struct Ring {
        int percent;
        char const *name;
        bool limited_charges;
        >>> char const *quantity; (always 1)
        char const *experience_points;
        char const *sale_value_in_gp;
        class_restrictions_t class_restrictions;
    }
    
    struct rod_staff_or_wand {
        int percent;
        char const *name;
        >>> char const *quantity; (always 1)
        int experience_points;
        int sale_value_in_gp;
        class_restrictions_t class_restrictions;
    } 
    
    struct scroll {
        int percent;
        char const *name;
        >>> char const *quantity; (always 1)
        bool is_spell_scroll;
        int spell_count;
        char const *magic_user_spell_level_range;
        char const *clerical_spell_level_range;
        int experience_points;
        int sale_value_in_gp;
    }
    
    struct sword {
        int percent;
        char const *name_format;
        >>> char const *quantity; (always 1)
        int experience_points;
        int sale_value_in_gp;
    }
    
    
    struct (Magic Item) {
        int percent;
        possible_magic_items_t possible_magic_items_t;
        enum magic_item_type type;
        generate_function generate;
    } 

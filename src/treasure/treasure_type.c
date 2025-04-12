#include "treasure_type.h"

#include <assert.h>
#include <base/base.h>
#include <mechanics/mechanics.h>

#include "astr.h"
#include "gem.h"
#include "jewelry.h"
#include "magic_item.h"
#include "treasure.h"
#include "xstring.h"


struct amount {
    int count;
    int sides;
    int multiplier;
    bool is_constant;
};


struct coins_gems_or_jewelry {
    struct amount amount;
    int percent_chance;
    bool is_per_individual;
};


struct maps_or_magic_type {
    struct amount amount;
    bool is_map_possible;
    possible_magic_items_t possible_magic_items;
};


struct maps_or_magic {
    int percent_chance;
    struct maps_or_magic_type types[6];
    int types_count;
};


struct treasure_type {
    char letter;
    struct coins_gems_or_jewelry copper;
    struct coins_gems_or_jewelry silver;
    struct coins_gems_or_jewelry electrum;
    struct coins_gems_or_jewelry gold;
    struct coins_gems_or_jewelry platinum;
    struct coins_gems_or_jewelry gems;
    struct coins_gems_or_jewelry jewelry;
    struct maps_or_magic maps_or_magic;
};


static struct treasure_type treasure_types[] = {
    {
        .letter='A',
        .copper={
            .amount={.count=1, .sides=6, .multiplier=1000},
            .percent_chance=25,
        },
        .silver={
            .amount={.count=1, .sides=6, .multiplier=1000},
            .percent_chance=30,
        },
        .electrum={
            .amount={.count=1, .sides=6, .multiplier=1000},
            .percent_chance=35,
        },
        .gold={
            .amount={.count=1, .sides=10, .multiplier=1000},
            .percent_chance=40,
        },
        .platinum={
            .amount={.count=1, .sides=4, .multiplier=100},
            .percent_chance=25,
        },
        .gems={
            .amount={.count=4, .sides=10},
            .percent_chance=60,
        },
        .jewelry={
            .amount={.count=3, .sides=10},
            .percent_chance=50,
        },
        .maps_or_magic={
            .percent_chance=30,
            .types={
                {
                    .amount={.count=3, .is_constant=true},
                    .is_map_possible=true,
                    .possible_magic_items=ANY_MAGIC_ITEM
                }
            },
            .types_count=1
        }
    },
    {
        .letter='B',
        .copper={
            .amount={.count=1, .sides=8, .multiplier=1000},
            .percent_chance=50
        },
        .silver={
            .amount={.count=1, .sides=6, .multiplier=1000},
            .percent_chance=25
        },
        .electrum={
            .amount={.count=1, .sides=4, .multiplier=1000},
            .percent_chance=25
        },
        .gold={
            .amount={.count=1, .sides=3, .multiplier=1000},
            .percent_chance=25
        },
        .platinum={
            .percent_chance=0
        },
        .gems={
            .amount={.count=1, .sides=8},
            .percent_chance=30
        },
        .jewelry={
            .amount={.count=1, .sides=4},
            .percent_chance=20
        },
        .maps_or_magic={
            .percent_chance=10,
            .types={
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=MAGIC_WEAPON_OR_ARMOR
                }
            },
            .types_count=1
        }
    },
    {
        .letter='C',
        .copper={
            .amount={.count=1, .sides=12, .multiplier=1000},
            .percent_chance=20
        },
        .silver={
            .amount={.count=1, .sides=6, .multiplier=1000},
            .percent_chance=30
        },
        .electrum={
            .amount={.count=1, .sides=4, .multiplier=1000},
            .percent_chance=10
        },
        .gems={
            .amount={.count=1, .sides=6},
            .percent_chance=25
        },
        .jewelry={
            .amount={.count=1, .sides=3},
            .percent_chance=20
        },
        .maps_or_magic={
            .percent_chance=10,
            .types={
                {
                    .amount={.count=2, .is_constant=true},
                    .is_map_possible=true,
                    .possible_magic_items=ANY_MAGIC_ITEM
                }
            },
            .types_count=1
        }
    },
    {
        .letter='D',
        .copper={
            .amount={.count=1, .sides=8, .multiplier=1000},
            .percent_chance=10
        },
        .silver={
            .amount={.count=1, .sides=12, .multiplier=1000},
            .percent_chance=15
        },
        .electrum={
            .amount={.count=1, .sides=8, .multiplier=1000},
            .percent_chance=15
        },
        .gold={
            .amount={.count=1, .sides=6, .multiplier=1000},
            .percent_chance=50
        },
        .gems={
            .amount={.count=1, .sides=10},
            .percent_chance=30
        },
        .jewelry={
            .amount={.count=1, .sides=6},
            .percent_chance=25
        },
        .maps_or_magic={
            .percent_chance=15,
            .types={
                {
                    .amount={.count=2, .is_constant=true},
                    .is_map_possible=true,
                    .possible_magic_items=ANY_MAGIC_ITEM
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=POTION
                }
            },
            .types_count=2
        }
    },
    {
        .letter='E',
        .copper={
            .amount={.count=1, .sides=10, .multiplier=1000},
            .percent_chance=5
        },
        .silver={
            .amount={.count=1, .sides=12, .multiplier=1000},
            .percent_chance=25
        },
        .electrum={
            .amount={.count=1, .sides=6, .multiplier=1000},
            .percent_chance=25
        },
        .gold={
            .amount={.count=1, .sides=8, .multiplier=1000},
            .percent_chance=25
        },
        .gems={
            .amount={.count=1, .sides=12},
            .percent_chance=15
        },
        .jewelry={
            .amount={.count=1, .sides=8},
            .percent_chance=10
        },
        .maps_or_magic={
            .percent_chance=25,
            .types={
                {
                    .amount={.count=3, .is_constant=true},
                    .is_map_possible=true,
                    .possible_magic_items=ANY_MAGIC_ITEM
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=SCROLL
                }
            },
            .types_count=2
        }
    },
    {
        .letter='F',
        .silver={
            .amount={.count=1, .sides=20, .multiplier=1000},
            .percent_chance=10
        },
        .electrum={
            .amount={.count=1, .sides=12, .multiplier=1000},
            .percent_chance=15
        },
        .gold={
            .amount={.count=1, .sides=10, .multiplier=1000},
            .percent_chance=40
        },
        .platinum={
            .amount={.count=1, .sides=8, .multiplier=100},
            .percent_chance=35
        },
        .gems={
            .amount={.count=3, .sides=10},
            .percent_chance=20
        },
        .jewelry={
            .amount={.count=1, .sides=10},
            .percent_chance=10
        },
        .maps_or_magic={
            .percent_chance=30,
            .types={
                {
                    .amount={.count=3, .is_constant=true},
                    .is_map_possible=true,
                    .possible_magic_items=NON_WEAPON_MAGIC
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=POTION
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=SCROLL
                }
            },
            .types_count=3
        }
    },
    {
        .letter='G',
        .gold={
            .amount={.count=10, .sides=4, .multiplier=1000},
            .percent_chance=50
        },
        .platinum={
            .amount={.count=1, .sides=20, .multiplier=100},
            .percent_chance=50
        },
        .gems={
            .amount={.count=5, .sides=4},
            .percent_chance=30
        },
        .jewelry={
            .amount={.count=1, .sides=10},
            .percent_chance=25
        },
        .maps_or_magic={
            .percent_chance=35,
            .types={
                {
                    .amount={.count=4, .is_constant=true},
                    .is_map_possible=true,
                    .possible_magic_items=ANY_MAGIC_ITEM
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=SCROLL
                }
            },
            .types_count=2
        }
    },
    {
        .letter='H',
        .copper={
            .amount={.count=5, .sides=6, .multiplier=1000},
            .percent_chance=25
        },
        .silver={
            .amount={.count=1, .sides=100, .multiplier=1000},
            .percent_chance=40
        },
        .electrum={
            .amount={.count=10, .sides=4, .multiplier=1000},
            .percent_chance=40
        },
        .gold={
            .amount={.count=10, .sides=6, .multiplier=1000},
            .percent_chance=55
        },
        .platinum={
            .amount={.count=5, .sides=10, .multiplier=100},
            .percent_chance=25
        },
        .gems={
            .amount={.count=1, .sides=100},
            .percent_chance=50
        },
        .jewelry={
            .amount={.count=10, .sides=4},
            .percent_chance=50
        },
        .maps_or_magic={
            .percent_chance=15,
            .types={
                {
                    .amount={.count=4, .is_constant=true},
                    .is_map_possible=true,
                    .possible_magic_items=ANY_MAGIC_ITEM
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=POTION
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=SCROLL
                }
            },
            .types_count=3
        }
    },
    {
        .letter='I',
        .platinum={
            .amount={.count=3, .sides=6, .multiplier=100},
            .percent_chance=30
        },
        .gems={
            .amount={.count=2, .sides=10},
            .percent_chance=55
        },
        .jewelry={
            .amount={.count=1, .sides=12},
            .percent_chance=50
        },
        .maps_or_magic={
            .percent_chance=15,
            .types={
                {
                    .amount={.count=1, .is_constant=true},
                    .is_map_possible=true,
                    .possible_magic_items=ANY_MAGIC_ITEM
                }
            },
            .types_count=1
        }
    },
    {
        .letter='J',
        .copper={
            .amount={.count=3, .sides=8},
            .percent_chance=100,
            .is_per_individual=true
        },
    },
    {
        .letter='K',
        .silver={
            .amount={.count=3, .sides=6},
            .percent_chance=100,
            .is_per_individual=true
        },
    },
    {
        .letter='L',
        .electrum={
            .amount={.count=2, .sides=6},
            .percent_chance=100,
            .is_per_individual=true
        },
    },
    {
        .letter='M',
        .gold={
            .amount={.count=2, .sides=4},
            .percent_chance=100,
            .is_per_individual=true
        },
    },
    {
        .letter='N',
        .platinum={
            .amount={.count=1, .sides=6},
            .percent_chance=100,
            .is_per_individual=true
        },
    },
    {
        .letter='O',
        .copper={
            .amount={.count=1, .sides=4, .multiplier=1000},
            .percent_chance=25
        },
        .silver={
            .amount={.count=1, .sides=3, .multiplier=1000},
            .percent_chance=20
        },
    },
    {
        .letter='P',
        .silver={
            .amount={.count=1, .sides=6, .multiplier=1000},
            .percent_chance=30
        },
        .electrum={
            .amount={.count=1, .sides=2, .multiplier=1000},
            .percent_chance=25
        },
    },
    {
        .letter='Q',
        .gems={
            .amount={.count=1, .sides=4},
            .percent_chance=50
        },
    },
    {
        .letter='R',
        .gold={
            .amount={.count=2, .sides=4, .multiplier=1000},
            .percent_chance=40
        },
        .platinum={
            .amount={.count=10, .sides=6, .multiplier=100},
            .percent_chance=50
        },
        .gems={
            .amount={.count=4, .sides=8},
            .percent_chance=55
        },
        .jewelry={
            .amount={.count=1, .sides=12},
            .percent_chance=45
        },
    },
    {
        .letter='S',
        .maps_or_magic={
            .percent_chance=40,
            .types={
                {
                    .amount={.count=2, .sides=4},
                    .possible_magic_items=POTION
                }
            },
            .types_count=1
        }
    },
    {
        .letter='T',
        .maps_or_magic={
            .percent_chance=50,
            .types={
                {
                    .amount={.count=1, .sides=4},
                    .possible_magic_items=SCROLL
                }
            },
            .types_count=1
        }
    },
    {
        .letter='U',
        .gems={
            .amount={.count=10, .sides=8},
            .percent_chance=90
        },
        .jewelry={
            .amount={.count=5, .sides=6},
            .percent_chance=80
        },
        .maps_or_magic={
            .percent_chance=70,
            .types={
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=RING
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=ROD_STAFF_WAND
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=MISC_MAGIC
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=ARMOR_SHIELD
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=SWORD
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=MISC_WEAPON
                }
            },
            .types_count=6
        }
    },
    {
        .letter='V',
        .maps_or_magic={
            .percent_chance=85,
            .types={
                {
                    .amount={.count=2, .is_constant=true},
                    .possible_magic_items=RING
                },
                {
                    .amount={.count=2, .is_constant=true},
                    .possible_magic_items=ROD_STAFF_WAND
                },
                {
                    .amount={.count=2, .is_constant=true},
                    .possible_magic_items=MISC_MAGIC
                },
                {
                    .amount={.count=2, .is_constant=true},
                    .possible_magic_items=ARMOR_SHIELD
                },
                {
                    .amount={.count=2, .is_constant=true},
                    .possible_magic_items=SWORD
                },
                {
                    .amount={.count=2, .is_constant=true},
                    .possible_magic_items=MISC_WEAPON
                }
            },
            .types_count=6
        }
    },
    {
        .letter='W',
        .gold={
            .amount={.count=5, .sides=6, .multiplier=1000},
            .percent_chance=60
        },
        .platinum={
            .amount={.count=1, .sides=8, .multiplier=100},
            .percent_chance=15
        },
        .gems={
            .amount={.count=10, .sides=8},
            .percent_chance=60
        },
        .jewelry={
            .amount={.count=5, .sides=8},
            .percent_chance=50
        },
        .maps_or_magic={
            .percent_chance=55,
            .types={
                {
                    .amount={.count=1, .is_constant=true},
                    .is_map_possible=true,
                    .possible_magic_items=NO_MAGIC_ITEM
                }
            },
            .types_count=1
        }
    },
    {
        .letter='X',
        .maps_or_magic={
            .percent_chance=60,
            .types={
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=MISC_MAGIC
                },
                {
                    .amount={.count=1, .is_constant=true},
                    .possible_magic_items=POTION
                }
            },
            .types_count=2
        }
    },
    {
        .letter='Y',
        .gold={
            .amount={.count=2, .sides=6, .multiplier=1000},
            .percent_chance=70
        },
    },
    {
        .letter='Z',
        .copper={
            .amount={.count=1, .sides=3, .multiplier=1000},
            .percent_chance=20
        },
        .silver={
            .amount={.count=1, .sides=4, .multiplier=1000},
            .percent_chance=25
        },
        .electrum={
            .amount={.count=1, .sides=4, .multiplier=1000},
            .percent_chance=25
        },
        .gold={
            .amount={.count=1, .sides=4, .multiplier=1000},
            .percent_chance=30
        },
        .platinum={
            .amount={.count=1, .sides=6, .multiplier=100},
            .percent_chance=30
        },
        .gems={
            .amount={.count=10, .sides=6},
            .percent_chance=55
        },
        .jewelry={
            .amount={.count=5, .sides=6},
            .percent_chance=50
        },
        .maps_or_magic={
            .percent_chance=50,
            .types={
                {
                    .amount={.count=3, .is_constant=true},
                    .possible_magic_items=ANY_MAGIC_ITEM
                }
            },
            .types_count=1
        }
    }
};


static char *
describe_base_range(struct amount *amount);

static char *
describe_coins_gems_or_jewelry(struct coins_gems_or_jewelry *coins_gems_or_jewelry);

static char *
describe_maps_or_magic(struct maps_or_magic *maps_or_magic);

static void
generate_coins(int *coins,
               struct rnd *rnd,
               struct coins_gems_or_jewelry *coins_type);

static void
generate_gems(struct treasure *treasure, struct rnd *rnd);

static void
generate_jewelry(struct treasure *treasure, struct rnd *rnd);

static void
generate_maps_or_magic(struct treasure *treasure, struct rnd *rnd);

static void
generate_maps_or_magic_items(struct maps_or_magic_type *type,
                             struct treasure *treasure,
                             struct rnd *rnd);

static int
max_amount(struct amount *amount);

static char const *
possible_maps_or_magic_name(bool is_map_possible,
                            possible_magic_items_t possible_magic_items);

static int
xroll_amount(struct amount *amount, struct rnd *rnd);


char *
treasure_type_alloc_description(struct treasure_type *treasure_type,
                                bool include_header)
{
    /*          1000       1000       1000       1000       100                          */
    /* Type    copper     silver    electrum     gold     platinum     gems      jewelry */
    /*  A     1-6 :25%   1-6 :30%   1-6 :35%   1-10:40%   1-4 :25%   4-40:60%   3-30:50% */
    /*                                                                                   */
    /*  J    3-24 pieces                                                                 */
    /*      per individual                                                               */
    static char const *header =
    "          |  1,000's  |  1,000's  |  1,000's  |  1,000's  |   100's   |           |           |    Maps   \n"
    "Treasure  |    of     |    of     |    of     |    of     |    of     |           |           |     or    \n"
    "  Type    |  Copper   |  Silver   | Electrum  |   Gold    | Platinum  |   Gems    |  Jewelry  |   Magic   \n"
    "----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+-----------\n";
    char *letter_format = "    %c    ";
    
    char *letter;
    char *copper = describe_coins_gems_or_jewelry(&treasure_type->copper);
    char *silver = describe_coins_gems_or_jewelry(&treasure_type->silver);
    char *electrum = describe_coins_gems_or_jewelry(&treasure_type->electrum);
    char *gold = describe_coins_gems_or_jewelry(&treasure_type->gold);
    char *platinum = describe_coins_gems_or_jewelry(&treasure_type->platinum);
    char *gems = describe_coins_gems_or_jewelry(&treasure_type->gems);
    char *jewelry = describe_coins_gems_or_jewelry(&treasure_type->jewelry);
    char *maps_or_magic = describe_maps_or_magic(&treasure_type->maps_or_magic);
    
    letter = astr_f(letter_format, treasure_type->letter);
    
    char *format = "%s%s | %s | %s | %s | %s | %s | %s | %s | %s\n";
    char *description = astr_f(format,
                               (include_header ? header : ""),
                               letter, copper, silver, electrum,
                               gold, platinum, gems, jewelry,
                               maps_or_magic);
    free(letter);
    free(copper);
    free(silver);
    free(electrum);
    free(gold);
    free(platinum);
    free(gems);
    free(jewelry);
    free(maps_or_magic);

    return description;
}


char *
treasure_type_alloc_name(struct treasure_type *treasure_type)
{
    return astr_f("%c", treasure_type->letter);
}


static char *
describe_base_range(struct amount *amount)
{
    assert(amount);
    if (amount->is_constant) {
        return astr_f("%i", amount->count);
    } else {
        int low = amount->count;
        int high = amount->count * amount->sides;
        return astr_f("%i-%i", low, high);
    }
}


static char *
describe_coins_gems_or_jewelry(struct coins_gems_or_jewelry *coins_gems_or_jewelry)
{
    if ( ! coins_gems_or_jewelry->percent_chance) return xstrdup("   nil   ");

    if (coins_gems_or_jewelry->is_per_individual) {
        char *range = describe_base_range(&coins_gems_or_jewelry->amount);
        char *description = astr_centered_f(9, "%s per", range);
        free(range);
        return description;
    } else {
        char *range = describe_base_range(&coins_gems_or_jewelry->amount);
        char *description = astr_centered_f(9, "%s:%2i%%", range,
                                            coins_gems_or_jewelry->percent_chance);
        free(range);
        return description;
    }
}


static char *
describe_maps_or_magic(struct maps_or_magic *maps_or_magic)
{
    if ( ! maps_or_magic->percent_chance) return xstrdup("   nil   ");

    char *type_descriptions[6] = {};
    for (int i = 0; i < maps_or_magic->types_count; ++i) {
        struct maps_or_magic_type *type = &maps_or_magic->types[i];
        char const *type_name = possible_maps_or_magic_name(type->is_map_possible,
                                                            type->possible_magic_items);
        char *range = describe_base_range(&type->amount);
        if ( ! type->amount.is_constant) {
            type_descriptions[i] = astr_f("%s %ss",
                                          range, type_name);
        } else if (   type->is_map_possible
                   && type->possible_magic_items == ANY_MAGIC_ITEM)
        {
            type_descriptions[i] = astr_f("any %s", range);
        } else if (type->possible_magic_items == NON_WEAPON_MAGIC) {
            type_descriptions[i] = astr_f("any %s except sword or misc weapon",
                                          range);
        } else if (type->possible_magic_items == ANY_MAGIC_ITEM) {
            type_descriptions[i] = astr_f("any %s magic", range);
        } else {
            char const *plural = (1 == max_amount(&type->amount)) ? "" : "s";
            type_descriptions[i] = astr_f("%s %s%s", range,
                                          type_name, plural);
        }
        free(range);
    }

    char *description = NULL;
    for (int i = 0; i < maps_or_magic->types_count; ++i) {
        char const *separator = (i) ? ", " : "";
        description = astr_cat_f(description, "%s%s",
                                 separator, type_descriptions[i]);
        free(type_descriptions[i]);
    }
    description = astr_cat_f(description, ": %i%%",
                             maps_or_magic->percent_chance);
    return description;
}


void
treasure_type_generate(struct treasure_type *treasure_type,
                       struct rnd *rnd,
                       struct treasure *treasure)
{
    treasure->type = treasure_type;
    
    generate_coins(&treasure->coins.cp, rnd, &treasure_type->copper);
    generate_coins(&treasure->coins.sp, rnd, &treasure_type->silver);
    generate_coins(&treasure->coins.ep, rnd, &treasure_type->electrum);
    generate_coins(&treasure->coins.gp, rnd, &treasure_type->gold);
    generate_coins(&treasure->coins.pp, rnd, &treasure_type->platinum);
    generate_gems(treasure, rnd);
    generate_jewelry(treasure, rnd);
    generate_maps_or_magic(treasure, rnd);
}


static void
generate_coins(int *coins,
               struct rnd *rnd,
               struct coins_gems_or_jewelry *coins_type)
{
    *coins = 0;
    if ( ! coins_type->percent_chance) return;
    if (coins_type->is_per_individual) {
        *coins += xroll_amount(&coins_type->amount, rnd);
    } else {
        int percent_score = xroll("1d100", rnd);
        if (percent_score <= coins_type->percent_chance) {
            *coins = xroll_amount(&coins_type->amount, rnd);
        }
    }
}


static void
generate_gems(struct treasure *treasure, struct rnd *rnd)
{
    treasure->gems = NULL;
    treasure->gems_count = 0;
    if ( ! treasure->type->gems.percent_chance) return;
    int percent_score = xroll("1d100", rnd);
    if (percent_score <= treasure->type->gems.percent_chance) {
        treasure->gems_count = xroll_amount(&treasure->type->gems.amount, rnd);
        treasure->gems = xcalloc(treasure->gems_count, sizeof(struct gem));
        for (int i = 0; i < treasure->gems_count; ++i) {
            gem_initialize(&treasure->gems[i]);
            gem_generate(&treasure->gems[i], rnd);
        }
    }
}


static void
generate_jewelry(struct treasure *treasure, struct rnd *rnd)
{
    treasure->jewelry = NULL;
    treasure->jewelry_count = 0;
    if ( ! treasure->type->jewelry.percent_chance) return;
    int percent_score = xroll("1d100", rnd);
    if (percent_score <= treasure->type->jewelry.percent_chance) {
        treasure->jewelry_count = xroll_amount(&treasure->type->jewelry.amount, rnd);
        treasure->jewelry = xcalloc(treasure->jewelry_count,
                                    sizeof(struct jewelry));
        for (int i = 0; i < treasure->jewelry_count; ++i) {
            jewelry_initialize(&treasure->jewelry[i]);
            jewelry_generate(&treasure->jewelry[i], rnd);
        }
    }
}


static void
generate_maps_or_magic(struct treasure *treasure, struct rnd *rnd)
{
    if ( ! treasure->type->maps_or_magic.percent_chance) return;
    int percent_score = xroll("1d100", rnd);
    if (percent_score <= treasure->type->maps_or_magic.percent_chance) {
        for (int i = 0; i < treasure->type->maps_or_magic.types_count; ++i) {
            struct maps_or_magic_type *type = &treasure->type->maps_or_magic.types[i];
            generate_maps_or_magic_items(type, treasure, rnd);
        }
    }
}


static void
generate_maps_or_magic_items(struct maps_or_magic_type *type,
                             struct treasure *treasure,
                             struct rnd *rnd)
{
    int amount = xroll_amount(&type->amount, rnd);
    int magic_items_count = 0;
    int maps_count = 0;
    for (int i = 0; i < amount; ++i) {
        if (type->is_map_possible) {
            bool is_magic_item_possible = type->possible_magic_items & ANY_MAGIC_ITEM;
            if (is_magic_item_possible) {
                (xroll("1d100", rnd) <= 10) ? ++maps_count : ++magic_items_count;
            } else {
                ++maps_count;
            }
        } else {
            ++magic_items_count;
        }
    }
    
    if (maps_count) {
        treasure_generate_maps(treasure, rnd, maps_count);
    }
    if (magic_items_count) {
        treasure_generate_magic_items(treasure, rnd, magic_items_count,
                                      type->possible_magic_items);
    }
}


static int
max_amount(struct amount *amount)
{
    assert(amount);
    if (amount->is_constant) {
        return amount->count;
    } else {
        return amount->count * amount->sides * amount->multiplier;
    }
}


static char const *
possible_maps_or_magic_name(bool is_map_possible,
                            possible_magic_items_t possible_magic_items)
{
    if (is_map_possible) {
        switch (possible_magic_items) {
            case NO_MAGIC_ITEM: return "map";
            case ANY_MAGIC_ITEM: return "any";
            case NON_WEAPON_MAGIC: return "map or non weapon magic";
            default:
                unreachable("possible_magic_items_t = %0x", possible_magic_items);
                return "(map or magic)";
        }
    } else {
        switch (possible_magic_items) {
            case POTION: return "potion";
            case SCROLL: return "scroll";
            case RING: return "ring";
            case ROD_STAFF_WAND: return "rod/staff/wand";
            case MISC_MAGIC: return "misc magic";
            case ARMOR_SHIELD: return "armor/shield";
            case SWORD: return "sword";
            case MISC_WEAPON: return "misc weapon";
                
            case ANY_MAGIC_ITEM: return "any magic";
            case MAGIC_WEAPON_OR_ARMOR: return "sword, armor or misc weapon";
            default:
                unreachable("possible_magic_items_t = %0x", possible_magic_items);
                return "(magic)";
        }
    }
}


struct treasure_type *
treasure_type_by_letter(char letter)
{
    assert(letter >= 'A' && letter <= 'Z');
    return &treasure_types[letter - 'A'];
}


static int
xroll_amount(struct amount *amount, struct rnd *rnd)
{
    assert(amount);
    assert(rnd);

    if (amount->is_constant) {
        return amount->count;
    }

    struct xdice dice = xdice_make_plus_times(amount->count,
                                              amount->sides,
                                              0,
                                              amount->multiplier ? amount->multiplier : 1);
    return xdice_roll(dice, rnd, NULL);
}

#include "options.h"

#include <base/base.h>
#include <character/character.h>
#include <mechanics/mechanics.h>

#include "dungeon/dungeon.h"
#include "dungeon/dungeon_options.h"

#include "game/game.h"

#include "treasure/gem.h"
#include "treasure/jewelry.h"
#include "treasure/treasure_map.h"
#include "treasure/treasure_type.h"

#include "tui/app.h"
#include "tui/menu_view.h"


static void
check(FILE *out, uint32_t constant);

static void
enumerate_treasure_items(struct treasure *treasure, FILE *out);

static void
generate_character(struct rnd *rnd,
                   FILE *out,
                   enum characteristic_generation_method method);

static void
generate_each_treasure(struct rnd *rnd, FILE *out);

static void
generate_map(struct rnd *rnd, FILE *out);

static void
generate_magic_items(struct rnd *rnd, FILE *out, int count);

static void
generate_random_dungeon(struct rnd *rnd,
                        struct dungeon_options *dungeon_options,
                        FILE *out);

static void
generate_sample_dungeon(struct rnd *rnd, FILE *out);

static void
generate_treasure_type(struct rnd *rnd, FILE *out, char letter);

static void
generate_treasure_type_table(FILE *out);

static void
play_game(struct rnd *rnd);

static void
print_dungeon(struct dungeon *dungeon, FILE *out);

static void
run_tui(void);


static void
check(FILE *out, uint32_t constant)
{
    struct rnd *fake_rnd = rnd_alloc_fake_fixed(constant);
    struct dungeon_options *dungeon_options = dungeon_options_alloc_default();
    
    generate_treasure_type_table(out);
    generate_map(fake_rnd, out);
    generate_each_treasure(fake_rnd, out);
    generate_sample_dungeon(fake_rnd, out);
    generate_random_dungeon(fake_rnd, dungeon_options, out);
    generate_character(fake_rnd, out, characteristic_generation_method_simple);
    generate_character(fake_rnd, out, characteristic_generation_method_1);
    generate_character(fake_rnd, out, characteristic_generation_method_2);
    generate_character(fake_rnd, out, characteristic_generation_method_3);
    generate_character(fake_rnd, out, characteristic_generation_method_4);
    generate_character(fake_rnd, out, characteristic_generation_method_general_NPC);
    generate_character(fake_rnd, out, characteristic_generation_method_special_NPC);
    
    dungeon_options_free(dungeon_options);
    rnd_free(fake_rnd);
}


static void
enumerate_treasure_items(struct treasure *treasure, FILE *out)
{
    if (treasure->gems_count) {
        fprintf(out, "  Gems: --------------------------------\n");
        for (int i = 0; i < treasure->gems_count; ++i) {
            fprintf(out, "    %2i  %s\n", i + 1, treasure->gems[i].visible_description);
            fprintf(out, "          %s\n", treasure->gems[i].true_description);
        }
    }
    
    if (treasure->jewelry_count) {
        fprintf(out, "  Jewelry: -----------------------------\n");
        for (int i = 0; i < treasure->jewelry_count; ++i) {
            fprintf(out, "    %2i  %s\n",
                    i + 1, treasure->jewelry[i].true_description);
        }
    }
    
    if (treasure->maps_count) {
        fprintf(out, "  Maps: --------------------------------\n");
        for (int i = 0; i < treasure->maps_count; ++i) {
            fprintf(out, "    %2i  %s\n", i + 1, treasure->maps[i].true_description);
        }
    }
    
    if (treasure->magic_items_count) {
        fprintf(out, "  Magic Items: -------------------------\n");
        for (int i = 0; i < treasure->magic_items_count; ++i) {
            fprintf(out, "    %2i  %s\n",
                    i + 1, treasure->magic_items[i].true_description);
            if (treasure->magic_items[i].true_details) {
                int j = 0;
                while (treasure->magic_items[i].true_details[j]) {
                    fprintf(out, "            %s\n",
                            treasure->magic_items[i].true_details[j]);
                    ++j;
                }
            }
        }
    }
}


static void
generate_character(struct rnd *rnd,
                   FILE *out,
                   enum characteristic_generation_method method)
{
    char const *method_name = characteristic_generation_method_name(method);
    const char *method_description = characteristic_generation_method_description(method);

    enum ability_flag special_abilities = ability_flag_strength;
    struct characteristic_rolls *rolls = characteristic_rolls_alloc(rnd,
                                                                    method,
                                                                    special_abilities);
    if (characteristic_rolls_type_six_scores == rolls->type)
    {
        fprintf(out, "Character (%s): -------------------------\n", method_name);
        fprintf(out, "(%s)\n", method_description);
        for (int i = 0; i < 6; ++i) {
            fprintf(out, "  %2i) %i\n", i + 1, rolls->scores[i]);
        }
    } else if (characteristic_rolls_type_twelve_sets_of_six_characteristics == rolls->type) {
        fprintf(out, "Possible Characters (%s): -------------------------\n", method_name);
        fprintf(out, "(%s)\n", method_description);
        fprintf(out, "                 %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
        fprintf(out, "                 %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s\n",
                "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--");
        fprintf(out, "  Strength:      %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                rolls->characteristics_sets[0].strength,
                rolls->characteristics_sets[1].strength,
                rolls->characteristics_sets[2].strength,
                rolls->characteristics_sets[3].strength,
                rolls->characteristics_sets[4].strength,
                rolls->characteristics_sets[5].strength,
                rolls->characteristics_sets[6].strength,
                rolls->characteristics_sets[7].strength,
                rolls->characteristics_sets[8].strength,
                rolls->characteristics_sets[9].strength,
                rolls->characteristics_sets[10].strength,
                rolls->characteristics_sets[11].strength);
        fprintf(out, "  Intelligence:  %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                rolls->characteristics_sets[0].intelligence,
                rolls->characteristics_sets[1].intelligence,
                rolls->characteristics_sets[2].intelligence,
                rolls->characteristics_sets[3].intelligence,
                rolls->characteristics_sets[4].intelligence,
                rolls->characteristics_sets[5].intelligence,
                rolls->characteristics_sets[6].intelligence,
                rolls->characteristics_sets[7].intelligence,
                rolls->characteristics_sets[8].intelligence,
                rolls->characteristics_sets[9].intelligence,
                rolls->characteristics_sets[10].intelligence,
                rolls->characteristics_sets[11].intelligence);
        fprintf(out, "  Wisdom:        %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                rolls->characteristics_sets[0].wisdom,
                rolls->characteristics_sets[1].wisdom,
                rolls->characteristics_sets[2].wisdom,
                rolls->characteristics_sets[3].wisdom,
                rolls->characteristics_sets[4].wisdom,
                rolls->characteristics_sets[5].wisdom,
                rolls->characteristics_sets[6].wisdom,
                rolls->characteristics_sets[7].wisdom,
                rolls->characteristics_sets[8].wisdom,
                rolls->characteristics_sets[9].wisdom,
                rolls->characteristics_sets[10].wisdom,
                rolls->characteristics_sets[11].wisdom);
        fprintf(out, "  Dexterity:     %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                rolls->characteristics_sets[0].dexterity,
                rolls->characteristics_sets[1].dexterity,
                rolls->characteristics_sets[2].dexterity,
                rolls->characteristics_sets[3].dexterity,
                rolls->characteristics_sets[4].dexterity,
                rolls->characteristics_sets[5].dexterity,
                rolls->characteristics_sets[6].dexterity,
                rolls->characteristics_sets[7].dexterity,
                rolls->characteristics_sets[8].dexterity,
                rolls->characteristics_sets[9].dexterity,
                rolls->characteristics_sets[10].dexterity,
                rolls->characteristics_sets[11].dexterity);
        fprintf(out, "  Constitution:  %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                rolls->characteristics_sets[0].constitution,
                rolls->characteristics_sets[1].constitution,
                rolls->characteristics_sets[2].constitution,
                rolls->characteristics_sets[3].constitution,
                rolls->characteristics_sets[4].constitution,
                rolls->characteristics_sets[5].constitution,
                rolls->characteristics_sets[6].constitution,
                rolls->characteristics_sets[7].constitution,
                rolls->characteristics_sets[8].constitution,
                rolls->characteristics_sets[9].constitution,
                rolls->characteristics_sets[10].constitution,
                rolls->characteristics_sets[11].constitution);
        fprintf(out, "  Charisma:      %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                rolls->characteristics_sets[0].charisma,
                rolls->characteristics_sets[1].charisma,
                rolls->characteristics_sets[2].charisma,
                rolls->characteristics_sets[3].charisma,
                rolls->characteristics_sets[4].charisma,
                rolls->characteristics_sets[5].charisma,
                rolls->characteristics_sets[6].charisma,
                rolls->characteristics_sets[7].charisma,
                rolls->characteristics_sets[8].charisma,
                rolls->characteristics_sets[9].charisma,
                rolls->characteristics_sets[10].charisma,
                rolls->characteristics_sets[11].charisma);
    } else {
        fprintf(out, "Character (%s): -------------------------\n", method_name);
        fprintf(out, "(%s)\n", method_description);
        fprintf(out, "  Strength:     %2i\n", rolls->characteristics.strength);
        fprintf(out, "  Intelligence: %2i\n", rolls->characteristics.intelligence);
        fprintf(out, "  Wisdom:       %2i\n", rolls->characteristics.wisdom);
        fprintf(out, "  Dexterity:    %2i\n", rolls->characteristics.dexterity);
        fprintf(out, "  Constitution: %2i\n", rolls->characteristics.constitution);
        fprintf(out, "  Charisma:     %2i\n", rolls->characteristics.charisma);
    }
    characteristic_rolls_free(rolls);
}


static void
generate_each_treasure(struct rnd *rnd, FILE *out)
{
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        generate_treasure_type(rnd, out, letter);
    }
}


static void
generate_map(struct rnd *rnd, FILE *out)
{
    struct treasure_map treasure_map;
    treasure_map_initialize(&treasure_map);
    treasure_map_generate(&treasure_map, rnd);
    fprintf(out, "%s\n", treasure_map.true_description);
    enumerate_treasure_items(&treasure_map.treasure, out);
    treasure_map_finalize(&treasure_map);
}


static void
generate_magic_items(struct rnd *rnd, FILE *out, int count)
{
    struct magic_item magic_item;
    
    fprintf(out, "Magic Items:\n");
    for (int i = 0; i < count; ++i) {
        magic_item_initialize(&magic_item);
        magic_item_generate(&magic_item, rnd, ANY_MAGIC_ITEM);
        fprintf(out, "  %4i %s\n", (i + 1), magic_item.true_description);
        if (magic_item.true_details) {
            int j = 0;
            while (magic_item.true_details[j]) {
                fprintf(out, "           %s\n", magic_item.true_details[j]);
                ++j;
            }
        }
        magic_item_finalize(&magic_item);
    }
    fprintf(out, "\n");
}


static void
generate_random_dungeon(struct rnd *rnd,
                        struct dungeon_options *dungeon_options,
                        FILE *out)
{
    struct dungeon *dungeon = dungeon_alloc();
    dungeon_generate(dungeon, rnd, dungeon_options, NULL, NULL);
    print_dungeon(dungeon, out);
    dungeon_free(dungeon);
}


static void
generate_sample_dungeon(struct rnd *rnd, FILE *out)
{
    struct dungeon *dungeon = dungeon_alloc();
    dungeon_generate_small(dungeon);
    print_dungeon(dungeon, out);
    dungeon_free(dungeon);
}


static void
generate_treasure_type(struct rnd *rnd, FILE *out, char letter)
{
    struct treasure treasure;
    treasure_initialize(&treasure);
    treasure_type_generate(treasure_type_by_letter(letter), rnd, &treasure);
    
    struct ptr_array *lines = treasure_alloc_details(&treasure);
    
    treasure_finalize(&treasure);
    
    for (int i = 0; i < lines->count; ++i) {
        fprintf(out, "%s\n", lines->elements[i]);
    }
    
    ptr_array_clear(lines, free_or_die);
    ptr_array_free(lines);
}


static void
generate_treasure_type_table(FILE *out)
{
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        struct treasure_type *treasureType = treasure_type_by_letter(letter);
        char *description = treasure_type_alloc_description(treasureType, letter == 'A');
        fprintf(out, "%s", description);
        free_or_die(description);
    }
}


int
main(int argc, char *argv[])
{
    FILE *out = stdout;
    struct options *options = options_alloc(argc, argv);
    
    if (options->error || options->help) {
        options_print_usage(options);
        return options->error? EXIT_FAILURE : EXIT_SUCCESS;
    }
    
    if (action_game == options->action) {
        play_game(options->rnd);
    } else if (action_tui == options->action) {
        run_tui();
    } else {
        fprintf(out, "Fiends and Fortune\n");
        switch (options->action) {
            case action_character:
                generate_character(options->rnd, out, options->character_method);
                break;
            case action_check:
                check(out, options->check_constant);
                break;
            case action_dungeon:
                if (options->dungeon_type_small) {
                    generate_sample_dungeon(options->rnd, out);
                } else {
                    generate_random_dungeon(options->rnd,
                                            options->dungeon_options,
                                            out);
                }
                break;
            case action_each:
                generate_each_treasure(options->rnd, out);
                break;
            case action_magic:
                generate_magic_items(options->rnd, out, options->magic_count);
                break;
            case action_map:
                generate_map(options->rnd, out);
                break;
            case action_table:
                generate_treasure_type_table(out);
                break;
            case action_treasure:
                generate_treasure_type(options->rnd, out, options->treasure_type);
                break;
            default:
                fprintf(stderr, "%s: unrecognized option\n", options->command_name);
                break;
        }
        fprintf(out, "\n");
    }
    
    options_free(options);
    alloc_count_is_zero_or_die();
    return EXIT_SUCCESS;
}


static void
play_game(struct rnd *rnd)
{
    struct game *game = game_alloc_or_die(rnd);
    struct result result = game_show(game);
    if (result_is_success(result)) result = game_run(game);
    game_hide(game);
    if (!result_is_success(result)) result_print_error(result);
    game_free_or_die(game);
}


static void
print_dungeon(struct dungeon *dungeon, FILE *out)
{
    int starting_level = dungeon_starting_level(dungeon);
    int level_count = dungeon_level_count(dungeon);
    for (int i = 0; i < level_count; ++i) {
        if (i > 0) fprintf(out, "\n");
        int level = starting_level + i;
        fprintf(out, "Level %i\n", level);
        fprintf(out, "\n");
        dungeon_print_map_for_level(dungeon, level, out);
        fprintf(out, "\n");
        fprintf(out, "Level %i Areas of Interest:\n", level);
        dungeon_print_areas_for_level(dungeon, level, out);
    }
}


static void
run_tui(void)
{
    struct app *app = app_alloc();
    
    struct view *menu_view = menu_view_alloc("Select an Item");
    menu_view_add_item(menu_view, "Try Item One");
    menu_view_add_item(menu_view, "Item Two Is Better");
    menu_view_add_item(menu_view, "You'll Love Item Three");
    
    app_add_view(app, menu_view);
    
    struct result result = app_run(app);
    if (!result_is_success(result)) {
        result_print_error(result);
    }
    
    app_free(app);
}

#include "options.h"

#include "common/alloc_or_die.h"
#include "common/dice.h"
#include "common/result.h"
#include "common/rnd.h"

#include "character/character.h"

#include "dungeon/dungeon.h"

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
generate_random_dungeon(struct rnd *rnd, FILE *out);

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
    
    generate_treasure_type_table(out);
    generate_map(fake_rnd, out);
    generate_each_treasure(fake_rnd, out);
    generate_sample_dungeon(fake_rnd, out);
    generate_random_dungeon(fake_rnd, out);
    generate_character(fake_rnd, out, characteristic_generation_method_simple);
    generate_character(fake_rnd, out, characteristic_generation_method_1);
    generate_character(fake_rnd, out, characteristic_generation_method_2);
    generate_character(fake_rnd, out, characteristic_generation_method_3);
    generate_character(fake_rnd, out, characteristic_generation_method_4);
    generate_character(fake_rnd, out, characteristic_generation_method_general_NPC);
    generate_character(fake_rnd, out, characteristic_generation_method_special_NPC);
    
    rnd_free(fake_rnd);
}


static void
enumerate_treasure_items(struct treasure *treasure, FILE *out)
{
    if (treasure->gems_count) {
        fprintf(out, "  Gems: --------------------------------\n");
        for (int i = 0; i < treasure->gems_count; ++i) {
            fprintf(out, "    %2i  %s\n", i + 1, treasure->gems[i].true_description);
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
    uint32_t special_characteristics = STRENGTH;
    int *characteristics = alloc_characteristics(rnd, method, special_characteristics);
    if (   method == characteristic_generation_method_1
        || method == characteristic_generation_method_2)
    {
        fprintf(out, "Character (%s): -------------------------\n", method_name);
        fprintf(out, "(%s)\n", characteristic_generation_method_description(method));
        for (int i = 0; i < 6; ++i) {
            fprintf(out, "  %2i) %i\n", i + 1, characteristics[i]);
        }
    } else if (method == characteristic_generation_method_4) {
        fprintf(out, "Possible Characters (%s): -------------------------\n", method_name);
        fprintf(out, "(%s)\n", characteristic_generation_method_description(method));
        fprintf(out, "                 %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
        fprintf(out, "                 %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s   %2s\n",
                "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--", "--");
        fprintf(out, "  Strength:      %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[0], characteristics[0 + 6], characteristics[0 + 12],
                characteristics[0 + 18], characteristics[0 + 24], characteristics[0 + 30],
                characteristics[0 + 36], characteristics[0 + 42], characteristics[0 + 48],
                characteristics[0 + 54], characteristics[0 + 60], characteristics[0 + 66]);
        fprintf(out, "  Intelligence:  %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[1], characteristics[1 + 6], characteristics[1 + 12],
                characteristics[1 + 18], characteristics[1 + 24], characteristics[1 + 30],
                characteristics[1 + 36], characteristics[1 + 42], characteristics[1 + 48],
                characteristics[1 + 54], characteristics[1 + 60], characteristics[1 + 66]);
        fprintf(out, "  Wisdom:        %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[2], characteristics[2 + 6], characteristics[2 + 12],
                characteristics[2 + 18], characteristics[2 + 24], characteristics[2 + 30],
                characteristics[2 + 36], characteristics[2 + 42], characteristics[2 + 48],
                characteristics[2 + 54], characteristics[2 + 60], characteristics[2 + 66]);
        fprintf(out, "  Dexterity:     %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[3], characteristics[3 + 6], characteristics[3 + 12],
                characteristics[3 + 18], characteristics[3 + 24], characteristics[3 + 30],
                characteristics[3 + 36], characteristics[3 + 42], characteristics[3 + 48],
                characteristics[3 + 54], characteristics[3 + 60], characteristics[3 + 66]);
        fprintf(out, "  Constitution:  %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[4], characteristics[4 + 6], characteristics[4 + 12],
                characteristics[4 + 18], characteristics[4 + 24], characteristics[4 + 30],
                characteristics[4 + 36], characteristics[4 + 42], characteristics[4 + 48],
                characteristics[4 + 54], characteristics[4 + 60], characteristics[4 + 66]);
        fprintf(out, "  Charisma:      %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i   %2i\n",
                characteristics[5], characteristics[5 + 6], characteristics[5 + 12],
                characteristics[5 + 18], characteristics[5 + 24], characteristics[5 + 30],
                characteristics[5 + 36], characteristics[5 + 42], characteristics[5 + 48],
                characteristics[5 + 54], characteristics[5 + 60], characteristics[5 + 66]);
    } else {
        fprintf(out, "Character (%s): -------------------------\n", method_name);
        fprintf(out, "(%s)\n", characteristic_generation_method_description(method));
        fprintf(out, "  Strength:     %2i\n", characteristics[0]);
        fprintf(out, "  Intelligence: %2i\n", characteristics[1]);
        fprintf(out, "  Wisdom:       %2i\n", characteristics[2]);
        fprintf(out, "  Dexterity:    %2i\n", characteristics[3]);
        fprintf(out, "  Constitution: %2i\n", characteristics[4]);
        fprintf(out, "  Charisma:     %2i\n", characteristics[5]);
    }
    free_or_die(characteristics);
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
generate_random_dungeon(struct rnd *rnd, FILE *out)
{
    struct dungeon *dungeon = dungeon_alloc();
    dungeon_generate(dungeon, rnd);
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
    
    fprintf(out, "Treasure type %c", letter);
    int individual_count = 0;
    if (letter >= 'J' && letter <= 'N') {
        individual_count = roll("1d10", rnd);
        fprintf(out, " (%i individuals)", individual_count);
    }
    fprintf(out, "\n");
    
    treasure_initialize(&treasure);
    treasure_type_generate(treasure_type_by_letter(letter), rnd, individual_count, &treasure);
    
    char *description = treasure_alloc_description(&treasure);
    int value_cp = treasure_value_in_cp(&treasure);
    char *value_gp = coins_alloc_gp_cp_description(value_cp);
    fprintf(out, "  %s (total %s)\n", description, value_gp);
    free_or_die(value_gp);
    free_or_die(description);
    enumerate_treasure_items(&treasure, out);
    
    treasure_finalize(&treasure);
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
                    generate_random_dungeon(options->rnd, out);
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

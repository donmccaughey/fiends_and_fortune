cmake_minimum_required(VERSION 3.11)
project(fiends_and_fortune C)

set(CMAKE_C_STANDARD 99)
enable_testing()
add_compile_options(--coverage -g -O0)
set (CMAKE_EXE_LINKER_FLAGS --coverage)

find_library(form REQUIRED)
find_library(menu REQUIRED)
find_library(ncurses REQUIRED)


# ----- fiends command -----

add_executable(fiends
        src/fiends/game.c
        src/fiends/main.c
        src/fiends/selection.c
        )
target_link_libraries(fiends
        background
        base
        character
        dungeon
        magic
        mechanics
        treasure
        # external dependencies
        form
        menu
        ncurses
        )


# ----- fnf command -----

add_executable(fnf
        src/fnf/action.c
        src/fnf/options.c
        src/fnf/main.c)
target_link_libraries(fnf
        background
        base
        character
        dungeon
        magic
        mechanics
        treasure
        )


# ----- background library -----

add_library(background STATIC
        src/background/language.c
        src/background/money.c
        )
target_include_directories(background PUBLIC src)

add_executable(background_tests
        src/background/background_tests.c
        src/background/language_test.c
        src/background/money_test.c
        )
target_include_directories(background_tests PUBLIC src)
target_link_libraries(background_tests background base mechanics)
add_test(background_tests background_tests)


# ----- base library -----

add_library(base STATIC
        src/base/alloc_or_die.c
        src/base/fail.c
        src/base/int.c
        src/base/ptr_array.c
        src/base/result.c
        src/base/rnd.c
        src/base/str.c
        )
target_include_directories(base PUBLIC src)

add_executable(base_tests
        src/base/base_tests.c
        src/base/int_test.c
        src/base/ptr_array_test.c
        src/base/str_test.c
        src/base/result_test.c
        src/base/rnd_test.c
        )
target_include_directories(base_tests PUBLIC src)
target_link_libraries(base_tests base)
add_test(base_tests base_tests)


# ----- character library -----

add_library(character STATIC
        src/character/abilities.c
        src/character/ability_score_generation_method.c
        src/character/ability_scores.c
        src/character/ability_sets.c
        src/character/character.c
        )
target_include_directories(character PUBLIC src)

add_executable(character_tests
        src/character/abilities_test.c
        src/character/ability_score_generation_method_test.c
        src/character/ability_scores_test.c
        src/character/ability_sets_test.c
        src/character/character_tests.c
        )
target_include_directories(character_tests PUBLIC src)
target_link_libraries(character_tests character base mechanics)
add_test(character_tests character_tests)


# ----- dungeon library -----

add_library(dungeon STATIC
        src/dungeon/area.c
        src/dungeon/box.c
        src/dungeon/digger.c
        src/dungeon/direction.c
        src/dungeon/dungeon.c
        src/dungeon/dungeon_options.c
        src/dungeon/exit.c
        src/dungeon/generator.c
        src/dungeon/level_map.c
        src/dungeon/orientation.c
        src/dungeon/periodic_check.c
        src/dungeon/point.c
        src/dungeon/size.c
        src/dungeon/text_rectangle.c
        src/dungeon/tile.c
        )
target_include_directories(dungeon PUBLIC src)

add_executable(dungeon_tests
        src/dungeon/box_test.c
        src/dungeon/dungeon_test.c
        src/dungeon/dungeon_tests.c
        src/dungeon/generator_test.c
        src/dungeon/point_test.c
        )
target_include_directories(dungeon_tests PUBLIC src)
target_link_libraries(dungeon_tests dungeon base mechanics)
add_test(dungeon_tests dungeon_tests)


# ----- magic library -----

add_library(magic STATIC
        src/magic/spell.c
        )
target_include_directories(magic PUBLIC src)

add_executable(magic_tests
        src/magic/magic_tests.c
        src/magic/spell_test.c
        )
target_include_directories(magic_tests PUBLIC src)
target_link_libraries(magic_tests magic base mechanics)
add_test(magic_tests magic_tests)


# ----- mechanics library -----

add_library(mechanics STATIC
        src/mechanics/dice.c
        )
target_include_directories(mechanics PUBLIC src)

add_executable(mechanics_tests
        src/mechanics/dice_test.c
        src/mechanics/mechanics_tests.c
        )
target_include_directories(mechanics_tests PUBLIC src)
target_link_libraries(mechanics_tests mechanics base)
add_test(mechanics_tests mechanics_tests)


# ----- treasure library -----

add_library(treasure STATIC
        src/treasure/coins.c
        src/treasure/gem.c
        src/treasure/jewelry.c
        src/treasure/magic_item.c
        src/treasure/treasure.c
        src/treasure/treasure_map.c
        src/treasure/treasure_type.c
        )
target_include_directories(treasure PUBLIC src)

add_executable(treasure_tests
        src/treasure/coins_test.c
        src/treasure/gem_test.c
        src/treasure/jewelry_test.c
        src/treasure/magic_item_test.c
        src/treasure/treasure_map_test.c
        src/treasure/treasure_test.c
        src/treasure/treasure_tests.c
        src/treasure/treasure_type_test.c
        )
target_include_directories(treasure_tests PUBLIC src)
target_link_libraries(treasure_tests treasure background base magic mechanics)
add_test(treasure_tests treasure_tests)



# ----- tui library -----

add_library(tui STATIC
        src/tui/app.c
        src/tui/menu_view.c
        src/tui/rect.c
        src/tui/screen_view.c
        src/tui/view.c
        src/tui/window.c
        )
target_include_directories(tui PUBLIC src)

add_executable(tui_example
        src/tui/tui_example.c
        )
target_include_directories(tui_example PUBLIC src)
target_link_libraries(tui_example
        tui
        base
        # external dependencies
        form
        menu
        ncurses
        )


# ----- functional tests -----

configure_file(tests/fnf_check.out fnf_check.out)
configure_file(tests/fnf_check fnf_check)
configure_file(tests/precheck precheck)
add_test(fnf_check fnf_check)


# ----- build all targets and run all tests -----

add_custom_target(check
        COMMAND ./precheck
        COMMAND background_tests
        COMMAND base_tests
        COMMAND character_tests
        COMMAND dungeon_tests
        COMMAND magic_tests
        COMMAND mechanics_tests
        COMMAND treasure_tests
        COMMAND ./fnf_check
        )
add_dependencies(check
        fiends
        fnf
        background_tests
        base_tests
        character_tests
        dungeon_tests
        magic_tests
        mechanics_tests
        treasure_tests
        tui_example
        )

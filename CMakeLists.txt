cmake_minimum_required(VERSION 3.11)
project(fiends_and_fortune C)

set(CMAKE_C_STANDARD 99)
enable_testing()
add_compile_options(--coverage -g -O0)
set (CMAKE_EXE_LINKER_FLAGS --coverage)

find_library(form REQUIRED)
find_library(menu REQUIRED)
find_library(ncurses REQUIRED)


# ----- fnf command -----

add_executable(fnf
        src/fnf/action.c
        src/fnf/options.c
        src/fnf/main.c)
target_link_libraries(fnf base mechanics background magic libfnf form menu ncurses)


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


# ----- background library -----

add_library(background STATIC
        src/background/language.c
        )
target_include_directories(background PUBLIC src)

add_executable(background_tests
        src/background/background_tests.c
        src/background/language_test.c
        )
target_include_directories(background_tests PUBLIC src)
target_link_libraries(background_tests background base mechanics)
add_test(background_tests background_tests)


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


# ----- fnf library -----

add_library(libfnf STATIC
        src/character/character.c
        src/character/method.c

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

        src/game/game.c
        src/game/selection.c

        src/treasure/coins.c
        src/treasure/gem.c
        src/treasure/jewelry.c
        src/treasure/magic_item.c
        src/treasure/treasure.c
        src/treasure/treasure_map.c
        src/treasure/treasure_type.c

        src/tui/app.c
        src/tui/menu_view.c
        src/tui/rect.c
        src/tui/screen_view.c
        src/tui/view.c
        src/tui/window.c
        )
target_include_directories(libfnf PUBLIC src)
set_target_properties(libfnf PROPERTIES OUTPUT_NAME "fnf")


# ----- unit tests -----

add_executable(unit_tests
        tests/unit_tests/dungeon/box_test.c
        tests/unit_tests/dungeon/dungeon_test.c
        tests/unit_tests/dungeon/generator_test.c
        tests/unit_tests/dungeon/point_test.c

        tests/unit_tests/main.c
        )
target_include_directories(unit_tests PUBLIC src/dungeon)
target_link_libraries(unit_tests base mechanics libfnf)
add_test(unit_tests unit_tests)


# ----- functional tests -----

configure_file(tests/fnf_check.out fnf_check.out)
configure_file(tests/fnf_check fnf_check)
configure_file(tests/precheck precheck)
add_test(fnf_check fnf_check)


# ----- run all tests -----

add_custom_target(check
        COMMAND ./precheck
        COMMAND background_tests
        COMMAND base_tests
        COMMAND magic_tests
        COMMAND mechanics_tests
        COMMAND unit_tests
        COMMAND ./fnf_check
        )
add_dependencies(check fnf unit_tests)

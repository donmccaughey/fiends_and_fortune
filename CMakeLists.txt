cmake_minimum_required(VERSION 3.12)
project(fiends_and_fortune C)

set(CMAKE_C_STANDARD 99)


find_library(form REQUIRED)
find_library(menu REQUIRED)
find_library(ncurses REQUIRED)


# ----- command -----

add_executable(fnf
        src/fnf/action.c
        src/fnf/options.c
        src/fnf/main.c
        )
target_link_libraries(fnf libfnf form menu ncurses)


# ----- library -----

add_library(libfnf STATIC
        src/character/character.c
        src/character/method.c

        src/common/alloc_or_die.c
        src/common/dice.c
        src/common/fail.c
        src/common/int.c
        src/common/language.c
        src/common/ptr_array.c
        src/common/result.c
        src/common/rnd.c
        src/common/spell.c
        src/common/str.c

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
set_target_properties(libfnf PROPERTIES PREFIX "")


# ----- unit tests -----

add_executable(unit_tests
        tests/unit_tests/dungeon/box_test.c
        tests/unit_tests/dungeon/dungeon_test.c
        tests/unit_tests/dungeon/generator_test.c
        tests/unit_tests/dungeon/point_test.c

        tests/unit_tests/dice_test.c
        tests/unit_tests/main.c
        tests/unit_tests/ptr_array_test.c
        tests/unit_tests/str_test.c
        )
target_include_directories(unit_tests PUBLIC src/dungeon)
target_link_libraries(unit_tests libfnf)


# ----- functional tests -----

add_custom_target(check
        COMMAND ./check.sh
        )
add_dependencies(check fnf unit_tests)

configure_file(tests/check.out check.out)
configure_file(tests/check.sh check.sh)

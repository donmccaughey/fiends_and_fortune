cmake_minimum_required(VERSION 3.17)
project(fiends_and_fortune
        VERSION 0.0.1
        DESCRIPTION "Fiends and Fortune"
        HOMEPAGE_URL https://github.com/donmccaughey/fiends_and_fortune
        LANGUAGES C
        )

option(COVERAGE "Enable code coverage analysis")
option(GRAPH_TARGETS "Generate a graph of build targets")

set(CMAKE_C_STANDARD 99)
enable_testing()

if(COVERAGE)
    add_compile_options(--coverage -g -O0)
    set(CMAKE_EXE_LINKER_FLAGS --coverage)
endif()


# ----- find external dependencies -----

find_library(LIB_NCURSES
        NAMES libncurses.a
        HINTS /usr/local/opt/ncurses/lib
        )
if(NOT LIB_NCURSES)
    message(FATAL_ERROR "Unable to locate ncurses library")
endif()

find_library(LIB_MENU
        NAMES libmenu.a
        HINTS /usr/local/opt/ncurses/lib
        )
if(NOT LIB_MENU)
    message(FATAL_ERROR "Unable to locate ncurses menu library")
endif()

find_library(LIB_FORM
        NAMES libform.a
        HINTS /usr/local/opt/ncurses/lib
        )
if(NOT LIB_FORM)
    message(FATAL_ERROR "Unable to locate ncurses form library")
endif()

# ----- add build files in subdirectories -----

add_subdirectory(libs/cJSON)
add_subdirectory(src/background)
add_subdirectory(src/base)
add_subdirectory(src/character)
add_subdirectory(src/dungeon)
add_subdirectory(src/fiends)
add_subdirectory(src/json)
add_subdirectory(src/magic)
add_subdirectory(src/mechanics)
add_subdirectory(src/treasure)


# ----- fnf command -----

add_executable(fnf
        src/fnf/action.c
        src/fnf/options.c
        src/fnf/main.c)
target_link_libraries(fnf
        background
        base
        character
        cJSON
        dungeon
        json
        magic
        mechanics
        treasure
        )

add_executable(fnf_tests
        src/fnf/action.c
        src/fnf/action_test.c
        src/fnf/fnf_tests.c
        src/fnf/options.c
        src/fnf/options_test.c
        )
target_include_directories(fnf_tests PUBLIC src)
target_link_libraries(fnf_tests
        background
        base
        character
        cJSON
        dungeon
        json
        magic
        mechanics
        treasure
        )
add_test(fnf_tests fnf_tests)


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
target_link_libraries(tui
        base
        ${LIB_FORM}
        ${LIB_MENU}
        ${LIB_NCURSES}
        )

add_executable(tui_example
        src/tui/tui_example.c
        )
target_include_directories(tui_example PUBLIC src)
target_link_libraries(tui_example
        tui
        base
        ${LIB_FORM}
        ${LIB_MENU}
        ${LIB_NCURSES}
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
        COMMAND fnf_tests
        COMMAND json_tests
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
        fnf_tests
        json_tests
        magic_tests
        mechanics_tests
        treasure_tests
        tui_example
        )


# ----- generate a graph of build targets -----

if(GRAPH_TARGETS)
    add_custom_target(target_graph ALL
            COMMAND "${CMAKE_COMMAND}" "--graphviz=${CMAKE_BINARY_DIR}/fnf_targets.dot" .
            COMMAND dot -Tpng -o "${CMAKE_BINARY_DIR}/fnf_targets.png" "${CMAKE_BINARY_DIR}/fnf_targets.dot"
            )
endif()

cmake_minimum_required(VERSION 3.13...3.18)
project(fiends_and_fortune
        VERSION 0.0.1
        DESCRIPTION "Fiends and Fortune"
        HOMEPAGE_URL https://github.com/donmccaughey/fiends_and_fortune
        LANGUAGES C
        )


include(CheckSymbolExists)


option(ADDRESS_SANITIZER "Enable the address sanitizer")
option(COVERAGE "Enable code coverage analysis")
option(GRAPH_TARGETS "Generate a graph of build targets")
option(HOMEBREW_NCURSES "Use Homebrew ncurses instead of macOS system ncurses")
set(HOMEBREW_NCURSES_PATH
    "/opt/homebrew/opt/ncurses"
    CACHE PATH
    "Location of the Homebrew 'ncurses' directory"
    )
option(WALL "Enable all warnings")


set(CMAKE_C_STANDARD 99)
enable_testing()

if(ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address -g)
    add_link_options(-fsanitize=address)
endif()

if(COVERAGE)
    add_compile_options(--coverage -g -O0)
    add_link_options(--coverage)
endif()

if(WALL)
    add_compile_options(-Wall -Werror)
endif()


# ----- find external dependencies -----

if(HOMEBREW_NCURSES)
    include_directories("${HOMEBREW_NCURSES_PATH}/include")
    link_directories("${HOMEBREW_NCURSES_PATH}/lib")
    set(LIB_NCURSES_HINTS "${HOMEBREW_NCURSES_PATH}/lib")
else()
    set(LIB_NCURSES_HINTS "")
endif()

find_library(LIB_NCURSES 
    NAMES ncurses
    HINTS ${LIB_NCURSES_HINTS}
    )
if(NOT LIB_NCURSES)
    message(FATAL_ERROR "Unable to locate ncurses library")
endif()

find_library(LIB_MENU 
    NAMES menu
    HINTS ${LIB_NCURSES_HINTS}
    )
if(NOT LIB_MENU)
    message(FATAL_ERROR "Unable to locate ncurses menu library")
endif()

find_library(LIB_FORM 
    NAMES form
    HINTS ${LIB_NCURSES_HINTS}
    )
if(NOT LIB_FORM)
    message(FATAL_ERROR "Unable to locate ncurses form library")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_library(LIB_BSD
        NAMES bsd
        )
    if(NOT LIB_BSD)
        message(FATAL_ERROR "Unable to locate bsd compatibility library")
    endif()
    find_path(LIB_BSD_PATH 
        "bsd.h" 
        PATH_SUFFIXES bsd
        )
    if(NOT LIB_BSD_PATH)
        message(FATAL_ERROR "Unable to locate libbsd header files")
    endif()
    include_directories(BEFORE SYSTEM "${LIB_BSD_PATH}")
    add_definitions(-DLIBBSD_OVERLAY)
    link_libraries(bsd)
endif()


# ----- compatibility checks -----

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    # make asprintf() visible
    add_definitions(-D_GNU_SOURCE)
endif()

check_symbol_exists(optreset "getopt.h" HAS_OPTRESET)


# ----- add build files in subdirectories -----

add_subdirectory(libs/cJSON)
add_subdirectory(src/background)
add_subdirectory(src/base)
add_subdirectory(src/character)
add_subdirectory(src/dungeon)
add_subdirectory(src/fiends)
add_subdirectory(src/fnf)
add_subdirectory(src/json)
add_subdirectory(src/magic)
add_subdirectory(src/mechanics)
add_subdirectory(src/treasure)
add_subdirectory(src/tui)


# ----- build all targets and run all tests -----

add_custom_target(check
        COMMAND background_tests
        COMMAND base_tests
        COMMAND character_tests
        COMMAND dungeon_tests
        COMMAND fnf_tests
        COMMAND json_tests
        COMMAND magic_tests
        COMMAND mechanics_tests
        COMMAND treasure_tests
        )
add_dependencies(check
        fiends
        fnf
        fnf_check
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

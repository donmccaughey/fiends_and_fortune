cmake_minimum_required(VERSION 3.15...3.27)
project(fiends_and_fortune
        VERSION 0.0.1
        DESCRIPTION "Fiends and Fortune"
        HOMEPAGE_URL https://github.com/donmccaughey/fiends_and_fortune
        LANGUAGES C CXX
        )


include(CheckCXXCompilerFlag)
include(CheckSymbolExists)


option(ADDRESS_SANITIZER "Enable the address sanitizer")
option(COVERAGE "Enable code coverage analysis")
option(GRAPH_TARGETS "Generate a graph of build targets")
option(WALL "Enable all warnings")


set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
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
    add_compile_options(-Wall -Werror -Wno-deprecated-declarations)
endif()


# ----- find external dependencies -----

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
    add_definitions(-D_GNU_SOURCE)  # make asprintf() visible
endif()

check_cxx_compiler_flag(-Wno-unqualified-std-cast-call HAS_NO_UNQUALIFIED_STD_CAST_CALL)

check_symbol_exists(optreset "getopt.h" HAS_OPTRESET)


# ----- add build files in subdirectories -----

add_subdirectory(libs/cJSON)
add_subdirectory(libs/tvision)
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


# ----- build all targets and run all tests -----

add_custom_target(check
        COMMAND background_tests
        COMMAND base_tests
        COMMAND character_tests
        COMMAND dungeon_tests
        COMMAND fiends_tests
        COMMAND fnf_tests
        COMMAND json_tests
        COMMAND magic_tests
        COMMAND mechanics_tests
        COMMAND treasure_tests
        )
add_dependencies(check
        fnf
        fnf_check
        background_tests
        base_tests
        character_tests
        dungeon_tests
        fiends_tests
        fnf_tests
        json_tests
        magic_tests
        mechanics_tests
        treasure_tests
        )


# ----- generate a graph of build targets -----

if(GRAPH_TARGETS)
    add_custom_target(target_graph ALL
            COMMAND "${CMAKE_COMMAND}" "--graphviz=${CMAKE_BINARY_DIR}/fnf_targets.dot" .
            COMMAND dot -Tpng -o "${CMAKE_BINARY_DIR}/fnf_targets.png" "${CMAKE_BINARY_DIR}/fnf_targets.dot"
            )
endif()

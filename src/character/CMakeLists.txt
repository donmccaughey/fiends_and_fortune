add_library(character STATIC
        abilities.h
        abilities.c
        ability_score_generation_method.h
        ability_score_generation_method.c
        ability_score_generation_type.h
        ability_scores.h
        ability_scores.c
        ability_sets.h
        ability_sets.c
        character.h
        character.c
)
if(WALL)
    target_compile_options(character
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_include_directories(character
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_link_libraries(character
        PUBLIC base
        PUBLIC mechanics
)

add_executable(character_tests
        abilities_test.c
        ability_score_generation_method_test.c
        ability_scores_test.c
        ability_sets_test.c
        character_tests.c
)
if(WALL)
    target_compile_options(character_tests
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_link_libraries(character_tests character)
add_test(character_tests character_tests)

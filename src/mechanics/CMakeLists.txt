add_library(mechanics STATIC
        dice.c
        )
target_include_directories(mechanics
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
target_link_libraries(mechanics
        PUBLIC base
        )

add_executable(mechanics_tests
        dice_test.c
        mechanics_tests.c
        )
target_link_libraries(mechanics_tests mechanics)
add_test(mechanics_tests mechanics_tests)

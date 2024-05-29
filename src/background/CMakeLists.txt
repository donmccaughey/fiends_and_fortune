add_library(background STATIC
        background.h
        direction.h
        direction.c
        language.h
        language.c
        money.h
        money.c
        orientation.h
        orientation.c
        )
target_include_directories(background
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
target_link_libraries(background
        PUBLIC base mechanics
        )

add_executable(background_tests
        background_tests.c
        direction_test.c
        language_test.c
        money_test.c
        orientation_test.c
        )
target_link_libraries(background_tests background)
add_test(background_tests background_tests)

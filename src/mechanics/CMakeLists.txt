add_library(mechanics STATIC
        xdice.h
        xdice.c
        mechanics.h
)
target_include_directories(mechanics
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_link_libraries(mechanics
        PUBLIC base
)

add_executable(mechanics_tests
        xdice_test.c
        mechanics_tests.c
)
target_link_libraries(mechanics_tests mechanics)
add_test(mechanics_tests mechanics_tests)

add_library(magic STATIC
        spell.c
        )
target_include_directories(magic
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
target_link_libraries(magic
        PUBLIC base mechanics
        )

add_executable(magic_tests
        magic_tests.c
        spell_test.c
        )
target_link_libraries(magic_tests magic)
add_test(magic_tests magic_tests)

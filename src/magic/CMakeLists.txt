add_library(magic STATIC
        magic.h
        spell.h
        spell.c
)
if(WALL)
    target_compile_options(magic
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_include_directories(magic
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_link_libraries(magic
        PUBLIC base
        PUBLIC mechanics
)

add_executable(magic_tests
        magic_tests.c
        spell_test.c
)
if(WALL)
    target_compile_options(magic_tests
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_link_libraries(magic_tests magic)
add_test(magic_tests magic_tests)

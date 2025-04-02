add_library(mechanics STATIC
        xdice.h
        xdice.c
        mechanics.h
)
if(WALL)
    target_compile_options(mechanics
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
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
if(WALL)
    target_compile_options(mechanics_tests
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_link_libraries(mechanics_tests mechanics)
add_test(mechanics_tests mechanics_tests)

add_library(base STATIC
        array.h
        base.h
        int.h
        int.c
        ptr_array.h
        ptr_array.c
        rnd.h
        rnd.c
        sort.h
        sort.c
        unreachable.h
        unreachable.c
)
if(WALL)
    target_compile_options(base
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_include_directories(base
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_link_libraries(base
        PUBLIC astr
        PUBLIC xmalloc
)


add_executable(base_tests
        base_tests.c
        int_test.c
        ptr_array_test.c
        rnd_test.c
        sort_test.c
)
if(WALL)
    target_compile_options(base_tests
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_link_libraries(base_tests base)
add_test(base_tests base_tests)

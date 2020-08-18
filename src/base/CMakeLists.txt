add_library(base STATIC
        alloc_or_die.c
        fail.c
        int.c
        ptr_array.c
        result.c
        rnd.c
        str.c
        )
target_include_directories(base
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )

add_executable(base_tests
        base_tests.c
        int_test.c
        ptr_array_test.c
        str_test.c
        result_test.c
        rnd_test.c
        )
target_include_directories(base_tests
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
target_link_libraries(base_tests
        PRIVATE base
        )
add_test(base_tests base_tests)

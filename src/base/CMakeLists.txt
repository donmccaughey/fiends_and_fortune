add_library(base STATIC
        fail.c
        int.c
        ptr_array.c
        result.c
        rnd.c
        sort.c
        str.c
)
if(HAS_REALLOCARRAY)
        target_compile_definitions(base PRIVATE HAS_REALLOCARRAY)
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
        sort_test.c
        result_test.c
        rnd_test.c
)
if(HAS_REALLOCARRAY)
        target_compile_definitions(base_tests PRIVATE HAS_REALLOCARRAY)
endif()
target_link_libraries(base_tests base)
add_test(base_tests base_tests)

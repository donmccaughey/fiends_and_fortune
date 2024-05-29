add_library(json STATIC
        json.h
        json.c
        )
target_include_directories(json
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
target_link_libraries(json
        PUBLIC astr
        PUBLIC cJSON
        PUBLIC xmalloc
        )

add_executable(json_tests
        json_tests.c
        )
target_link_libraries(json_tests json)
add_test(json_tests json_tests)

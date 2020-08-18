add_library(json STATIC
        json.c
        )
target_include_directories(json
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
target_link_libraries(json
        PUBLIC base cJSON
        )

add_executable(json_tests
        json_tests.c
        )
target_include_directories(json_tests
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
target_link_libraries(json_tests
        PRIVATE json base cJSON
        )
add_test(json_tests json_tests)

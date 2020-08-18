add_library(treasure STATIC
        coins.c
        gem.c
        jewelry.c
        magic_item.c
        treasure.c
        treasure_map.c
        treasure_type.c
        )
target_include_directories(treasure
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
target_link_libraries(treasure
        PUBLIC
            background
            base
            cJSON
            json
            magic
            mechanics
        )

add_executable(treasure_tests
        coins_test.c
        gem_test.c
        jewelry_test.c
        magic_item_test.c
        treasure_map_test.c
        treasure_test.c
        treasure_tests.c
        treasure_type_test.c
        )
target_link_libraries(treasure_tests treasure)
add_test(treasure_tests treasure_tests)

add_library(treasure STATIC
        coins.h
        coins.c
        gem.h
        gem.c
        jewelry.h
        jewelry.c
        magic_item.h
        magic_item.c
        treasure.h
        treasure.c
        treasure_map.h
        treasure_map.c
        treasure_struct.h
        treasure_type.h
        treasure_type.c
)
if(WALL)
    target_compile_options(treasure
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_include_directories(treasure
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_link_libraries(treasure
        PUBLIC background
        PUBLIC base
        PUBLIC cJSON
        PUBLIC json
        PUBLIC magic
        PUBLIC mechanics
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
if(WALL)
    target_compile_options(treasure_tests
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_link_libraries(treasure_tests treasure)
add_test(treasure_tests treasure_tests)

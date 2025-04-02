add_library(dungeon STATIC
        area.h
        area.c
        area_features.h
        box.h
        box.c
        digger.h
        digger.c
        dungeon.h
        dungeon.c
        dungeon_options.h
        dungeon_options.c
        exit.h
        exit.c
        generator.h
        generator.c
        level_map.h
        level_map.c
        periodic_check.h
        periodic_check.c
        point.h
        point.c
        size.h
        size.c
        text_rectangle.h
        text_rectangle.c
        tile.h
        tile.c
        tile_features.h
        tile_type.h
)
if(WALL)
    target_compile_options(dungeon
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_include_directories(dungeon
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
target_link_libraries(dungeon
        PUBLIC background
        PUBLIC base
        PUBLIC mechanics
)

add_executable(dungeon_tests
        area_test.c
        box_test.c
        digger_test.c
        dungeon_test.c
        dungeon_tests.c
        generator_test.c
        level_map_test.c
        point_test.c
        size_test.c
        text_rectangle_test.c
        tile_test.c
        tiles_thumbnail.h
        tiles_thumbnail.c
        tiles_thumbnail_test.c
)
if(WALL)
    target_compile_options(dungeon_tests
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
target_link_libraries(dungeon_tests dungeon)
add_test(dungeon_tests dungeon_tests)

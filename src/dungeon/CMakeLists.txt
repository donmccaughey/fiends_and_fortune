add_library(dungeon STATIC
        area.c
        box.c
        digger.c
        dungeon.c
        dungeon_options.c
        exit.c
        generator.c
        level_map.c
        periodic_check.c
        point.c
        size.c
        text_rectangle.c
        tile.c
        )
target_include_directories(dungeon
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
        )
target_link_libraries(dungeon
        PUBLIC background base mechanics
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
        tiles_thumbnail.c
        tiles_thumbnail_test.c
        )
target_link_libraries(dungeon_tests dungeon)
add_test(dungeon_tests dungeon_tests)

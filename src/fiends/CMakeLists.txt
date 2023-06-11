add_executable(fiends
        main.cpp

        application/application.cpp
        application/desk_top.cpp
        application/menu_bar.cpp
        application/status_line.cpp

        about/about.cpp
        about/about_dialog.cpp

        generate_dungeon/dungeon_window.cpp
        generate_dungeon/generate_dungeon.cpp

        generate_magic_items/generate_magic_items.cpp
        generate_magic_items/generate_magic_items_dialog.cpp
        generate_magic_items/magic_items_window.cpp

        generate_treasure/generate_treasure.cpp
        generate_treasure/generate_treasure_dialog.cpp
        generate_treasure/treasure_window.cpp

        treasure_types_table/treasure_types_table.cpp
        treasure_types_table/treasure_types_table_window.cpp

        util/text_rect.cpp
        util/window_numbers.cpp

        views/text_rect_view.cpp
        )
target_link_libraries(fiends
        base
        treasure
        tvision
        )
if(HAS_NO_UNQUALIFIED_STD_CAST_CALL)
    target_compile_options(fiends
            PRIVATE -Wno-unqualified-std-cast-call  # to use `move()` w/o `std::`
            )
endif()


add_executable(fiends_tests
        fiends_tests.cpp
        )
add_test(fiends_tests fiends_tests)

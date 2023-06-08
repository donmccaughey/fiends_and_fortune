add_executable(fiends
        main.cpp

        application/application.cpp
        application/desk_top.cpp
        application/menu_bar.cpp
        application/status_line.cpp

        about/about.cpp
        about/about_dialog.cpp

        generate_treasure/generate_treasure.cpp
        generate_treasure/generate_treasure_dialog.cpp
        generate_treasure/treasure_view.cpp
        generate_treasure/treasure_window.cpp

        treasure_types_table/treasure_types_table.cpp
        treasure_types_table/treasure_types_table_view.cpp
        treasure_types_table/treasure_types_table_window.cpp

        util/text_rect.cpp
        util/window_numbers.cpp
        )
target_link_libraries(fiends
        base
        treasure
        tvision
        )

add_executable(fiends_tests
        fiends_tests.cpp
        )
add_test(fiends_tests fiends_tests)

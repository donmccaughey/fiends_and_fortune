add_executable(fiends
        application.cpp
        desk_top.cpp
        main.cpp
        menu_bar.cpp
        status_line.cpp
        text_rect.cpp
        window_numbers.cpp

        about/about.cpp
        about/about_dialog.cpp

        generate_treasure/generate_treasure.cpp
        generate_treasure/generate_treasure_dialog.cpp
        generate_treasure/treasure_view.cpp
        generate_treasure/treasure_window.cpp

        treasure_types_table/treasure_types_table.cpp
        treasure_types_table/treasure_types_table_view.cpp
        treasure_types_table/treasure_types_table_window.cpp
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

add_executable(fiends
        about_dialog.cpp
        application.cpp
        desk_top.cpp
        generate_treasure_dialog.cpp
        main.cpp
        menu_bar.cpp
        status_line.cpp
        treasure_types_table_view.cpp
        treasure_types_table_window.cpp
        window_numbers.cpp
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

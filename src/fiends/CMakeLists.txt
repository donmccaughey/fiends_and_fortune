add_executable(fiends
        desk_top.cpp
        fiends_app.cpp
        main.cpp
        menu_bar.cpp
        status_line.cpp
        treasure_table_window.cpp
        window_numbers.cpp
        )
target_link_libraries(fiends
        tvision
        )

add_executable(fiends_tests
        fiends_tests.cpp
        )
add_test(fiends_tests fiends_tests)

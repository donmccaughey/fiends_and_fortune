add_library(tui STATIC
        app.c
        menu_view.c
        rect.c
        screen_view.c
        view.c
        window.c
        )
target_include_directories(tui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(tui
        PUBLIC
            base
            ${LIB_FORM}
            ${LIB_MENU}
            ${LIB_NCURSES}
        )

add_executable(tui_example
        tui_example.c
        )
target_link_libraries(tui_example tui)

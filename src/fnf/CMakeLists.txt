add_executable(fnf
        action.c
        options.c
        main.c)
target_link_libraries(fnf
        background
        base
        character
        cJSON
        dungeon
        json
        magic
        mechanics
        treasure
        )

add_executable(fnf_tests
        action.c
        action_test.c
        fnf_tests.c
        options.c
        options_test.c
        )
target_link_libraries(fnf_tests
        background
        base
        character
        cJSON
        dungeon
        json
        magic
        mechanics
        treasure
        )
add_test(fnf_tests fnf_tests)

add_custom_target(fnf_check
        COMMAND "./check"
        )
configure_file(check check)
add_test(fnf_check check)

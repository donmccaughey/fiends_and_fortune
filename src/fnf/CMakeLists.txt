add_executable(fnf
        action.h
        action.c
        main.c
        options.h
        options.c
)
if(HAS_OPTRESET)
        target_compile_definitions(fnf PRIVATE HAS_OPTRESET)
endif()
if(WALL)
    target_compile_options(fnf
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
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
if(WALL)
    target_compile_options(fnf_tests
            PRIVATE -Wall
            PRIVATE -Werror
    )
endif()
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

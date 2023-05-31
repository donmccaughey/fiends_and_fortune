add_executable(fiends
        fiends_app.cpp
        main.cpp
        )
target_link_libraries(fiends
        tvision
        )

add_executable(fiends_tests
        fiends_tests.cpp
        )
add_test(fiends_tests fiends_tests)

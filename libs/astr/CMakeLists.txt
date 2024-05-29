cmake_minimum_required(VERSION 3.15...3.27)
project(astr
        VERSION 1.0.0
        DESCRIPTION "astr"
        HOMEPAGE_URL https://git.sr.ht/~donmcc/astr
        LANGUAGES C
)


option(ADDRESS_SANITIZER "Enable the address sanitizer")
if(ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address -g)
    add_link_options(-fsanitize=address)
endif()

option(BUILD_TESTING "Build the tests" ON)

option(WALL "Enable all warnings")
if(WALL)
    add_compile_options(-Wall -Werror)
endif()


set(CMAKE_C_STANDARD 99)
enable_testing()
include(GNUInstallDirs)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(astr_is_top_level_project TRUE)
endif()


add_library(astr STATIC
        astr.h
        astr.c
)
target_include_directories(astr
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)


if(astr_is_top_level_project AND BUILD_TESTING)

    add_executable(astr_tests
            astr_tests.c
    )
    target_link_libraries(astr_tests astr)
    add_test(astr_tests astr_tests)


    add_library(astr_ndebug STATIC
            astr.h
            astr.c
    )
    target_compile_definitions(astr_ndebug
            PRIVATE NDEBUG
    )
    target_include_directories(astr
            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
    )


    add_executable(astr_ndebug_tests
            astr_ndebug_tests.c
    )
    target_link_libraries(astr_ndebug_tests astr_ndebug)
    add_test(astr_ndebug_tests astr_ndebug_tests)

endif()


install(TARGETS astr)
install(FILES astr.h TYPE INCLUDE)
install(FILES astr.3 TYPE MAN)

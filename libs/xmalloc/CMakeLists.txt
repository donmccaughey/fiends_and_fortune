cmake_minimum_required(VERSION 3.15...3.28)
project(xmalloc
        VERSION 0.0.1
        DESCRIPTION "xmalloc"
        HOMEPAGE_URL https://git.sr.ht/~donmcc/xmalloc
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

include(CheckSymbolExists)
check_symbol_exists(reallocarray "stdlib.h" has_reallocarray)
check_symbol_exists(recallocarray "stdlib.h" has_recallocarray)
check_symbol_exists(vasprintf "stdio.h" has_vasprintf)
if(NOT has_vasprintf)
    set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
    check_symbol_exists(vasprintf "stdio.h" has_vasprintf_gnu_source)
    if(NOT has_vasprintf_gnu_source)
        message(FATAL_ERROR "vasprintf() not found")
    endif()
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(xmalloc_is_top_level_project TRUE)
endif()


add_library(xmalloc STATIC
        xlibgen.c
        xmalloc.c
        xstdio.c
        xstring.c
        xunistd.c
)
target_include_directories(xmalloc PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
if(NOT has_reallocarray)
    target_compile_definitions(xmalloc PUBLIC XMALLOC_NEEDS_REALLOCARRAY)
endif()
if(NOT has_recallocarray)
    target_compile_definitions(xmalloc PUBLIC XMALLOC_NEEDS_RECALLOCARRAY)
endif()
if(has_vasprintf_gnu_source)
    target_compile_definitions(xmalloc PUBLIC _GNU_SOURCE)
endif()


if(xmalloc_is_top_level_project AND BUILD_TESTING)

    add_library(xmalloc_ndebug STATIC
            xlibgen.c
            xmalloc.c
            xstdio.c
            xstring.c
            xunistd.c
    )
    target_compile_definitions(xmalloc_ndebug PRIVATE NDEBUG)
    target_include_directories(xmalloc_ndebug
            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    if(NOT has_reallocarray)
        target_compile_definitions(xmalloc_ndebug
                PUBLIC XMALLOC_NEEDS_REALLOCARRAY
        )
    endif()
    if(NOT has_recallocarray)
        target_compile_definitions(xmalloc_ndebug
                PUBLIC XMALLOC_NEEDS_RECALLOCARRAY
        )
    endif()
    if(has_vasprintf_gnu_source)
        target_compile_definitions(xmalloc_ndebug PUBLIC _GNU_SOURCE)
    endif()

    add_executable(xlibgen_tests xlibgen_tests.c)
    target_link_libraries(xlibgen_tests xmalloc)
    add_test(xlibgen_tests xlibgen_tests)

    add_executable(xmalloc_tests xmalloc_tests.c)
    target_link_libraries(xmalloc_tests xmalloc)
    add_test(xmalloc_tests xmalloc_tests)

    add_executable(xstdio_tests xstdio_tests.c)
    target_link_libraries(xstdio_tests xmalloc)
    add_test(xstdio_tests xstdio_tests)

    add_executable(xstring_tests xstring_tests.c)
    target_link_libraries(xstring_tests xmalloc)
    add_test(xstring_tests xstring_tests)

    add_executable(xstring_ndebug_tests xstring_ndebug_tests.c)
    target_link_libraries(xstring_ndebug_tests xmalloc_ndebug)
    add_test(xstring_ndebug_tests xstring_ndebug_tests)

    add_executable(xunistd_tests xunistd_tests.c)
    target_link_libraries(xunistd_tests xmalloc)
    add_test(xunistd_tests xunistd_tests)

endif()


install(TARGETS xmalloc)
install(FILES xlibgen.h TYPE INCLUDE)
install(FILES xmalloc.h TYPE INCLUDE)
install(FILES xstdio.h TYPE INCLUDE)
install(FILES xstring.h TYPE INCLUDE)
install(FILES xunistd.h TYPE INCLUDE)

add_library(cJSON STATIC
        cJSON.c
        )
target_include_directories(cJSON
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

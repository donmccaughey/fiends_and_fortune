#ifndef FNF_COMMON_RESULT_H_INCLUDED
#define FNF_COMMON_RESULT_H_INCLUDED


#include <errno.h>
#include <stdbool.h>

#include "result_type.h"


struct result {
    enum result_type type;
    union {
        int error_code;
    };
};


inline bool
result_is_success(struct result result) {
    return result_type_success == result.type;
}


inline struct result
result_make_success(void) {
    return (struct result){
        .type=result_type_success
    };
}


inline struct result
result_make_system_error(void) {
    return (struct result){
        .type=result_type_system_error,
        .error_code=errno
    };
}


inline struct result
result_make_ncurses_err(void) {
    return (struct result){
        .type=result_type_ncurses_err
    };
}


inline struct result
result_make_ncurses_error(int error_code) {
    return (struct result){
        .type=result_type_ncurses_error,
        .error_code=error_code
    };
}


#endif

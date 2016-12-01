#ifndef FNF_COMMON_RESULT_H_INCLUDED
#define FNF_COMMON_RESULT_H_INCLUDED


#include <errno.h>
#include <stdbool.h>

#include "result_type.h"


struct result {
    enum result_type type;
    int error_code;
};


inline bool
result_is_success(struct result result)
{
    return result_type_success == result.type;
}


inline struct result
result_success(void)
{
    return (struct result){ .type=result_type_success };
}

    
inline struct result
result_system_error(void)
{
    return (struct result){
        .type=result_type_system_error,
        .error_code=errno
    };
}

    
inline struct result
result_ncurses_err(void)
{
    return (struct result){ .type=result_type_ncurses_err };
}


inline struct result
result_ncurses_errno(void)
{
    return (struct result){
        .type=result_type_ncurses_error,
        .error_code=errno
    };
}


inline struct result
result_ncurses_error(int error_code)
{
    return (struct result){
        .type=result_type_ncurses_error,
        .error_code=error_code
    };
}


void
result_print_error(struct result result);


inline struct result
result_set_system_error(int error_code)
{
    errno = error_code;
    return result_system_error();
}


#endif

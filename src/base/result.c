#include "result.h"

#include <stdio.h>
#include <string.h>


void
result_print_error(struct result result)
{
    switch (result.type) {
        case result_type_success:
            break;
        case result_type_ncurses_err:
            fprintf(stderr, "Ncurses Error\n");
            break;
        case result_type_ncurses_error:
            fprintf(stderr, "Ncurses Error: %i: %s\n",
                    result.error_code, strerror(result.error_code));
            break;
        case result_type_system_error:
            fprintf(stderr, "System Error: %i: %s\n",
                    result.error_code, strerror(result.error_code));
            break;
        default:
            fprintf(stderr, "Error\n");
            break;
    }
}


bool
result_is_success(struct result result)
{
    return result_type_success == result.type;
}


struct result
result_success(void)
{
    return (struct result){ .type=result_type_success };
}


struct result
result_system_error(void)
{
    return (struct result){
        .type=result_type_system_error,
        .error_code=errno
    };
}


struct result
result_ncurses_err(void)
{
    return (struct result){ .type=result_type_ncurses_err };
}


struct result
result_ncurses_errno(void)
{
    return (struct result){
        .type=result_type_ncurses_error,
        .error_code=errno
    };
}


struct result
result_ncurses_error(int error_code)
{
    return (struct result){
        .type=result_type_ncurses_error,
        .error_code=error_code
    };
}


struct result
result_set_system_error(int error_code)
{
    errno = error_code;
    return result_system_error();
}

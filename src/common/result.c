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


extern inline bool
result_is_success(struct result result);

extern inline struct result
result_success(void);

extern inline struct result
result_system_error(void);

extern inline struct result
result_ncurses_err(void);

extern inline struct result
result_ncurses_errno(void);

extern inline struct result
result_ncurses_error(int error_code);

extern inline struct result
result_set_system_error(int error_code);

#ifndef FNF_BASE_RESULT_H_INCLUDED
#define FNF_BASE_RESULT_H_INCLUDED


#include <errno.h>
#include <stdbool.h>
#include <base/result_type.h>


struct result {
    enum result_type type;
    int error_code;
};


bool
result_is_success(struct result result);

struct result
result_success(void);

struct result
result_system_error(void);

    
struct result
result_ncurses_err(void);

struct result
result_ncurses_errno(void);

struct result
result_ncurses_error(int error_code);

void
result_print_error(struct result result);

struct result
result_set_system_error(int error_code);


#endif

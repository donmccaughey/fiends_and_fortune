#include "result.h"


extern inline bool
result_is_success(struct result result);

extern inline struct result
result_make_ncurses_err(void);

extern inline struct result
result_make_ncurses_error(int error_code);

extern inline struct result
result_make_success(void);

extern inline struct result
result_make_system_error(void);
